/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_v4l2.cpp
// Purpose:	Provide a camera capture class around v4l2 and libwebcam
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     17/05/2010
// Copyright:   (C) 2008-10 Cesar Mauri Loba - CREA Software Systems
//              Portions of guvcview are (c) of Paulo Assis and others
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
#include "crvcamera_v4l2.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <list>
#include "pwc-ioctl.h"
#include "colorspaces.h"
#if !defined(NDEBUG)
#include <iostream>
#endif

#define IOCTL_RETRY 4

#ifndef V4L2_PIX_FMT_SRGGB8
	#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /* RGRG.. GBGB..    */
#endif

/* ioctl with a number of retries in the case of failure
* args:
* fd - device descriptor
* IOCTL_X - ioctl reference
* arg - pointer to ioctl data
* returns - ioctl result
* Based on xioctl from guvcview
*/
static
int xioctl(int fd, int IOCTL_X, void *arg)
{
	int ret = 0;
	int tries= IOCTL_RETRY;
	do {
		ret = ioctl(fd, IOCTL_X, arg);
	} 
	while (ret && tries-- && ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

	if (ret && (tries <= 0)) fprintf(stderr, "ioctl (%i) retried %i times - giving up: %s)\n", IOCTL_X, IOCTL_RETRY, strerror(errno));
	
	return (ret);
} 

// TODO: remove 2nd parameter from constructor
CCameraV4L2::CCameraV4L2(const char* device_name, bool usePwc)
{
	strncpy (m_deviceShortName, device_name, CCameraEnum::CAM_DEVICE_SHORT_NAME_LENGHT);
	m_Fd = -1;	
	m_libWebcamHandle= 0;
	m_captureMethod= CAP_NONE;
	m_usePwc= usePwc;
	m_isStreaming= false;
	m_buffersReady= false;
	memset(&m_captureBuffersInfo, 0, sizeof(struct v4l2_buffer) * STREAMING_CAPTURE_NBUFFERS);
	memset(&m_captureBuffersPtr, 0, sizeof(void*) * STREAMING_CAPTURE_NBUFFERS);	
	memset(&m_currentFormat, 0, sizeof(m_currentFormat));
	AddSupportedPixelFormats ();
}

void CCameraV4L2::AddSupportedPixelFormats ()
{
	// Adds supported pixel formats in preferred order
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_RGB24);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_YUYV);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_YUV420);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SGBRG8);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SGRBG8);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SBGGR8);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SRGGB8);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_UYVY);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_YVYU);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_YYUV);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_YVU420);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_NV12);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_NV21);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_NV16);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_NV61);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_Y41P);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_GREY);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SPCA501);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SPCA505);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_SPCA508);
	m_supportedPixelFormats.push_back (V4L2_PIX_FMT_BGR24);
}

bool CCameraV4L2::PopulateCameraControls ()
{
	CControl *controls= NULL;
	unsigned int size= 0, count= 0;

	// First call to discover required buffer size
	if (c_enum_controls(m_libWebcamHandle, controls, &size, &count)!= C_BUFFER_TOO_SMALL) return false;
	else {
		// Ok, allocate buffer and query information
		assert (size> 0);
		unsigned char buffer[size];
		controls= (CControl *) buffer;
		if (c_enum_controls(m_libWebcamHandle, controls, &size, &count)!= C_SUCCESS) return false;
		
		// Populate camera control vector
		for (unsigned int i= 0; i< count; ++i) {
			if (controls[i].type!=  CC_TYPE_RAW)
				// Ignore raw controls
				m_cameraControls.push_back(CCameraControlV4l2(m_libWebcamHandle, controls[i]));
		}
	}
	return true;
}

unsigned int CCameraV4L2::GetCameraControlsCount()
{
	return m_cameraControls.size();
}

CCameraControl* CCameraV4L2::GetCameraControl(unsigned int numControl)
{
	if (numControl>= GetCameraControlsCount()) return NULL;
	return &m_cameraControls[numControl];
}

CCameraV4L2::~CCameraV4L2(void)
{
	Close ();	
}

void CCameraV4L2::Close ()
{
	if (m_isStreaming) EnableVideo(false);	
	if (m_buffersReady) DeallocateBuffers();
	if (m_Fd!= -1) { 
		close(m_Fd); 
		m_Fd = -1; 
	}
	if (m_libWebcamHandle!= 0) {
		c_close_device (m_libWebcamHandle);
		m_libWebcamHandle= 0;
	}
	m_captureMethod= CAP_NONE;
	m_cameraControls.clear();
}

bool CCameraV4L2::DoOpen()
{
	char devName[CCameraEnum::CAM_DEVICE_SHORT_NAME_LENGHT+5];
	struct stat st;	

	// Create device name
	snprintf (devName, CCameraEnum::CAM_DEVICE_SHORT_NAME_LENGHT+5, "/dev/%s", m_deviceShortName);

	// Check if exists and if it is a device
	if (stat (devName, &st)== -1) {
		fprintf (stderr, "ERROR: Cannot identify ’%s’: %d, %s\n", devName, errno, strerror (errno));
		return false;
	}
	if (!S_ISCHR (st.st_mode)) {
		fprintf (stderr, "ERROR: %s is no device\n", devName);
		return false;
	}
	
	// "Open" device via libwebcam 
	m_libWebcamHandle= c_open_device (m_deviceShortName);
	if (m_libWebcamHandle== 0) {
		fprintf (stderr, "ERROR: Cannot open ’%s’ via libwebcam\n", devName);
		Close();
		return false;
	}
	
	PopulateCameraControls ();
	
	// Open device
	m_Fd= open (devName, O_RDWR);	
	if (m_Fd== -1) {
		fprintf (stderr, "ERROR: Cannot open ’%s’: %d, %s\n", devName, errno, strerror (errno));
		Close();
		return false;
	}
	
	return true;
}
	
