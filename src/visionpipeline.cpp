/////////////////////////////////////////////////////////////////////////////
// Copyright:   (C) 2008-19 Cesar Mauri Loba - CREA Software Systems
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////
#include "visionpipeline.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"

#include "crvimage.h"
#include "timeutil.h"
#include "paths.h"
#include "simplelog.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/video/tracking.hpp>

#include <math.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

// Constants
#define DEFAULT_TRACK_AREA_WIDTH_PERCENT 0.50f
#define DEFAULT_TRACK_AREA_HEIGHT_PERCENT 0.30f
#define DEFAULT_TRACK_AREA_X_CENTER_PERCENT 0.5f
#define DEFAULT_TRACK_AREA_Y_CENTER_PERCENT 0.5f
#define DEFAULT_FACE_DETECTION_TIMEOUT 5000
#define COLOR_DEGRADATION_TIME 5000

using namespace cv;

static bool safeHaarCascadeLoad(CascadeClassifier& c, const char *fileName) {
	std::string fileName0(fileName);
	bool result = false;

	try {
		result= c.load(fileName0);
	}
	catch (cv::Exception& e) {
		SLOG_WARNING("Cannot load haar cascade: %s", e.what());
	}
	catch (...) {
		SLOG_WARNING("Error loading haar cascade");
	}

	return result;
}

CVisionPipeline::CVisionPipeline (wxThreadKind kind) 
: wxThread (kind)
// Actually it is not needed all the features a condition object offers, but
// we use it because we need a timeout based wait call. The associated mutex
// is not used at all.
, m_condition(m_mutex)
, m_faceLocationStatus(0) // 0 -> not available, 1 -> available
{
	InitDefaults();

	m_isRunning= false;
	m_trackAreaTimeout.SetWaitTimeMs(COLOR_DEGRADATION_TIME);

	//
	// Load face haarcascade
	// 
	wxString cascadePath (eviacam::GetDataDir() + _T("/haarcascade_frontalface_default.xml"));
	bool result = safeHaarCascadeLoad(m_faceCascade, cascadePath.mb_str(wxConvUTF8));
	if (!result) {
		// For OpenCV 2 on linux
		result = safeHaarCascadeLoad(m_faceCascade,
			"/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml");
	}

	if (!result) {
		// For OpenCV 3	on linux
		result = safeHaarCascadeLoad(m_faceCascade,
			"/usr/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
	}

    if (!result) {
		// For OpenCV 4	on linux
		result = safeHaarCascadeLoad(m_faceCascade,
			"/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");
	}
	
	if (!result) {
		wxMessageDialog dlg (NULL, _("The face localization option is not enabled."),
			_T("Enable Viacam"), wxICON_ERROR | wxOK );
		dlg.ShowModal();
		return;
	}

	// Create and start face detection thread
	if (Create() == wxTHREAD_NO_ERROR) {
#if defined (WIN32)
		// On linux this ends up calling setpriority syscall which changes
		// the priority of the whole process :-( (see wxWidgets threadpsx.cpp)
		// TODO: implement it using pthreads
		SetPriority (WXTHREAD_MIN_PRIORITY);
#endif
		m_isRunning= true;
		Run();
	}
}

CVisionPipeline::~CVisionPipeline () {
	if (!m_faceCascade.empty()) {
		m_isRunning= false;
		m_condition.Signal();
		Wait();
	}
}

void CVisionPipeline::AllocWorkingSpace (CIplImage &image)
{
	bool retval;

	if (!m_imgPrev.Initialized () ||
		image.Width() != m_imgPrev.Width() ||
		image.Height() != m_imgPrev.Height() ) {

		m_imageCopyMutex.Enter();
		retval= m_imgPrev.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);
		m_imageCopyMutex.Leave();

		retval= m_imgCurr.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);
	}
}

wxThreadError CVisionPipeline::Create(unsigned int stackSize)
{
	return wxThread::Create (stackSize);
}

// Low-priority secondary thread where face localization occurs
wxThread::ExitCode CVisionPipeline::Entry( )
{
	bool retval;
	unsigned long ts1 = 0;
	for (;;) {
		m_condition.WaitTimeout(1000);
		if (!m_isRunning) {
			break;
		}

		unsigned long now = CTimeUtil::GetMiliCount();
		if (now - ts1>= (unsigned long) m_threadPeriod) {
			ts1 = CTimeUtil::GetMiliCount();
			m_imageCopyMutex.Enter();
			if (!m_imgPrev.Initialized ()) {
				m_imageCopyMutex.Leave();
				continue;
			}
			
			if (!m_imgThread.Initialized () ||
						  m_imgPrev.Width() != m_imgThread.Width() ||
						  m_imgPrev.Height() != m_imgThread.Height() ) {				

				retval= m_imgThread.Create (m_imgPrev.Width(), m_imgPrev.Height(), 
					IPL_DEPTH_8U, "GRAY");
				assert (retval);
			}
			
			cvCopy(m_imgPrev.ptr(), m_imgThread.ptr());
			m_imageCopyMutex.Leave();

			ComputeFaceTrackArea(m_imgThread);
		}
	}
	return 0;
}