// Do VIDIOC_REQBUFS	
bool CCameraV4L2::RequestBuffers(enum v4l2_memory mem)
{
	struct v4l2_requestbuffers requestbuffers;

	memset (&requestbuffers, 0, sizeof(requestbuffers));
	requestbuffers.count= STREAMING_CAPTURE_NBUFFERS;
	requestbuffers.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	requestbuffers.memory= mem; //V4L2_MEMORY_MMAP;
	if (ioctl(m_Fd, VIDIOC_REQBUFS, &requestbuffers)== 0) {
		if (requestbuffers.count== STREAMING_CAPTURE_NBUFFERS) return true;
		if (requestbuffers.count> 0) UnRequestBuffers(mem);		
	}
	fprintf (stderr, "ERROR: RequestBuffers: failed\n");
	return false;
}

// Undo VIDIOC_REQBUFS
bool CCameraV4L2::UnRequestBuffers(enum v4l2_memory mem)
{
	struct v4l2_requestbuffers requestbuffers;

	memset (&requestbuffers, 0, sizeof(requestbuffers));
	requestbuffers.count= 0;
	requestbuffers.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	requestbuffers.memory= mem; //V4L2_MEMORY_MMAP;
	if (ioctl(m_Fd, VIDIOC_REQBUFS, &requestbuffers)== 0) return true;
	return false;
}
	
// This method must be AFTER desired format is set	
CCameraV4L2::ECaptureMethod CCameraV4L2::DetectCaptureMethod()
{
	struct v4l2_capability capability;	

	// Query basic capabilities. This never should fail
	if (-1 == ioctl (m_Fd, VIDIOC_QUERYCAP, &capability)) {
#if !defined(NDEBUG)
		fprintf (stderr, "ERROR: Cannot query camera capabilities: VIDIOC_QUERYCAP ioctl failed\n");
#endif
		return CAP_NONE;
	}

	if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
#if !defined(NDEBUG)
	    fprintf (stderr, "ERROR: is no video capture device\n");
#endif
	    return CAP_NONE;
	}

	// Driver supports streaming
	if ((capability.capabilities & V4L2_CAP_STREAMING)) {
		// Streaming supported. What kind?
		// It can be V4L2_MEMORY_MMAP or V4L2_MEMORY_USERPTR

		// Check MMAP first as preferent option
		if (RequestBuffers(V4L2_MEMORY_MMAP)) {
			UnRequestBuffers(V4L2_MEMORY_MMAP);
			return CAP_STREAMING_MMAP;
		}

		// Check using user-allocated memory
		if (RequestBuffers(V4L2_MEMORY_USERPTR)) {
			UnRequestBuffers(V4L2_MEMORY_USERPTR);
			return CAP_STREAMING_USR;
		}
	}
	if (capability.capabilities & V4L2_CAP_READWRITE) return CAP_READ;

	return CAP_NONE;
}

// Only used by DetectBestImageFormat
typedef struct  {
	unsigned int min_frame_rate;
	unsigned int max_frame_rate;
	unsigned int step_frame_rate;
	unsigned int min_width;
	unsigned int min_height;
	unsigned int max_width;
	unsigned int max_height;
	unsigned int step_width;
	unsigned int step_height;		
	uint32_t pixelformat;	// Four character code	
} TImageFormatEx;
#if !defined(NDEBUG)
static 
void dump_TImageFormatEx (TImageFormatEx& ife)
{
	std::cout << "dump_TImageFormatEx. fr: " << ife.min_frame_rate << ", " << ife.max_frame_rate << ", " << ife.step_frame_rate 
		<< "w:" << ife.min_width << ", " << ife.max_width << ", " << ife.step_width 
		<< "h:" << ife.min_height << ", " << ife.max_height << ", " << ife.step_height 
		<< "px;" << ife.pixelformat << std::endl;
}
#endif

// Given a range returns the absolute distance to a value
template <class T>
T abs_distance_to_range (T min, T max, T val)
{
	if (val< min) return min-val;
	if (val< max) return 0;
	return val-max;
}

// Given a steped range and a desired value, selects a value within 
// the range as near as possible to the desired value
static 
uint range_value_fit (uint min, uint max, uint step, uint val)
{
	if (val<= min) return min;
	if (val>= max) return max;
	
	// Adjust step
	if (step== 0) step= 1;
	val-= (val % step);
	if (val< min) val= min;
	return val;	
}


// Look for best matching entries for a desired frame rate and remove all other entries
// fr= 0 means that nothing is done (all formats are appropiate), fr=UINT_MAX selects highest
// frame rate
static void SelectBestFrameRate (unsigned int fr, std::list<TImageFormatEx>& availableFormats)
{
	if (fr> 0) {	
		unsigned int bestdiff= UINT_MAX;
		// Find closest frame ratio	
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			unsigned int diff= abs_distance_to_range<uint> (i->min_frame_rate, i->max_frame_rate, fr);
			if (diff< bestdiff) bestdiff= diff;
		}
		// Remove worse entries
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end();)
			if (abs_distance_to_range<uint> (i->min_frame_rate, i->max_frame_rate, fr)!= bestdiff) 
				i= availableFormats.erase(i);
			else
				++i;
		
		assert (availableFormats.size()> 0);
	}
}

// Sizes. Chooses closest number of pixels (width*height) to the requested
static void SelectBestFramePixelNumber (unsigned int npixels, std::list<TImageFormatEx>& availableFormats)
{
	if (npixels> 0) {
		unsigned int bestdiff= UINT_MAX;
		//unsigned int npixels= imgformat.width * imgformat.height;
		// Find closest frame ratio	
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			unsigned int diff= abs_distance_to_range<uint> (i->min_width * i->min_height, i->max_width * i->max_height, npixels);
			if (diff< bestdiff) bestdiff= diff;
		}
		// Remove worse entries
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end();)
			if (abs_distance_to_range<uint> (i->min_width * i->min_height, i->max_width * i->max_height, npixels)!= bestdiff) 
				i= availableFormats.erase(i);
			else
				++i;
		
		assert (availableFormats.size()> 0);		
	}
}


// Given a set of desired parameters finds the most suitable configuration which the camera supports
//
// Initialize the fields of the TImageFormat struct with the desired value for
// each field or 0 to express that any possibility is fine or UINT_MAX to request
// the maximum available value. 

bool CCameraV4L2::DetectBestImageFormat(TImageFormat& imgformat)
{
	std::list<TImageFormatEx> availableFormats;

	//
	// First build an array containing all possible formats
	//
	
	unsigned int sformats, ssizes, sintervals;
	unsigned int cformats, csizes, cintervals;
	// Request needed buffer to store formats
	sformats= cformats= 0;	
	if (c_enum_pixel_formats (m_libWebcamHandle, NULL, &sformats, &cformats)!= C_BUFFER_TOO_SMALL) return false;
	else {
		unsigned char bufferformats[sformats];
		CPixelFormat* formats= (CPixelFormat*) bufferformats;
		// Get formats
		if (c_enum_pixel_formats (m_libWebcamHandle, formats, &sformats, &cformats)!= C_SUCCESS) return false;
		
		for (unsigned int ifo= 0; ifo< cformats; ++ifo) {
			TImageFormatEx tmpif;
			memset(&tmpif, 0, sizeof(tmpif));
			
			// Store pixel format
			tmpif.pixelformat= v4l2_fourcc (formats[ifo].fourcc[0], formats[ifo].fourcc[1], formats[ifo].fourcc[2], formats[ifo].fourcc[3]);
						
			// Request needed buffer to store sizes
			ssizes= csizes= 0;
			if (c_enum_frame_sizes (m_libWebcamHandle, &formats[ifo], NULL, &ssizes, &csizes)!= C_BUFFER_TOO_SMALL)
				// No frame sizes detected, should not happen but we cope with it anyway
				availableFormats.push_back(tmpif);			
			else {
				unsigned char buffersizes[ssizes];
				CFrameSize* sizes= (CFrameSize*) buffersizes;
				// Get sizes
				if (c_enum_frame_sizes (m_libWebcamHandle, &formats[ifo], sizes, &ssizes, &csizes)!= C_SUCCESS) {
					// Unlikely but we cope with it anyway
					availableFormats.push_back(tmpif);
					continue;
				}
				
				for (unsigned int is= 0; is< csizes; ++is) {
					// Store size
					if (sizes[is].type== CF_SIZE_CONTINUOUS) {
						tmpif.min_width= sizes[is].min_width;
						tmpif.min_height= sizes[is].min_height;
						tmpif.max_width= sizes[is].max_width;
						tmpif.max_height= sizes[is].max_height;
						tmpif.step_width= sizes[is].step_width;
						tmpif.step_height= sizes[is].step_height;
					} 
					else {
						tmpif.min_width= sizes[is].width;
						tmpif.min_height= sizes[is].height;
						tmpif.max_width= sizes[is].width;
						tmpif.max_height= sizes[is].height;
						tmpif.step_width= 1;
						tmpif.step_height= 1;
					}
					
					// Request needed buffer to store intervals
					sintervals= cintervals= 0;
					if (c_enum_frame_intervals (m_libWebcamHandle, &formats[ifo], &sizes[is], NULL, &sintervals, &cintervals)!= C_BUFFER_TOO_SMALL)
						// No intervals detected. Some cameras doesn't provide this information
						availableFormats.push_back(tmpif);
					else {
						unsigned char bufferintervals[sintervals];
						CFrameInterval* intervals= (CFrameInterval*) bufferintervals;
						// Get intervals
						if (c_enum_frame_intervals (m_libWebcamHandle, &formats[ifo], &sizes[is], intervals, &sintervals, &cintervals)!= C_SUCCESS) {
							// Unlikely but we cope with it anyway
							availableFormats.push_back(tmpif);
							continue;
						}
						
						for (unsigned int ii= 0; ii< cintervals; ++ii) {
							// Store frame rate
							if (intervals[ii].type== CF_INTERVAL_DISCRETE) {
								tmpif.max_frame_rate= tmpif.min_frame_rate= (intervals[ii].n? intervals[ii].d / intervals[ii].n : 0);
								tmpif.step_frame_rate= 1;
							}
							else {
								tmpif.max_frame_rate= (intervals[ii].max_n? intervals[ii].max_d / intervals[ii].max_n : 0);
								tmpif.min_frame_rate= (intervals[ii].min_n? intervals[ii].min_d / intervals[ii].min_n : 0);
								tmpif.step_frame_rate= (intervals[ii].step_n? intervals[ii].step_d / intervals[ii].step_n : 0);
								if (tmpif.step_frame_rate== 0) tmpif.step_frame_rate= 1;
							}
							
							availableFormats.push_back(tmpif);				
						}
					}
				}
				
			}			
		}
	}
#if !defined(NDEBUG)	
	for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
		dump_TImageFormatEx (*i);
	}
#endif
	//
	// Selection process
	//
	
	// Filter by compatible pixel formats. Remove entries which use non supported encodings
	for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end();) {
		bool found= false;
		for (unsigned int ienc= 0; !found && ienc< m_supportedPixelFormats.size(); ++ienc)
			if (m_supportedPixelFormats[ienc]== i->pixelformat) found= true;
		
		if (!found) {			
			char* tmp= (char *) &i->pixelformat;
			std::cerr << "crvcamera_v4l2: discarding unsuported format: " << tmp[0] << tmp[1] << tmp[2] << tmp[3] << std::endl;
			i= availableFormats.erase (i);
		}
		else ++i;
	}

	// No available formats detected
	if (availableFormats.size()== 0) return false;

	// As for realtime computer vision frame rate is usually a critical parameter we first choose it.
	SelectBestFrameRate (imgformat.frame_rate, availableFormats);
	
	// Sizes. Chooses closest number of pixel (width*height) to the requested
	SelectBestFramePixelNumber (imgformat.width * imgformat.height, availableFormats);
	
	// Check aspect ratio
	if (imgformat.width> 0 && imgformat.height> 0) {
		float bestdiff= FLT_MAX;
		float aratio= (float) imgformat.width / (float) imgformat.height;
		// Find closest frame ratio	
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			unsigned int diff= abs_distance_to_range<float> ((float)i->min_width / (float)i->max_height, (float)i->max_width / (float)i->min_height, aratio);
			if (diff< bestdiff) bestdiff= diff;
		}
		// Remove worst entries
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end();)
			if (abs_distance_to_range<float> ((float)i->min_width / (float)i->max_height, (float)i->max_width / (float)i->min_height, aratio)!= bestdiff) 
				i= availableFormats.erase(i);
			else
				++i;
		
		assert (availableFormats.size()> 0);
	}
	
	// If frame rate not explicity specified then selects highest fr available
	if (imgformat.frame_rate== 0) {
		imgformat.frame_rate= UINT_MAX;
		SelectBestFrameRate (imgformat.frame_rate, availableFormats);
	}
	
	// If frame size not explicity specified then selects bigger frame size available
	if (imgformat.width== 0 || imgformat.height== 0) {
		if (!imgformat.width) imgformat.width= UINT_MAX;
		if (!imgformat.height) imgformat.height= UINT_MAX;		
		SelectBestFramePixelNumber (UINT_MAX, availableFormats);
	}
	
	// Finally chooses best available pixelformat
	for (unsigned int ienc= 0; ienc< m_supportedPixelFormats.size(); ++ienc) {		
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			if (m_supportedPixelFormats[ienc]== i->pixelformat) {
				// Bingo! Store data and finish
				imgformat.pixelformat= m_supportedPixelFormats[ienc];
				imgformat.frame_rate= range_value_fit (i->min_frame_rate, i->max_frame_rate, i->step_frame_rate, imgformat.frame_rate);
				imgformat.width= range_value_fit (i->min_width, i->max_width, i->step_width, imgformat.width);
				imgformat.height= range_value_fit (i->min_height, i->max_height, i->step_height, imgformat.height);
				return true;
			}
		}
	}
	
	// Execution should never reach this point
	assert (false);
	return false;
}