void CVisionPipeline::ComputeFaceTrackArea (CIplImage &image)
{
	if (!m_trackFace) return;
	if (m_faceLocationStatus) return;	// Already available

	Mat image0 = cvarrToMat(image.ptr());
	std::vector<Rect> faces;

	m_faceCascade.detectMultiScale(
		image0, faces, 1.5, 2,
		cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_CANNY_PRUNING,
		cvSize(65, 65));
	
	if (faces.size()> 0) {
		m_faceLocation = faces[0];
		m_faceLocationStatus = 1;

		m_waitTime.Reset();
		m_trackAreaTimeout.Reset();
	}
}

bool CVisionPipeline::IsFaceDetected () const
{
	return !m_waitTime.HasExpired();
}

static 
void DrawCorners(CIplImage &image, const std::vector<Point2f> corners, CvScalar color) {
	for (int i = 0; i < corners.size(); i++)
		cvCircle(image.ptr(), cvPoint(corners[i].x, corners[i].y), 1, color);
}

void CVisionPipeline::NewTracker(CIplImage &image, float &xVel, float &yVel)
{
	cv::Point2d trackAreaLocation;
	CvSize2D32f trackAreaSize;
	bool updateFeatures = false;

	// Face location has been updated?
	if (m_faceLocationStatus) {
		trackAreaLocation.x = m_faceLocation.x;
		trackAreaLocation.y = m_faceLocation.y;
		trackAreaSize.width = m_faceLocation.width;
		trackAreaSize.height = m_faceLocation.height;
		m_faceLocationStatus = 0;
		updateFeatures = true;
	}
	else {
		CvRect box;
		m_trackArea.GetBoxImg(&image, box);
		trackAreaLocation.x = box.x;
		trackAreaLocation.y = box.y;
		trackAreaSize.width = box.width;
		trackAreaSize.height = box.height;
		
        // Need to update corners?
		if (m_corners.size()< NUM_CORNERS) updateFeatures = true;
	}	

	if (updateFeatures) {
		// 
		// Set smaller area to extract features to track
		//
		#define SMALL_AREA_RATIO 0.4f

		CvRect featuresTrackArea;
		featuresTrackArea.x = trackAreaLocation.x + 
			trackAreaSize.width * ((1.0f - SMALL_AREA_RATIO) / 2.0f);
		featuresTrackArea.y = trackAreaLocation.y + 
			trackAreaSize.height * ((1.0f - SMALL_AREA_RATIO) / 2.0f);
		featuresTrackArea.width = trackAreaSize.width * SMALL_AREA_RATIO;
		featuresTrackArea.height = trackAreaSize.height * SMALL_AREA_RATIO;

		//
		// Find features to track
		//
		#define QUALITY_LEVEL  0.001   // 0.01
		#define MIN_DISTANTE 2

		m_imgPrev.SetROI(featuresTrackArea);
		m_imgCurr.SetROI(featuresTrackArea);

        Mat prevImg = cvarrToMat(m_imgPrev.ptr());
        Mat currImg = cvarrToMat(m_imgCurr.ptr());

        goodFeaturesToTrack(prevImg, m_corners, NUM_CORNERS, QUALITY_LEVEL, MIN_DISTANTE);
        TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
        if (m_corners.size()) {
		    cornerSubPix(prevImg, m_corners, Size(5, 5), Size(-1, -1), termcrit);
        }

		m_imgPrev.ResetROI();
		m_imgCurr.ResetROI();

		//
		// Update features location
		//
		for (int i = 0; i < m_corners.size(); i++) {
			m_corners[i].x += featuresTrackArea.x;
			m_corners[i].y += featuresTrackArea.y;
		}
	}

	if (slog_get_priority() >= SLOG_PRIO_DEBUG) {
	    DrawCorners(image, m_corners, cvScalar(255, 0, 0));
    }

	//
	// Track corners
	//
	CvRect ofTrackArea;
	ofTrackArea.x = trackAreaLocation.x;
	ofTrackArea.y = trackAreaLocation.y;
	ofTrackArea.width = trackAreaSize.width;
	ofTrackArea.height = trackAreaSize.height;
	m_imgPrev.SetROI(ofTrackArea);
	m_imgCurr.SetROI(ofTrackArea);

    Mat prevImg = cvarrToMat(m_imgPrev.ptr());
    Mat currImg = cvarrToMat(m_imgCurr.ptr());

	// Update corners location for the new ROI
	for (int i = 0; i < m_corners.size(); i++) {
		m_corners[i].x -= ofTrackArea.x;
		m_corners[i].y -= ofTrackArea.y;
	}

    vector<Point2f> new_corners;
    vector<uchar> status;
    vector<float> err;
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,14,0.03);
    if (m_corners.size()) {
        calcOpticalFlowPyrLK(
            prevImg, currImg, m_corners, new_corners, status, err, Size(11, 11), 0, termcrit);
    }
	
	m_imgPrev.ResetROI();
	m_imgCurr.ResetROI();

	// Update corners location
	for (int i = 0; i < m_corners.size(); i++) {
		m_corners[i].x += ofTrackArea.x;
		m_corners[i].y += ofTrackArea.y;
		new_corners[i].x += ofTrackArea.x;
		new_corners[i].y += ofTrackArea.y;
	}

	//
	// Accumulate motion (TODO: remove outliers?)
	//	
	int valid_corners = 0;
	float dx = 0, dy = 0;

	for (int i = 0; i< m_corners.size(); i++) {
		if (status[i] &&
			m_corners[i].x >= trackAreaLocation.x &&
			m_corners[i].x < trackAreaLocation.x + trackAreaSize.width &&
			m_corners[i].y >= trackAreaLocation.y &&
			m_corners[i].y < trackAreaLocation.y + trackAreaSize.height) {
			dx += m_corners[i].x - new_corners[i].x;
			dy += m_corners[i].y - new_corners[i].y;

			// Save new corner location
			m_corners[valid_corners++] = new_corners[i];
		}
	}
    m_corners.resize(valid_corners);

	if (valid_corners) {
		dx = dx / (float) valid_corners;
		dy = dy / (float) valid_corners;

		xVel = 2.0 * dx;
		yVel = 2.0 * -dy;
	}
	else {
		xVel = yVel = 0;
	}

	//
	// Update tracking area location
	//
	if (m_trackFace) {
		trackAreaLocation.x -= dx;
		trackAreaLocation.y -= dy;
	}
	
	//
	// Update visible tracking area
	//
	m_trackArea.SetSizeImg(&image, trackAreaSize.width, trackAreaSize.height);
	m_trackArea.SetCenterImg(&image, 
		trackAreaLocation.x + trackAreaSize.width / 2.0f, 
		trackAreaLocation.y + trackAreaSize.height / 2.0f);

	//
	// Draw corners
	//
	DrawCorners(image, m_corners, cvScalar(0, 255, 0));
}

bool CVisionPipeline::ProcessImage (CIplImage& image, float& xVel, float& yVel)
{
	try {
		AllocWorkingSpace(image);

		cvCvtColor(image.ptr(), m_imgCurr.ptr(), CV_BGR2GRAY);

		// TODO: fine grained synchronization
		m_imageCopyMutex.Enter();

		NewTracker(image, xVel, yVel);

		// Store current image as previous
		m_imgPrev.Swap(&m_imgCurr);
		m_imageCopyMutex.Leave();

		// Notifies face detection thread when needed
		if (m_trackFace) {
			m_trackArea.SetDegradation(255 - m_trackAreaTimeout.PercentagePassed() * 255 / 100);
			m_condition.Signal();
		}

		if (m_trackFace && m_enableWhenFaceDetected && !IsFaceDetected())
			return false;
		else
			return true;
	}
	catch (const std::exception& e) {
		SLOG_ERR("Exception: %s\n", e.what());
		exit(1);
	}

	return false;
}

enum ECpuValues { LOWEST = 1500, LOW = 800, NORMAL = 400, HIGH = 100, HIGHEST = 0 };

int CVisionPipeline::GetCpuUsage ()
{
	switch (m_threadPeriod)
	{
		case LOWEST:
			return (int) CVisionPipeline::ECpuUsage(CPU_LOWEST);
			break;
		case LOW:
			return (int) CVisionPipeline::ECpuUsage(CPU_LOW);
			break;
		case HIGH:
			return (int) CVisionPipeline::ECpuUsage(CPU_HIGH);
			break;
		case HIGHEST:
			return (int) CVisionPipeline::ECpuUsage(CPU_HIGHEST);
			break;
		default:
			return (int) CVisionPipeline::ECpuUsage(CPU_NORMAL);
			break;
	}
}