bool CCameraV4L2::SetImageFormat(const TImageFormat& imgformat)
{
	struct v4l2_format format;
	
	//Set frame format
	memset (&format, 0, sizeof (format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl (m_Fd, VIDIOC_G_FMT, &format)== -1) {
		fprintf(stderr, "ERROR: Unable to get format.\n");
		return false;	
	}
	
	format.fmt.pix.width = imgformat.width;
	format.fmt.pix.height = imgformat.height;
	format.fmt.pix.pixelformat = imgformat.pixelformat;
	format.fmt.pix.field = V4L2_FIELD_ANY;

	if (ioctl (m_Fd, VIDIOC_S_FMT, &format)== -1) {
		fprintf(stderr, "ERROR: Unable to set format.\n");
		return false;	
	}
	
	// Store currently set format (last VIDIOC_S_FMT may have changed it)
	m_currentFormat.frame_rate= imgformat.frame_rate;
	m_currentFormat.width= format.fmt.pix.width;
	m_currentFormat.height= format.fmt.pix.height;
	m_currentFormat.pixelformat= format.fmt.pix.pixelformat;	

	//Set framerate
	if (m_usePwc) {
		bool properlySet= false;
		struct video_window vwin;

		// Using a PWC based camera
		if ((ioctl(m_Fd, VIDIOCGWIN, &vwin) == 0) && (vwin.flags & PWC_FPS_FRMASK)) {
			vwin.flags &= ~PWC_FPS_FRMASK;
			vwin.flags |= (imgformat.frame_rate << PWC_FPS_SHIFT);
			if (ioctl(m_Fd, VIDIOCSWIN, &vwin) == 0) properlySet= true;
		}

		if (!properlySet) fprintf (stderr, "Warning: cannot set FPS: %d for PWC camera\n", imgformat.frame_rate);
	}
	else {
		bool properlySet= false;
		struct v4l2_streamparm parm;

		// An UVC camera is assumed
		memset(&parm, 0, sizeof (parm));
		parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
		if (ioctl (m_Fd, VIDIOC_G_PARM, &parm)== 0) {
			parm.parm.capture.timeperframe.numerator = 1;
			parm.parm.capture.timeperframe.denominator = imgformat.frame_rate;
			if (ioctl(m_Fd, VIDIOC_S_PARM, &parm)== 0) properlySet= true;
		}
		if (!properlySet) fprintf (stderr, "Warning: cannot set FPS: %d for UVC camera\n", imgformat.frame_rate);
		
		// Try to set exposure control to match desired frame ratio
		for (unsigned int i= 0; i< m_cameraControls.size(); ++i) {
			if (m_cameraControls[i].GetId()== CCameraControl::CAM_AUTO_EXPOSURE_PRIORITY) {
				m_cameraControls[i].SetValue(0);
				break;
			}
		}			
	}
	return true;
}

void CCameraV4L2::UnmapBuffers()
{
	for (int i= STREAMING_CAPTURE_NBUFFERS-1; i>= 0; --i) {
		if (m_captureBuffersPtr[i]!= NULL) {
			if (munmap(m_captureBuffersPtr[i], m_captureBuffersInfo[i].length)!= 0) perror("couldn't unmap buff");
			m_captureBuffersPtr[i]= NULL;
		}
	}
}

// Enable/disable video
bool CCameraV4L2::EnableVideo(bool enable)
{
	if (m_Fd== -1) return false;	// Camera not open, fail
	if (enable== m_isStreaming) return true;
	
	switch(m_captureMethod)	{		
	case CAP_READ:
		// capture using read, nothing to enable
		break;
	case CAP_STREAMING_MMAP: 
	    {
		int type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		int action= (enable? VIDIOC_STREAMON : VIDIOC_STREAMOFF);
		if (xioctl(m_Fd, action, &type)!= 0) return false;
		break;
	    }	
	case CAP_STREAMING_USR:
		// Not implemented
		fprintf (stderr, "CAP_STREAMING_USR: feature not implemented\n");	
		return false;
	default:
		// Never should happen
		assert (false);
		return false;		
	}
	m_isStreaming= enable;

	return true;
}

// Allocate buffers before start capturing
bool CCameraV4L2::AllocateBuffers()
{
	if (m_isStreaming) {
		fprintf (stderr, "ERROR: AllocateBuffers: trying to allocate buffers while streaming\n");
		return false;
	}
	if (m_Fd== -1) {
		fprintf (stderr, "ERROR: AllocateBuffers: device not open\n");
		return false;
	}
	if (m_buffersReady) return true; // Already allocated
	
	if (m_captureMethod== CAP_READ) {
		// TODO
	}
	else if (m_captureMethod== CAP_STREAMING_MMAP) {
		// Request buffers
		if (!RequestBuffers(V4L2_MEMORY_MMAP)) {
			perror("VIDIOC_REQBUFS - Unable to allocate buffers");
			return false;
		}
		
		//  Buffers details
		for (int i= 0; i< STREAMING_CAPTURE_NBUFFERS; ++i) {
			memset(&m_captureBuffersInfo[i], 0, sizeof(struct v4l2_buffer));
			m_captureBuffersInfo[i].index= i;
			m_captureBuffersInfo[i].type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
			m_captureBuffersInfo[i].memory= V4L2_MEMORY_MMAP;				
			if (xioctl(m_Fd, VIDIOC_QUERYBUF, &m_captureBuffersInfo[i])!= 0) {
				perror("VIDIOC_QUERYBUF - Unable to query buffer");
				UnRequestBuffers(V4L2_MEMORY_MMAP);
				return false;
			}
			if (m_captureBuffersInfo[i].length <= 0) 
				fprintf (stderr, "WARNING VIDIOC_QUERYBUF - buffer length is %d\n", m_captureBuffersInfo[i].length);
		}
		
		// MMap buffers
		memset(&m_captureBuffersPtr, 0, sizeof(void*) * STREAMING_CAPTURE_NBUFFERS);
		for (int i= 0; i< STREAMING_CAPTURE_NBUFFERS; ++i) {
			m_captureBuffersPtr[i]= mmap(NULL, m_captureBuffersInfo[i].length, PROT_READ | PROT_WRITE, MAP_SHARED, m_Fd, m_captureBuffersInfo[i].m.offset);
			if (m_captureBuffersPtr[i]== MAP_FAILED) {
				m_captureBuffersPtr[i]= NULL;
				perror("Unable to map buffer");
				UnmapBuffers();
				UnRequestBuffers(V4L2_MEMORY_MMAP);
				return false;
			}
		}
		
		// Queue buffers
		for (int i= 0; i< STREAMING_CAPTURE_NBUFFERS; ++i) {
			if (xioctl(m_Fd, VIDIOC_QBUF, &m_captureBuffersInfo[i])!= 0) {
				perror("VIDIOC_QBUF - Unable to queue buffer");
				UnmapBuffers();
				UnRequestBuffers(V4L2_MEMORY_MMAP);
				return false;
			}
		}
			
		// Result image	
		// TODO: make sure that image is not initialized with padded rows
		if (!m_resultImage.Create (m_currentFormat.width, m_currentFormat.height, IPL_DEPTH_8U, "RGB", IPL_ORIGIN_TL, IPL_ALIGN_DWORD )) {
			fprintf (stderr, "Cannot create result image\n");
			UnmapBuffers();
			UnRequestBuffers(V4L2_MEMORY_MMAP);
			return false;
		}
	}
	else if (m_captureMethod== CAP_STREAMING_USR) {
		fprintf (stderr, "ERROR: AllocateBuffers: CAP_STREAMING_USR not implemented\n");
		assert (false);
		return false;
	}
	else {
		fprintf (stderr, "ERROR: AllocateBuffers: capture method not set\n");
		return false;
	}
	m_buffersReady= true;
	return true;
}

// Deallocate buffers
bool CCameraV4L2::DeallocateBuffers()
{
	if (m_isStreaming) {
		fprintf (stderr, "ERROR: trying to deallocate buffers while streaming\n");
		return false;
	}
	if (!m_buffersReady) return true; // Already deallocated

	if (m_captureMethod== CAP_READ) {
		// TODO
	}
	else if (m_captureMethod== CAP_STREAMING_MMAP) {
		UnmapBuffers();
		UnRequestBuffers(V4L2_MEMORY_MMAP);
	}
	else if (m_captureMethod== CAP_STREAMING_USR) {
		fprintf (stderr, "ERROR: DeallocateBuffers: CAP_STREAMING_USR not implemented\n");
		assert (false);
		return false;
	}
	else {
		fprintf (stderr, "ERROR: DeallocateBuffers: capture method not set\n");
		return false;
	}
	m_buffersReady= false;
	return true;
}

bool CCameraV4L2::Open ()
{
	if (m_Fd!= -1) return true;	// Already open	
	if (!DoOpen()) return false;

	TImageFormat imgformat;	
	memset(&imgformat, 0, sizeof(imgformat));
	
	// TODO: set values from constructor/parameters
	imgformat.frame_rate= 30;
	imgformat.width= 320;
	imgformat.height= 240;
	// TODO: set values from constructor/parameters
	
	if (m_usePwc)
		// TODO: Workaround for PWC cameras. It seems that is not possible
		// to open a device twice and fails when trying to enumerate formats
		imgformat.pixelformat= V4L2_PIX_FMT_YUV420;
	else
		if (!DetectBestImageFormat(imgformat)) {
			fprintf (stderr, "Unable to find any suitable image format\n");
			Close();
			return false;
		}
		
	if (!SetImageFormat(imgformat)) {
		Close();
		return false;
	}
	m_captureMethod= DetectCaptureMethod();
	if (m_captureMethod== CAP_NONE) {
		fprintf (stderr, "Unable to find a suitable capure mode\n");
		Close();
		return false;
	}	

	if (!AllocateBuffers()) {
		fprintf (stderr, "Unable to allocate buffers\n");
		Close();
		return false;
	}
	if (!EnableVideo(true)) {
		fprintf (stderr, "Unable to enable video\n");
		DeallocateBuffers();
		Close();
		return false;
	}
	return true;
}

// From opencv (otherlibs/highgui/cvcap_v4l.cpp)
/*
 * Turn a YUV4:2:0 block into an RGB block
 *
 * Video4Linux seems to use the blue, green, red channel
 * order convention-- rgb[0] is blue, rgb[1] is green, rgb[2] is red.
 *
 * Color space conversion coefficients taken from the excellent
 * http://www.inforamp.net/~poynton/ColorFAQ.html
 * In his terminology, this is a CCIR 601.1 YCbCr -> RGB.
 * Y values are given for all 4 pixels, but the U (Pb)
 * and V (Pr) are assumed constant over the 2x2 block.
 *
 * To avoid floating point arithmetic, the color conversion
 * coefficients are scaled into 16.16 fixed-point integers.
 * They were determined as follows:
 *
 *  double brightness = 1.0;  (0->black; 1->full scale)
 *  double saturation = 1.0;  (0->greyscale; 1->full color)
 *  double fixScale = brightness * 256 * 256;
 *  int rvScale = (int)(1.402 * saturation * fixScale);
 *  int guScale = (int)(-0.344136 * saturation * fixScale);
 *  int gvScale = (int)(-0.714136 * saturation * fixScale);
 *  int buScale = (int)(1.772 * saturation * fixScale);
 *  int yScale = (int)(fixScale);
 */

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))

static inline void move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v, int rowPixels, unsigned char * rgb)
{
	const int rvScale = 91881;
	const int guScale = -22553;
	const int gvScale = -46801;
	const int buScale = 116129;
	const int yScale  = 65536;
	int r, g, b;
	g = guScale * u + gvScale * v;
    //  if (force_rgb) {
    //      r = buScale * u;
    //      b = rvScale * v;
    //  } else {
	r = rvScale * v;
	b = buScale * u;
    //  }

	yTL *= yScale; yTR *= yScale;
	yBL *= yScale; yBR *= yScale;

	/* Write out top two pixels */
	rgb[0] = LIMIT(b+yTL); rgb[1] = LIMIT(g+yTL);
	rgb[2] = LIMIT(r+yTL);

	rgb[3] = LIMIT(b+yTR); rgb[4] = LIMIT(g+yTR);
	rgb[5] = LIMIT(r+yTR);

	/* Skip down to next line to write out bottom two pixels */
	rgb += 3 * rowPixels;
	rgb[0] = LIMIT(b+yBL); rgb[1] = LIMIT(g+yBL);
	rgb[2] = LIMIT(r+yBL);

	rgb[3] = LIMIT(b+yBR); rgb[4] = LIMIT(g+yBR);
	rgb[5] = LIMIT(r+yBR);
}