void CVisionPipeline::SetCpuUsage (int value)
{
	switch (value)
	{
		case (int) CVisionPipeline::ECpuUsage(CPU_LOWEST):
			SetThreadPeriod(LOWEST);
			break;
		case (int) CVisionPipeline::ECpuUsage(CPU_LOW):
			SetThreadPeriod(LOW);
			break;
		case (int) CVisionPipeline::ECpuUsage(CPU_NORMAL):
			SetThreadPeriod(NORMAL);
			break;
		case (int) CVisionPipeline::ECpuUsage(CPU_HIGH):
			SetThreadPeriod(HIGH);
			break;
		case (int) CVisionPipeline::ECpuUsage(CPU_HIGHEST):
			SetThreadPeriod(HIGHEST);
			break;
	}
}

void CVisionPipeline::SetThreadPeriod (int value)
{
	switch (value)
	{
		case LOWEST:
			m_threadPeriod= LOWEST;
			break;
		case LOW:
			m_threadPeriod= LOWEST;
			break;
		case HIGH:
			m_threadPeriod= HIGH;
			break;
		case HIGHEST:
			m_threadPeriod= HIGHEST;
			break;
		default:
			m_threadPeriod= NORMAL;
			break;
	}
}

//
// Configuration methods
//
void CVisionPipeline::InitDefaults()
{
	m_trackFace= true;
	m_enableWhenFaceDetected= false;
	m_waitTime.SetWaitTimeMs(DEFAULT_FACE_DETECTION_TIMEOUT);
	SetThreadPeriod(CPU_NORMAL);
	m_trackArea.SetSize (DEFAULT_TRACK_AREA_WIDTH_PERCENT, DEFAULT_TRACK_AREA_HEIGHT_PERCENT);
	m_trackArea.SetCenter (DEFAULT_TRACK_AREA_X_CENTER_PERCENT, DEFAULT_TRACK_AREA_Y_CENTER_PERCENT);
	
}

void CVisionPipeline::WriteProfileData(wxConfigBase* pConfObj)
{
	float xc, yc, width, height;

	pConfObj->SetPath (_T("motionTracker"));	

	pConfObj->Write(_T("trackFace"), m_trackFace);
	pConfObj->Write(_T("enableWhenFaceDetected"), m_enableWhenFaceDetected);
	pConfObj->Write(_T("locateFaceTimeout"), (int) m_waitTime.GetWaitTimeMs());
	pConfObj->Write(_T("threadPeriod"), (int) m_threadPeriod);

	m_trackArea.GetSize (width, height);
	
	pConfObj->Write (_T("trackAreaWidth"), (double) width);
	pConfObj->Write (_T("trackAreaHeight"), (double) height);

	if (!m_trackFace) {		
		m_trackArea.GetCenter (xc, yc);
		pConfObj->Write (_T("trackAreaCenterX"), (double) xc);
		pConfObj->Write (_T("trackAreaCenterY"), (double) yc);
	}

	pConfObj->SetPath (_T(".."));
}

void CVisionPipeline::ReadProfileData(wxConfigBase* pConfObj)
{
	// Ensure proper default values if read fails
	double	xc= DEFAULT_TRACK_AREA_X_CENTER_PERCENT, 
		yc= DEFAULT_TRACK_AREA_Y_CENTER_PERCENT,
                width= DEFAULT_TRACK_AREA_WIDTH_PERCENT,
		height= DEFAULT_TRACK_AREA_HEIGHT_PERCENT;
				
	int locateFaceTimeout = DEFAULT_FACE_DETECTION_TIMEOUT;
	int threadPeriod = -1;

	pConfObj->SetPath (_T("motionTracker"));
	pConfObj->Read(_T("trackFace"), &m_trackFace);
	pConfObj->Read(_T("enableWhenFaceDetected"), &m_enableWhenFaceDetected);
	pConfObj->Read(_T("locateFaceTimeout"), &locateFaceTimeout);
	pConfObj->Read (_T("trackAreaWidth"), &width);
	pConfObj->Read (_T("trackAreaHeight"), &height);
	pConfObj->Read (_T("threadPeriod"), &threadPeriod);
	
	SetThreadPeriod(threadPeriod);
	
	m_trackArea.SetSize ((float) width, (float)height);
	m_waitTime.SetWaitTimeMs(locateFaceTimeout);

	if (!m_trackFace) {
		pConfObj->Read (_T("trackAreaCenterX"), &xc);
		pConfObj->Read (_T("trackAreaCenterY"), &yc);

		m_trackArea.SetCenter ((float)xc, (float)yc);		
	}

	pConfObj->SetPath (_T(".."));
}