// From opencv (otherlibs/highgui/cvcap_v4l.cpp)
// Consider a YUV420P image of 8x2 pixels.
//
// A plane of Y values    A B C D E F G H
//                        I J K L M N O P
//
// A plane of U values    1   2   3   4
// A plane of V values    1   2   3   4 ....
//
// The U1/V1 samples correspond to the ABIJ pixels.
//     U2/V2 samples correspond to the CDKL pixels.
//
/* Converts from planar YUV420P to RGB24. */
static void yuv420p_to_rgb24(int width, int height, unsigned char *pIn0, unsigned char *pOut0)
{
	const int numpix = width * height;
	const int bytes = 24 >> 3;
	int i, j, y00, y01, y10, y11, u, v;
	unsigned char *pY = pIn0;
	unsigned char *pU = pY + numpix;
	unsigned char *pV = pU + numpix / 4;
	unsigned char *pOut = pOut0;

	for (j = 0; j <= height - 2; j += 2) {
		for (i = 0; i <= width - 2; i += 2) {
			y00 = *pY;
			y01 = *(pY + 1);
			y10 = *(pY + width);
			y11 = *(pY + width + 1);
			u = (*pU++) - 128;
			v = (*pV++) - 128;

			move_420_block(y00, y01, y10, y11, u, v, width, pOut);

			pY += 2;
			pOut += 2 * bytes;

		}
		pY += width;
		pOut += width * bytes;
	}
}

bool CCameraV4L2::DecodeToRGB (void* src, void* dst, int width, int height, uint32_t pixelformat)
{
	// TODO: check another codecs to allow direct conversions in all cases
	
	// Direct conversions
	switch (pixelformat) 
	{
		case V4L2_PIX_FMT_YUYV:
			yuyv2rgb ((BYTE*) src, (BYTE*) dst, width, height);
			return true;
		case V4L2_PIX_FMT_YUV420:
			yuv420p_to_rgb24(width, height, (BYTE*)src, (BYTE*)dst);
			return true;
		case V4L2_PIX_FMT_SGBRG8:
			bayer_to_rgb24 ((BYTE*) src, (BYTE*) dst, width, height, 0);
			return true;
		case V4L2_PIX_FMT_SGRBG8: //1
			bayer_to_rgb24 ((BYTE*) src, (BYTE*) dst, width, height, 1);
			return true;
		case V4L2_PIX_FMT_SBGGR8: //2
			bayer_to_rgb24 ((BYTE*) src, (BYTE*) dst, width, height, 2);
			return true;
		case V4L2_PIX_FMT_SRGGB8: //3
			bayer_to_rgb24 ((BYTE*) src, (BYTE*) dst, width, height, 3);
			return true;
		case V4L2_PIX_FMT_RGB24:
			memcpy(dst, src, width*height*3);
			return true;
	}
	
	// Indirect conversions through YUYV
	BYTE tmp_buffer[width*height*2];
	switch (pixelformat) 
	{
#if 0
		// TODO jpeg format
		case V4L2_PIX_FMT_JPEG:
		case V4L2_PIX_FMT_MJPEG:
			/*
			if(vd->buf.bytesused <= HEADERFRAME1) 
		{
				// Prevent crash on empty image
			g_printf("Ignoring empty buffer ...\n");
			return (ret);
	}
			memcpy(src, vd->mem[vd->buf.index],vd->buf.bytesused);
			*/

			if (jpeg_decode(&dst, src, width, height) < 0) {
				fprintf (stderr, "jpeg decode errors\n");
				return false;
			}
			break;
#endif
		case V4L2_PIX_FMT_UYVY:
			uyvy_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_YVYU:			
			yvyu_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_YYUV:
			yyuv_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_YVU420:
			yvu420_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_NV12:
			nv12_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_NV21:
			nv21_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_NV16:
			nv16_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_NV61:
			nv61_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_Y41P: 
			y41p_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_GREY:
			grey_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_SPCA501:
			s501_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_SPCA505:
			s505_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_SPCA508:
			s508_to_yuyv(tmp_buffer, (BYTE*) src, width, height);
			break;
		case V4L2_PIX_FMT_BGR24:
			bgr2yuyv((BYTE*) src, tmp_buffer, width, height);
			break;
		default:
			fprintf (stderr, "error grabbing (crvcamera_v4l2.cpp) unknown format: %i\n", pixelformat);
			return false;
	}
	yuyv2rgb ((BYTE*) tmp_buffer, (BYTE*) dst, width, height);
	
	return true;
}

IplImage *CCameraV4L2::QueryFrame()
{
	if (!m_isStreaming) return NULL;
	fd_set rdset;
	struct timeval timeout;
		
	FD_ZERO(&rdset);
	FD_SET(m_Fd, &rdset);
	timeout.tv_sec = 1; // 1 sec timeout 
	timeout.tv_usec = 0;
	
	// select - wait for data or timeout
	int retval = select(m_Fd + 1, &rdset, NULL, NULL, &timeout);
	if (retval < 0) {
		perror(" Could not grab image (select error)");
		return NULL;
	} else if (retval == 0) {
		perror(" Could not grab image (select timeout)");
		return NULL;
	}
	else if ((retval > 0) && (FD_ISSET(m_Fd, &rdset))) {
		switch (m_captureMethod) {
		case CAP_READ:
			// TODO
			return NULL;	
		case CAP_STREAMING_MMAP: {
			struct v4l2_buffer buffer;
			
			// Dequeue buffer
			memset(&buffer, 0, sizeof(struct v4l2_buffer));			
			buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buffer.memory = V4L2_MEMORY_MMAP;
			if (xioctl(m_Fd, VIDIOC_DQBUF, &buffer)!= 0) {
				perror("VIDIOC_DQBUF - Unable to dequeue buffer ");
				return NULL;
			}
			
			// Convert to destination format (always RGB 24bit)
			// TODO: check return value
			DecodeToRGB (m_captureBuffersPtr[buffer.index], (BYTE*) m_resultImage.ptr()->imageData, 
				     m_resultImage.Width(), m_resultImage.Height(), m_currentFormat.pixelformat);
			
			// Queue buffer again
			if (xioctl(m_Fd, VIDIOC_QBUF, &buffer)!= 0) {
				perror("VIDIOC_QBUF - Unable to queue buffer");			
				return NULL;
			}
			break;
		}
		case CAP_STREAMING_USR:
			fprintf (stderr, "Capture method not implemented yet\n");
			return NULL;			
		default:
			assert (false);
			return NULL;
		}		
	}
	OnQueryFrame (m_resultImage.ptr());
	
	// Flip image when needed
	if (m_resultImage.ptr()->origin == 1 && m_horizontalFlip) {
		cvFlip (m_resultImage.ptr(), NULL, -1);
		m_resultImage.ptr()->origin= 0;
	}
	else if (m_resultImage.ptr()->origin == 1 && !m_horizontalFlip) {
		cvFlip (m_resultImage.ptr(), NULL, 0);
		m_resultImage.ptr()->origin= 0;
	}
	else if (m_resultImage.ptr()->origin == 0 && m_horizontalFlip)
		cvFlip (m_resultImage.ptr(), NULL, 1);
	
	return m_resultImage.ptr();

#if 0
	if (-1 == read (m_Fd, m_buffer.start, m_buffer.length)) {
	switch (errno) {
	case EAGAIN:
	return 0;
#endif

}

#if !defined(NDEBUG)
void CCameraV4L2::Dump()
{
	std::cout << "CCameraV4L2::Dump(). Begin\n";
	
	std::cout << "m_deviceShortName:" << m_deviceShortName << std::endl;
	std::cout << "m_Fd:" << m_Fd << std::endl;
	std::cout << "m_libWebcamHandle:" << m_libWebcamHandle << std::endl;
	std::cout << "m_captureMethod:" << m_captureMethod << std::endl;
	std::cout << "m_usePwc:" << m_usePwc << std::endl;
	std::cout << "m_isStreaming:" << m_isStreaming << std::endl;
	std::cout << "m_buffersReady:" << m_buffersReady << std::endl;
	
	std::cout << "m_currentFormat.frame_rate:" <<  m_currentFormat.frame_rate << std::endl;
	std::cout << "m_currentFormat.width" << m_currentFormat.width << std::endl;
	std::cout << "m_currentFormat.height:" << m_currentFormat.height << std::endl;
	std::cout << "m_currentFormat.pielformat:" <<  m_currentFormat.pixelformat << std::endl;	
	
	for (unsigned int i= 0; i< m_supportedPixelFormats.size(); ++i)
		std::cout << "m_supportedPixelFormats[" << i << "]:" << m_supportedPixelFormats[i] << std::endl;
	for (unsigned int i= 0; i< m_cameraControls.size(); ++i) {
		std::cout << "m_cameraControls[" << i << "]:\n" << std::endl;
		 m_cameraControls[i].Dump();
	}
	std::cout << "CCameraV4L2::Dump(). End\n";
}
#endif
///////////////////////////////////////////////////////////////////////////////
//
// Implementacion of CCameraControlV4l2 class
//

CCameraControlV4l2::CCameraControlV4l2 (CHandle handle, const CControl& control) :
	m_name(control.name)
{
	m_handle= handle;	
	m_id= control.id;
	m_type= control.type;
	m_default= control.value.value;
		
	if (CControlType2ECameraControlType (control.type)!= CCTYPE_CHOICE) {
		m_min= control.min.value;
		m_max= control.max.value;
		// TODO: control.step is really necessary?
	}
	else {
		m_min= 0;
		m_max= control.choices.count-1;
		for (int i= m_min; i<= m_max; ++i) m_choices.push_back(control.choices.list[i].name);		
	}	
}

// Get/set the current value. For boolean controls 0 and 1 are the only
// possible values. For choice controls 0 represents the first option.
// Set method returns true on success, false otherwise
int CCameraControlV4l2::GetValue() const
{
	CControlValue value;
	value.type= m_type;
	value.value= 0;
	
	if (c_get_control (m_handle, m_id, &value)!= C_SUCCESS) {
		fprintf (stderr, "CCameraControlV4l2::GetValue() failed to query value\n");
		return 0;
	}
	
	return value.value;
}

bool CCameraControlV4l2::SetValue(int value)
{
	CControlValue cvalue;
	cvalue.type= m_type;
	cvalue.value= value;
	
	if (c_set_control (m_handle, m_id, &cvalue)!= C_SUCCESS) {
		fprintf (stderr, "CCameraControlV4l2::GetValue() failed to query value\n");
		return false;
	}
	return false;
}

const char* CCameraControlV4l2::GetChoiceName(unsigned int numOption) const
{
	if (numOption> (unsigned int) m_max) return NULL;
	
	return  m_choices[numOption].c_str();
}

bool CCameraControlV4l2::CheckSupportedLibwebcamId (CControlId id)
{
	return (LibwebcamId2ECameraControlId(id)!= CAM_ERROR_ENTRY);	
}

CCameraControl::ECameraControlId CCameraControlV4l2::LibwebcamId2ECameraControlId (CControlId id)
{
	switch(id) {
		case CC_BRIGHTNESS: return CAM_BRIGHTNESS;
		case CC_CONTRAST: return CAM_CONTRAST;
		case CC_GAIN: return CAM_GAIN;
		case CC_SATURATION: return CAM_SATURATION;
		case CC_HUE: return CAM_HUE;
		case CC_GAMMA: return CAM_GAMMA;
		case CC_SHARPNESS: return CAM_SHARPNESS;
		case CC_WHITE_BALANCE_TEMPERATURE: return CAM_WHITE_BALANCE_TEMPERATURE;
		case CC_AUTO_WHITE_BALANCE_TEMPERATURE: return CAM_AUTO_WHITE_BALANCE_TEMPERATURE;
		case CC_WHITE_BALANCE_COMPONENT: return CAM_WHITE_BALANCE_COMPONENT;
		case CC_AUTO_WHITE_BALANCE_COMPONENT: return CAM_AUTO_WHITE_BALANCE_COMPONENT;
		case CC_BACKLIGHT_COMPENSATION: return CAM_BACKLIGHT_COMPENSATION;
		case CC_POWER_LINE_FREQUENCY: return CAM_POWER_LINE_FREQUENCY;
		case CC_AUTO_HUE: return CAM_AUTO_HUE;
		case CC_AUTO_EXPOSURE_MODE: return CAM_AUTO_EXPOSURE_MODE;
		case CC_AUTO_EXPOSURE_PRIORITY: return CAM_AUTO_EXPOSURE_PRIORITY;
		case CC_EXPOSURE_TIME_ABSOLUTE: return CAM_EXPOSURE_TIME_ABSOLUTE;
		case CC_EXPOSURE_TIME_RELATIVE: return CAM_EXPOSURE_TIME_RELATIVE;
		case CC_AUTO_FOCUS: return CAM_AUTO_FOCUS;
		case CC_FOCUS_ABSOLUTE: return CAM_FOCUS_ABSOLUTE;
		case CC_FOCUS_RELATIVE: return CAM_FOCUS_RELATIVE;
		case CC_IRIS_ABSOLUTE: return CAM_IRIS_ABSOLUTE;
		case CC_IRIS_RELATIVE: return CAM_IRIS_RELATIVE;
		case CC_ZOOM_ABSOLUTE: return CAM_ZOOM_ABSOLUTE;
		case CC_ZOOM_RELATIVE: return CAM_ZOOM_RELATIVE;
		case CC_DIGITAL_ZOOM: return CAM_DIGITAL_ZOOM;
		case CC_PAN_ABSOLUTE: return CAM_PAN_ABSOLUTE;
		case CC_PAN_RELATIVE: return CAM_PAN_RELATIVE;
		case CC_TILT_ABSOLUTE: return CAM_TILT_ABSOLUTE;
		case CC_TILT_RELATIVE: return CAM_TILT_RELATIVE;
		case CC_ROLL_ABSOLUTE: return CAM_ROLL_ABSOLUTE;
		case CC_ROLL_RELATIVE: return CAM_ROLL_RELATIVE;
		case CC_PRIVACY: return CAM_PRIVACY;
		case CC_PAN_RESET: return CAM_PAN_RESET;
		case CC_TILT_RESET: return CAM_TILT_RESET;
		case CC_LOGITECH_PANTILT_RELATIVE: return CAM_LOGITECH_PANTILT_RELATIVE;
		case CC_LOGITECH_PANTILT_RESET: return CAM_LOGITECH_PANTILT_RESET;
		case CC_LOGITECH_LED1_MODE: return CAM_LOGITECH_LED1_MODE;
		case CC_LOGITECH_LED1_FREQUENCY: return CAM_LOGITECH_LED1_FREQUENCY;
		case CC_LOGITECH_DISABLE_PROCESSING: return CAM_LOGITECH_DISABLE_PROCESSING;
		case CC_LOGITECH_RAW_BITS_PER_PIXEL: return CAM_LOGITECH_RAW_BITS_PER_PIXEL;
		default: return CAM_ERROR_ENTRY;
	}
}

CCameraControl::ECameraControlType CCameraControlV4l2::CControlType2ECameraControlType (CControlType type)
{
	ECameraControlType ownType= CCTYPE_NUMBER;
	switch (type) {
		case CC_TYPE_BOOLEAN:
			ownType= CCTYPE_BOOLEAN;
			break;
		case CC_TYPE_BYTE:
		case CC_TYPE_WORD:
		case CC_TYPE_DWORD:
			ownType= CCTYPE_NUMBER;
			break;
		case CC_TYPE_CHOICE:
			ownType= CCTYPE_CHOICE;
			break;
		default:
			// Unsupported control. Execution should never reach this point
			assert (false);
	}	
	return ownType;
}

#if !defined(NDEBUG)
void CCameraControlV4l2::Dump()
{
	std::cout << " CCameraControlV4l2::Dump(). Begin\n";
	std::cout << "m_handle:" << m_handle << std::endl;
	std::cout << "m_id:" << m_id << std::endl;
	std::cout << "m_id (converted):" << LibwebcamId2ECameraControlId (m_id) << std::endl;
	std::cout << "m_name:" << m_name << std::endl;
	std::cout << "m_type:" << m_type << std::endl;
	std::cout << "m_type (converted):" << CControlType2ECameraControlType(m_type) << std::endl;
	std::cout << "m_default:" << m_default << std::endl;
	std::cout << "m_min:" << m_min << std::endl;
	std::cout << "m_max:" << m_max << std::endl;
	for (unsigned int i= 0; i< m_choices.size(); ++i)
		std::cout << "m_choices[" << i << "]:" << m_choices[i] << std::endl;
	std::cout << "Value:" << GetValue() << std::endl;	
	std::cout << " CCameraControlV4l2::Dump(). End\n";
}
#endif