/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_v4l2.cpp
// Purpose:	Provide a camera capture class around v4l2 and libwebcam
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     17/05/2010
// Copyright:   (C) 2008-11 Cesar Mauri Loba - CREA Software Systems
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
//#include <linux/videodev.h>
#include "incvideodev.h"
#include <libv4l2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <list>
#include "pwc-ioctl.h"
#include "colorspaces.h"
#if !defined(NDEBUG)
#include <iostream>
#endif

// TODO: thread safety


// Define possibly missing entries from videodev2.h
#ifndef V4L2_PIX_FMT_MJPEG
#define V4L2_PIX_FMT_MJPEG  v4l2_fourcc('M', 'J', 'P', 'G') /*  MJPEG stream     */
#endif

#ifndef V4L2_PIX_FMT_JPEG
#define V4L2_PIX_FMT_JPEG  v4l2_fourcc('J', 'P', 'E', 'G')  /*  JPEG stream      */
#endif

#ifndef V4L2_PIX_FMT_YUYV
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y','U','Y','V')   /* YUV 4:2:2        */
#endif

#ifndef V4L2_PIX_FMT_YVYU
#define V4L2_PIX_FMT_YVYU    v4l2_fourcc('Y','V','Y','U')   /* YUV 4:2:2        */
#endif

#ifndef V4L2_PIX_FMT_UYVY
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U','Y','V','Y')   /* YUV 4:2:2        */
#endif

#ifndef V4L2_PIX_FMT_YYUV
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y','Y','U','V')   /* YUV 4:2:2        */
#endif

#ifndef V4L2_PIX_FMT_YUV420
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y','U','1','2')   /* YUV 4:2:0 Planar  */
#endif

#ifndef V4L2_PIX_FMT_YVU420
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y','V','1','2')   /* YUV 4:2:0 Planar  */
#endif

#ifndef V4L2_PIX_FMT_NV12
#define V4L2_PIX_FMT_NV12  v4l2_fourcc('N','V','1','2')   /* YUV 4:2:0 Planar (u/v) interleaved */
#endif

#ifndef V4L2_PIX_FMT_NV21
#define V4L2_PIX_FMT_NV21  v4l2_fourcc('N','V','2','1')   /* YUV 4:2:0 Planar (v/u) interleaved */
#endif

#ifndef V4L2_PIX_FMT_NV16
#define V4L2_PIX_FMT_NV16  v4l2_fourcc('N','V','1','6')   /* YUV 4:2:2 Planar (u/v) interleaved */
#endif

#ifndef V4L2_PIX_FMT_NV61
#define V4L2_PIX_FMT_NV61  v4l2_fourcc('N','V','6','1')   /* YUV 4:2:2 Planar (v/u) interleaved */
#endif

#ifndef V4L2_PIX_FMT_Y41P
#define V4L2_PIX_FMT_Y41P  v4l2_fourcc('Y','4','1','P')    /* YUV 4:1:1          */
#endif

#ifndef V4L2_PIX_FMT_GREY
#define V4L2_PIX_FMT_GREY  v4l2_fourcc('G','R','E','Y')    /*      Y only       */
#endif

#ifndef V4L2_PIX_FMT_SPCA501
#define V4L2_PIX_FMT_SPCA501 v4l2_fourcc('S','5','0','1')  /* YUYV - by line     */
#endif

#ifndef V4L2_PIX_FMT_SPCA505
#define V4L2_PIX_FMT_SPCA505 v4l2_fourcc('S','5','0','5')  /* YYUV - by line     */
#endif

#ifndef V4L2_PIX_FMT_SPCA508
#define V4L2_PIX_FMT_SPCA508 v4l2_fourcc('S','5','0','8')  /* YUVY - by line     */
#endif

#ifndef V4L2_PIX_FMT_SGBRG8
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /* GBGB.. RGRG..    */
#endif

#ifndef V4L2_PIX_FMT_SGRBG8
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /* GRGR.. BGBG..    */
#endif

#ifndef V4L2_PIX_FMT_SBGGR8
#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /* BGBG.. GRGR..    */
#endif

#ifndef V4L2_PIX_FMT_SRGGB8
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /* RGRG.. GBGB..    */
#endif

#ifndef V4L2_PIX_FMT_BGR24
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8    */
#endif

#ifndef V4L2_PIX_FMT_RGB24
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8    */
#endif

CCameraV4L2::CCameraV4L2(int cameraId, unsigned int width, unsigned int height, 
		float fr) throw(camera_exception)
{
	InstanceCreated();
	if (cameraId>= GetNumDevices()) {
		InstanceDestroyed();
		throw camera_exception("wrong camera id");
	}
	m_Id= cameraId;
	
	m_desiredFormat.frame_rate= (unsigned int) fr;
	m_desiredFormat.width= width;
	m_desiredFormat.height= height;
	m_desiredFormat.pixelformat= 0;

	m_libWebcamHandle= 0;
	m_captureMethod= CAP_NONE;
	m_isStreaming= false;
	m_buffersReady= false;
	memset(&m_captureBuffersInfo, 0, sizeof(struct v4l2_buffer) * STREAMING_CAPTURE_NBUFFERS);
	memset(&m_captureBuffersPtr, 0, sizeof(void*) * STREAMING_CAPTURE_NBUFFERS);	
	memset(&m_currentFormat, 0, sizeof(m_currentFormat));
	AddSupportedPixelFormats ();
}

CCameraV4L2::~CCameraV4L2(void)
{
    Close ();
	InstanceDestroyed();
}

bool CCameraV4L2::DoOpen ()
{
#if !defined(NDEBUG)
	fprintf (stderr, "CCameraV4L2::DoOpen: begin\n");
#endif
	if (m_libWebcamHandle!= 0) {
#if !defined(NDEBUG)
		fprintf (stderr, "CCameraV4L2::DoOpen: already open\n");
#endif
		return true;	// Already open	
	}
	if (!InternalOpen()) {
#if !defined(NDEBUG)
		fprintf (stderr, "CCameraV4L2::DoOpen: open failed\n");
#endif
		return false;
	}
	
	assert (m_desiredFormat.width && m_desiredFormat.height && m_desiredFormat.frame_rate);
	m_currentFormat= m_desiredFormat;
		
	if (!DetectBestImageFormat()) {
		fprintf (stderr, "Unable to find any suitable image format\n");
		Close();
		return false;
	}
	if (!SetImageFormat()) {
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
	
	// TODO: Awful. This is a provisional solution to avoid broken frames while capturing.
	// It seems as if the driver/camera needs some time before start grabbing.
	usleep (2000000);
	return true;
}

void CCameraV4L2::DoClose ()
{
	if (m_isStreaming) EnableVideo(false);	
	if (m_buffersReady) DeallocateBuffers();
	if (m_libWebcamHandle!= 0) {
		c_close_device (m_libWebcamHandle);
		m_libWebcamHandle= 0;
	}
	m_captureMethod= CAP_NONE;
	m_cameraControls.clear();
}

bool CCameraV4L2::InternalOpen()
{
	char devName[CAM_DEVICE_SHORT_NAME_LENGHT+5];
	struct stat st;	

	// Create device name
	snprintf (devName, CAM_DEVICE_SHORT_NAME_LENGHT+5, "/dev/%s", g_deviceShortNames[m_Id]); //m_deviceShortName);

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
	m_libWebcamHandle= c_open_device (g_deviceShortNames[m_Id]); //m_deviceShortName);
	if (m_libWebcamHandle== 0 || c_get_file_descriptor (m_libWebcamHandle)<= 0) {
		fprintf (stderr, "ERROR: Cannot open ’%s’ via libwebcam\n", devName);
		Close();
		return false;
	}
	
	PopulateCameraControls ();
	 
	return true;
}

/* ioctl with a number of retries in the case of failure
* args:
* fd - device descriptor
* IOCTL_X - ioctl reference
* arg - pointer to ioctl data
* returns - ioctl result
* Based on xioctl from guvcview
*/
// TODO: this is code borrowed from Paulo's guvcview, but here
// it seems useless because we don't open the device in non-blocking mode
// Considered either removing or opening device in non-blocking mode
// See sleep TODO below

#define IOCTL_RETRY 4
static
int xioctl(int fd, int IOCTL_X, void *arg)
{
	int ret = 0;
	int tries= IOCTL_RETRY;
	do {
		ret = v4l2_ioctl(fd, IOCTL_X, arg);
	} 
	while (ret && tries-- && ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));

	if (ret && (tries <= 0)) fprintf(stderr, "ioctl (%i) retried %i times - giving up: %s)\n", IOCTL_X, IOCTL_RETRY, strerror(errno));
	
	return (ret);
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
				m_cameraControls.push_back(CCameraControlV4L2(m_libWebcamHandle, controls[i]));
		}
	}
	return true;
}

unsigned int CCameraV4L2::GetCameraControlsCount()
{
	return static_cast<unsigned int>(m_cameraControls.size());
}

CCameraControl* CCameraV4L2::GetCameraControl(unsigned int numControl)
{
	if (numControl>= GetCameraControlsCount()) return NULL;
	return &m_cameraControls[numControl];
}

// Do VIDIOC_REQBUFS	
bool CCameraV4L2::RequestBuffers(enum v4l2_memory mem)
{
	struct v4l2_requestbuffers requestbuffers;

	memset (&requestbuffers, 0, sizeof(requestbuffers));
	requestbuffers.count= STREAMING_CAPTURE_NBUFFERS;
	requestbuffers.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	requestbuffers.memory= mem; //V4L2_MEMORY_MMAP;
	if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_REQBUFS, &requestbuffers)== 0) {
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
	if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_REQBUFS, &requestbuffers)== 0) return true;
	fprintf (stderr, "ERROR: UnRequestBuffers: failed\n");
	return false;
}
	
// This method must be called AFTER desired format is set	
CCameraV4L2::ECaptureMethod CCameraV4L2::DetectCaptureMethod()
{
	struct v4l2_capability capability;	

	// Query basic capabilities. This never should fail
	if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_QUERYCAP, &capability)!= 0) {
		fprintf (stderr, "WARNING: Cannot query camera capabilities: VIDIOC_QUERYCAP ioctl failed\n");
		return CAP_NONE;
	}

	if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
	    fprintf (stderr, "WARNING: is no video capture device\n");
	    return CAP_NONE;
	}

	// Driver supports streaming
	if ((capability.capabilities & V4L2_CAP_STREAMING)) {
		// Streaming supported. Which kind?
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

bool CCameraV4L2::DetectBestImageFormat()
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
#if !defined(NDEBUG)
			char* tmp= (char *) &i->pixelformat;
			std::cerr << "crvcamera_v4l2: discarding unsuported format: " << tmp[0] << tmp[1] << tmp[2] << tmp[3] << std::endl;
#endif
			i= availableFormats.erase (i);
		}
		else ++i;
	}

	// No available formats detected
	if (availableFormats.size()== 0) return false;

	// As for realtime computer vision frame rate is usually a critical parameter we first choose it.
	SelectBestFrameRate (m_currentFormat.frame_rate, availableFormats);
	
	// Sizes. Chooses closest number of pixel (width*height) to the requested
	SelectBestFramePixelNumber (m_currentFormat.width * m_currentFormat.height, availableFormats);
	
	// Check aspect ratio
	//TODO: Check weird errors. floating point errors.	
	if (m_currentFormat.width> 0 && m_currentFormat.height> 0) {
		float bestdiff= FLT_MAX;
		float aratio= (float) m_currentFormat.width / (float) m_currentFormat.height;
		// Find closest frame ratio
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			float diff= abs_distance_to_range<float> ((float)i->min_width / (float)i->max_height, (float)i->max_width / (float)i->min_height, aratio);
			if (diff< bestdiff) bestdiff= diff;
		}
		// Remove worst entries
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end();)
		{
			if (abs_distance_to_range<float> ((float)i->min_width / (float)i->max_height, (float)i->max_width / (float)i->min_height, aratio)> bestdiff) 
				i= availableFormats.erase(i);
			else
				++i;
		}
		assert (availableFormats.size()> 0);
	}

	
	// If frame rate not explicity specified then selects highest fr available
	if (m_currentFormat.frame_rate== 0) {
		m_currentFormat.frame_rate= UINT_MAX;
		SelectBestFrameRate (m_currentFormat.frame_rate, availableFormats);
	}
	
	// If frame size not explicity specified then selects bigger frame size available
	if (m_currentFormat.width== 0 || m_currentFormat.height== 0) {
		if (!m_currentFormat.width) m_currentFormat.width= UINT_MAX;
		if (!m_currentFormat.height) m_currentFormat.height= UINT_MAX;		
		SelectBestFramePixelNumber (UINT_MAX, availableFormats);
	}
	
	// Finally chooses best available pixelformat
	for (unsigned int ienc= 0; ienc< m_supportedPixelFormats.size(); ++ienc) {		
		for (std::list<TImageFormatEx>::iterator i= availableFormats.begin(); i!= availableFormats.end(); ++i) {
			if (m_supportedPixelFormats[ienc]== i->pixelformat) {
				// Bingo! Store data and finish
				m_currentFormat.pixelformat= m_supportedPixelFormats[ienc];
				m_currentFormat.frame_rate= range_value_fit (i->min_frame_rate, i->max_frame_rate, i->step_frame_rate, m_currentFormat.frame_rate);
				m_currentFormat.width= range_value_fit (i->min_width, i->max_width, i->step_width, m_currentFormat.width);
				m_currentFormat.height= range_value_fit (i->min_height, i->max_height, i->step_height, m_currentFormat.height);
			#if !defined(NDEBUG)
				std::cout << "Choosen format:";
				dump_TImageFormatEx (*i);
			#endif
				return true;
			}
		}
	}
	
	// Execution should never reach this point
	assert (false);
	return false;
}

bool CCameraV4L2::SetImageFormat()
{
	struct v4l2_format format;
	
	//
	// Set frame format, including width, height and pixelformat. First query current
	// settings, then modify relevant values and finally query actual values
	//
	memset (&format, 0, sizeof (format));
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_G_FMT, &format)== -1) {
		fprintf(stderr, "ERROR: Unable to get format.\n");
		return false;	
	}
	
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width = m_currentFormat.width;
	format.fmt.pix.height = m_currentFormat.height;
	format.fmt.pix.pixelformat = m_currentFormat.pixelformat;
	format.fmt.pix.field = V4L2_FIELD_ANY;

	if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_FMT, &format)== -1) {
		fprintf(stderr, "ERROR: Unable to set format.\n");
		return false;	
	}
	
	// Store currently set format (last VIDIOC_S_FMT may have changed it)
	//m_currentFormat.frame_rate= imgformat.frame_rate;
	m_currentFormat.width= format.fmt.pix.width;
	m_currentFormat.height= format.fmt.pix.height;
	m_currentFormat.pixelformat= format.fmt.pix.pixelformat;	

	//	
	// Set framerate and other camera parameters.
	// 
	// We here try to cope with two common drivers: pwc and uvc. Unfortunately pwc driver versions above
	// 10.0.12 removed support for many ioctls (including that to set the framerate VIDIOCSWIN).
	// As of kernel version 3.0.0 (which ships pwc 10.0.14) this issue has not been fixed and the odds are
	// it won't be as the hardware has been discontinued :-(
	//
	if (strcasestr(g_deviceDriverNames[m_Id], "pwc")!= NULL)  {
		// Using a PWC based camera.
		bool properlySet= false;
		struct video_window vwin;		
		if ((xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOCGWIN, &vwin) == 0) && (vwin.flags & PWC_FPS_FRMASK)) {
			vwin.flags &= ~PWC_FPS_FRMASK;
			vwin.flags |= (m_currentFormat.frame_rate << PWC_FPS_SHIFT);
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOCSWIN, &vwin) == 0) properlySet= true;
		}

		if (!properlySet) fprintf (stderr, "Warning: cannot set FPS: %d for PWC camera\n", m_currentFormat.frame_rate);
	}
	else {
		struct v4l2_streamparm parm;

		// An UVC camera is assumed
		memset(&parm, 0, sizeof (parm));
		parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
		// Firstly try to get current v4l2_streamparm parameters
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_G_PARM, &parm)== 0) {
			parm.parm.capture.timeperframe.numerator = 1;
			parm.parm.capture.timeperframe.denominator = m_currentFormat.frame_rate;
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_PARM, &parm)!= 0) {
				// Show warning and continue
				fprintf (stderr, "Warning: cannot set FPS: %d for UVC camera\n", m_currentFormat.frame_rate);
			}
			
			// Read values again and store actual values
			if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_G_PARM, &parm)== 0) {
				// Set failed, store read values
				m_currentFormat.frame_rate= 0;
				if (parm.parm.capture.timeperframe.denominator)
					m_currentFormat.frame_rate= parm.parm.capture.timeperframe.denominator / parm.parm.capture.timeperframe.numerator;
			}
			else
				fprintf (stderr, "Warning: cannot read again VIDIOC_G_PARM\n");
		}
		else {
			fprintf (stderr, "Error: VIDIOC_G_PARM for UVC camera\n");
			return false;
		}
	
		//
		// Control V4L2_CID_EXPOSURE_AUTO is key to get a constant capture rate & good
		// lightning conditions. In order of preference, the value of this control should be:
		//
		//   - V4L2_EXPOSURE_APERTURE_PRIORITY: Auto exposure time, manual iris. In practice, 
		//     when (V4L2_CID_EXPOSURE_AUTO_PRIORITY== false) and the frame rate is previously 
		//     set via VIDIOC_S_PARM, provides a constant frame rate at top speed and changes 
		//     exposure time and gain automatically according to the lightning conditions.
		//     Tested with the Logitech Webcam 9000 (046d:0990). It seems that uvcvideo drivers 
		//     for newer kernels (3.0 and above) set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_MANUAL 
		//     by default instead of V4L2_EXPOSURE_SHUTTER_PRIORITY (at least for this camera model)
		//     which came as default option for older kernels.
		//
		//   - V4L2_EXPOSURE_SHUTTER_PRIORITY: ??? (not tested)
		//
		//   - V4L2_EXPOSURE_MANUAL: Manual exposure time, manual iris. Provides maximum flexibility
		//     and allows for constant frame but i) does not cope well variable lightnint conditions
		//     (forces user to manually adjust settings when needed) ii) it seems that default gain 
		//     and exposure values are undefined and thus they need to be set to reasonable 
		//     defaults and also suggest that camera controls should be permanently stored.
		//
		//   - V4L2_EXPOSURE_AUTO: Automatic exposure time, automatic iris aperture. Automatically 
		//     reduces the frame rate depending on the lightning conditions and thus should be avoided.
		//
		// TODO: now uses the V4L2 api directly instead of libwebcam as some controls are not available
		// any more.
		// TODO: store control settings permanently
		//
		
	
		struct v4l2_control control;
		memset (&control, 0, sizeof (control));

		/*
		control.id= V4L2_CID_EXPOSURE_AUTO;
		control.value= -1;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_G_CTRL, &control))
			fprintf (stderr, "Warning: cannot get V4L2_CID_EXPOSURE_AUTO\n");
		else {
			fprintf (stderr, "Info: V4L2_CID_EXPOSURE_AUTO= %d\n", control.value);
		}
		

		for (int i= 0; i<= 16; ++i) {
			control.id= V4L2_CID_EXPOSURE_AUTO;
			control.value= i;
			if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
				fprintf (stderr, "Warning: cannot set V4L2_CID_EXPOSURE_AUTO to %d\n", i);
			else
				fprintf (stderr, "Info: set V4L2_CID_EXPOSURE_AUTO to %d\n", i);
		}
		*/

		//
		// First set manual mode with reasonable exposure and gain values.
		//
		control.id= V4L2_CID_EXPOSURE_AUTO;
		control.value= V4L2_EXPOSURE_MANUAL;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
			fprintf (stderr, "Warning: cannot set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_MANUAL\n");
		else
			fprintf (stderr, "Info: set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_MANUAL\n");
			
		// Exposure time to match requested FPS
		control.id= V4L2_CID_EXPOSURE_ABSOLUTE;
		control.value= 10000 / m_currentFormat.frame_rate;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
			fprintf (stderr, "Warning: cannot set V4L2_CID_EXPOSURE_ABSOLUTE to %d\n", control.value);
		else
			fprintf (stderr, "Info: set V4L2_CID_EXPOSURE_ABSOLUTE to %d\n", control.value);
		
		// Sets gain to the maximum value
		for (unsigned int i= 0; i< m_cameraControls.size(); ++i) {
			if (m_cameraControls[i].GetId()== CCameraControl::CAM_GAIN) {
				if (m_cameraControls[i].SetValue(m_cameraControls[i].GetMaximumValue()))
					fprintf (stderr, "Info: set CAM_GAIN to %d\n", 
						m_cameraControls[i].GetMaximumValue());
				else
					fprintf (stderr, "Warning: cannot set CAM_GAIN to %d\n", 
						m_cameraControls[i].GetMaximumValue());
				break;
			}
		}
			
		// Tries to set automatic gain
		control.id= V4L2_CID_AUTOGAIN;
		control.value= 1;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
			fprintf (stderr, "Warning: cannot set V4L2_CID_AUTOGAIN\n");
		else
			fprintf (stderr, "Info: set V4L2_CID_AUTOGAIN\n");

		// Set V4L2_CID_EXPOSURE_AUTO_PRIORITY to false
		bool hasExposureAutoPriority= false;
		for (unsigned int i= 0; i< m_cameraControls.size(); ++i) {
			if (m_cameraControls[i].GetId()== CCameraControl::CAM_AUTO_EXPOSURE_PRIORITY) {
				if (m_cameraControls[i].SetValue(0)) {
					hasExposureAutoPriority= true;
					fprintf (stderr, "Info: AUTO_EXPOSURE_PRIORITY disabled\n");
				}
				else
					fprintf (stderr, "Warning: cannot disable AUTO_EXPOSURE_PRIORITY\n");
				break;
			}
		}

		// If EXPOSURE_AUTO_PRIORITY cannot be disabled does not attempt remaing settings 
		if (!hasExposureAutoPriority) return true;	
	
		//
		// Secondly tries to set V4L2_EXPOSURE_SHUTTER_PRIORITY mode
		//
		control.id= V4L2_CID_EXPOSURE_AUTO;
		control.value= V4L2_EXPOSURE_SHUTTER_PRIORITY;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
			fprintf (stderr, "Warning: cannot set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_SHUTTER_PRIORITY\n");
		else
			fprintf (stderr, "Info: set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_SHUTTER_PRIORITY\n");


		// Finally tries to set V4L2_EXPOSURE_APERTURE_PRIORITY
		control.id= V4L2_CID_EXPOSURE_AUTO;
		control.value= V4L2_EXPOSURE_APERTURE_PRIORITY;
		if (xioctl (c_get_file_descriptor (m_libWebcamHandle), VIDIOC_S_CTRL, &control))
			fprintf (stderr, "Warning: cannot set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_APERTURE_PRIORITY\n");
		else
			fprintf (stderr, "Info: set V4L2_CID_EXPOSURE_AUTO to V4L2_EXPOSURE_APERTURE_PRIORITY\n");
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
	if (m_libWebcamHandle== 0) return false;	// Camera not open, fail
	if (enable== m_isStreaming) return true;
	
	switch(m_captureMethod)	{		
	case CAP_READ:
		// capture using read, nothing to enable
		break;
	case CAP_STREAMING_MMAP: 
	    {
		int type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		int action= (enable? VIDIOC_STREAMON : VIDIOC_STREAMOFF);
		if (xioctl(c_get_file_descriptor (m_libWebcamHandle), action, &type)!= 0) {
			perror("VIDIOC_STREAMON - Unable to start capture");
			return false;
		}
		break;
	    }	
	case CAP_STREAMING_USR:
		// Not implemented
		fprintf (stderr, "CAP_STREAMING_USR: feature not implemented\n");
		assert (false);
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
	if (m_libWebcamHandle== 0) {
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
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_QUERYBUF, &m_captureBuffersInfo[i])!= 0) {
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
			m_captureBuffersPtr[i]= mmap(NULL, m_captureBuffersInfo[i].length, PROT_READ | PROT_WRITE, MAP_SHARED, c_get_file_descriptor (m_libWebcamHandle), m_captureBuffersInfo[i].m.offset);
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
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_QBUF, &m_captureBuffersInfo[i])!= 0) {
				perror("VIDIOC_QBUF - Unable to queue buffer");
				UnmapBuffers();
				UnRequestBuffers(V4L2_MEMORY_MMAP);
				return false;
			}
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
#define LIMIT(x) ((unsigned char)((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16))))

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

IplImage *CCameraV4L2::DoQueryFrame()
{
	if (!DoQueryFrame(m_resultImage)) return NULL;
	
	return m_resultImage.ptr();
}

bool CCameraV4L2::DoQueryFrame(CIplImage& image)
{
	if (!m_isStreaming) return false;
	fd_set rdset;
	struct timeval timeout;
		
	FD_ZERO(&rdset);
	FD_SET(c_get_file_descriptor (m_libWebcamHandle), &rdset);
	timeout.tv_sec = 1; // 1 sec timeout 
	timeout.tv_usec = 0;
	
	// select - wait for data or timeout
	int retval = select(c_get_file_descriptor (m_libWebcamHandle) + 1, &rdset, NULL, NULL, &timeout);
	if (retval < 0) {
		perror(" Could not grab image (select error)");
		return false;
	} else if (retval == 0) {
		perror(" Could not grab image (select timeout)");
		return false;
	}
	else if ((retval > 0) && (FD_ISSET(c_get_file_descriptor (m_libWebcamHandle), &rdset))) {
		switch (m_captureMethod) {
		case CAP_READ:
			fprintf (stderr, "CAP_READ Capture method not implemented yet\n");
			assert (false);
			return false;	
		case CAP_STREAMING_MMAP: {
			struct v4l2_buffer buffer;
			
			// Dequeue buffer
			memset(&buffer, 0, sizeof(struct v4l2_buffer));			
			buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buffer.memory = V4L2_MEMORY_MMAP;
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_DQBUF, &buffer)!= 0) {
				perror("VIDIOC_DQBUF - Unable to dequeue buffer ");
				return false;
			}
			
			// Allocate result image when necessary
			bool allocFailed= false;
			if (!image.Initialized() || 
				m_currentFormat.width!= static_cast<unsigned int>(image.Width()) || 
				m_currentFormat.height!= static_cast<unsigned int>(image.Height())) {
				
				// TODO: correct the V4L2_PIX_FMT_YUV420 conversion routine
				const char* planeOrder;
				if (m_currentFormat.pixelformat== V4L2_PIX_FMT_YUV420) planeOrder= "BGR";
				else planeOrder= "RGB";
				// TODO: make sure that image is not initialized with padded rows
				if (!image.Create (m_currentFormat.width, m_currentFormat.height, IPL_DEPTH_8U, planeOrder, IPL_ORIGIN_TL, IPL_ALIGN_DWORD )) {
					fprintf (stderr, "Cannot create result image\n");
					allocFailed= true;					
				}
			}
			// Convert to destination format (always RGB 24bit)
			// TODO: check return value
			if (!allocFailed) 
				DecodeToRGB (m_captureBuffersPtr[buffer.index], (BYTE*) image.ptr()->imageData, 
				     image.Width(), image.Height(), m_currentFormat.pixelformat);
			
			// Queue buffer again
			if (xioctl(c_get_file_descriptor (m_libWebcamHandle), VIDIOC_QBUF, &buffer)!= 0) {
				perror("VIDIOC_QBUF - Unable to queue buffer");			
				return false;
			}
			return (!allocFailed);
		}
		case CAP_STREAMING_USR:
			fprintf (stderr, "CAP_STREAMING_USR Capture method not implemented yet\n");
			assert (false);
			return false;			
		default:
			assert (false);
			return false;
		}		
	}
		
	return true;

#if 0
	if (-1 == read (c_get_file_descriptor (m_libWebcamHandle), m_buffer.start, m_buffer.length)) {
	switch (errno) {
	case EAGAIN:
	return 0;
#endif
}

#if !defined(NDEBUG)
void CCameraV4L2::Dump()
{
	std::cout << "CCameraV4L2::Dump(). Begin\n";
	
	std::cout << "m_libWebcamHandle:" << m_libWebcamHandle << std::endl;
	std::cout << "m_captureMethod:" << m_captureMethod << std::endl;
	std::cout << "m_isStreaming:" << m_isStreaming << std::endl;
	std::cout << "m_buffersReady:" << m_buffersReady << std::endl;
	
	std::cout << "m_currentFormat.frame_rate:" <<  m_currentFormat.frame_rate << std::endl;
	std::cout << "m_currentFormat.width" << m_currentFormat.width << std::endl;
	std::cout << "m_currentFormat.height:" << m_currentFormat.height << std::endl;
	std::cout << "m_currentFormat.pielformat:" <<  m_currentFormat.pixelformat << std::endl;	
/*	
	for (unsigned int i= 0; i< m_supportedPixelFormats.size(); ++i)
		std::cout << "m_supportedPixelFormats[" << i << "]:" << m_supportedPixelFormats[i] << std::endl;
	for (unsigned int i= 0; i< m_cameraControls.size(); ++i) {
		std::cout << "m_cameraControls[" << i << "]:\n" << std::endl;
		 m_cameraControls[i].Dump();
	}
	*/
	std::cout << "CCameraV4L2::Dump(). End\n";
}
#endif
///////////////////////////////////////////////////////////////////////////////
//
// Implementacion of CCameraControlV4L2 class
//

CCameraControlV4L2::CCameraControlV4L2 (CHandle handle, const CControl& control) :
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
int CCameraControlV4L2::GetValue() const
{
	CControlValue value;
	value.type= m_type;
	value.value= 0;
	
	if (c_get_control (m_handle, m_id, &value)!= C_SUCCESS) {
		fprintf (stderr, "CCameraControlV4L2::GetValue() failed to query value\n");
		return 0;
	}
	
	return value.value;
}

bool CCameraControlV4L2::SetValue(int value)
{
	CControlValue cvalue;
	cvalue.type= m_type;
	cvalue.value= value;
	
	if (c_set_control (m_handle, m_id, &cvalue)!= C_SUCCESS) {
		fprintf (stderr, "CCameraControlV4L2::SetValue() failed to set value\n");
		return false;
	}
	return true;
}

const char* CCameraControlV4L2::GetChoiceName(unsigned int numOption) const
{
	if (numOption> (unsigned int) m_max) return NULL;
	
	return  m_choices[numOption].c_str();
}

bool CCameraControlV4L2::CheckSupportedLibwebcamId (CControlId id)
{
	return (LibwebcamId2ECameraControlId(id)!= CAM_ERROR_ENTRY);	
}

CCameraControl::ECameraControlId CCameraControlV4L2::LibwebcamId2ECameraControlId (CControlId id)
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

CCameraControl::ECameraControlType CCameraControlV4L2::CControlType2ECameraControlType (CControlType type)
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

//
// Debugging code
//
#if !defined(NDEBUG)

static
void print_device_info (CHandle handle, char *device_name)
{
	assert(handle || device_name);
	unsigned int size = static_cast<unsigned int>(sizeof(CDevice)) + 
		(device_name? static_cast<unsigned int>(strlen(device_name)): 32) + 84;
	CDevice *info = (CDevice *)malloc(size);
	assert(info);

	if(handle)
		printf("  Getting device information for handle %d ...\n", handle);
	else if(device_name)
		printf("  Getting device information for device name '%s' ...\n", device_name);
	
	int ret = c_get_device_info(handle, device_name, info, &size);
	if(ret) {
		fprintf(stderr, "  Failed to c_get_device_info (%d).\n", ret);
	}
	else {
		printf("    { shortName = '%s', name = '%s', driver = '%s', location = '%s', vid = %04x, pid = %04x, bcd = %d }\n",
				info->shortName, info->name, info->driver, info->location, info->usb.vendor, info->usb.product, info->usb.release);
	}

	free(info);
}

static
void print_control_info(CControl *control)
{
	printf("      { id = %d, name = '%s', type = %d, flags = %d",
			control->id, control->name, control->type, control->flags);
	if(control->type == CC_TYPE_CHOICE) {
		printf(", choice = {");
		for(unsigned int index = 0; index < control->choices.count; index++) {
			printf(" '%s'[%d]", control->choices.list[index].name, control->choices.list[index].index);
		}
		printf(" }");
	}
	else {
		printf(", min = %d, max = %d, def = %d, step = %d",
				control->min.value, control->max.value, control->def.value, control->step.value);
	}
	printf(" }\n");
}

static
void print_device_controls(CHandle handle)
{
	unsigned int size = 0, count = 0;

	printf("  Getting control information for handle %d ...\n", handle);
	CResult ret = c_enum_controls(handle, NULL, &size, &count);
	if(ret == C_BUFFER_TOO_SMALL) {
		CControl *controls = (CControl *)malloc(size);
		ret = c_enum_controls(handle, controls, &size, &count);
		if(ret) fprintf(stderr, "Unable to c_enum_controls (%d).\n", ret);
/*		printf("    Buffer size = %d (%d controls of %d bytes)\n", size, count, sizeof(CControl));*/

		for(unsigned int i = 0; i < count; i++) {
			CControl *control = &controls[i];
			printf("    Control found: %s\n", control->name);
			print_control_info(control);
		}

		free(controls);
	}
	else {
		printf("  No controls found (ret = %d).\n", ret);
	}
}

void CCameraControlV4L2::Dump()
{
	std::cout << " CCameraControlV4L2::Dump(). Begin\n";
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
	std::cout << " CCameraControlV4L2::Dump(). End\n";
}
#endif
//
// End Debugging code
//


//
// Static attributes
//
int CCameraV4L2::g_numDevices= -1;
char CCameraV4L2::g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGHT];
char CCameraV4L2::g_deviceShortNames[MAX_CAM_DEVICES][CAM_DEVICE_SHORT_NAME_LENGHT];
char CCameraV4L2::g_deviceDriverNames[MAX_CAM_DEVICES][CAM_DEVICE_DRIVER_NAME_LENGHT];
int CCameraV4L2::g_numInstances= 0;

void CCameraV4L2::InstanceCreated() throw(camera_exception)
{
	// TODO: thread safety
	if (g_numInstances== 0) {
		GetNumDevices();	// Fill structures when needed
		CResult retval= c_init();
		if(retval) throw camera_exception("cannot initialize libwebcam");		
	}
	++g_numInstances;
}

void CCameraV4L2::InstanceDestroyed() 
{
	// TODO: thread safety
	assert (g_numInstances> 0);

	if (--g_numInstances== 0) {
		c_cleanup();
		g_numDevices= -1;
	}
}

int CCameraV4L2::GetNumDevices()
{
	// Linux: uses libwebcam
	if (g_numDevices== -1) {
		CResult retval= c_init();
		if(retval) { 
			fprintf(stderr, "Unable to c_init (%d).\n", retval);
			return 0;
		}
		
		// Enumerate devices
		unsigned int size = 0, count = 0;
		retval= c_enum_devices(NULL, &size, &count);
		if(retval == C_BUFFER_TOO_SMALL) {
			char devices_buffer[size];
			CDevice *devices = (CDevice *) devices_buffer;

			retval = c_enum_devices(devices, &size, &count);
			if(retval) { 
				fprintf(stderr, "Unable to c_enum_devices (%d).\n", retval);
				return 0;
			}
			// Adjust & store number of maximum devices
			if (count> MAX_CAM_DEVICES) count= MAX_CAM_DEVICES;
			g_numDevices= count;	
		
			// Store information about detected devices
			for(unsigned int i = 0; i< count; ++i) {
				CDevice *device = &devices[i];
				
				// Prepend device number and append device name
				unsigned int j= count - 1 - i;
				snprintf (g_deviceNames[j], CAM_DEVICE_NAME_LENGHT, " (Id:%d) %s", j, device->name);		
				snprintf (g_deviceShortNames[j], CAM_DEVICE_SHORT_NAME_LENGHT, "%s", device->shortName);
				snprintf (g_deviceDriverNames[j], CAM_DEVICE_DRIVER_NAME_LENGHT, "%s", device->driver);

	#ifndef NDEBUG
				//printf("  Device found: %s\n", device->name);
				CHandle handle = c_open_device(device->shortName);
				if(handle == 0) {
					fprintf(stderr, "  Failed to open device '%s'.\n", device->shortName);
					continue;
				}
				printf("  Opened device '%s' successfully (handle = %d)\n", device->shortName, handle);
				print_device_info(handle, NULL);
				print_device_controls(handle);
				c_close_device(handle);
				handle = 0;
				printf("  Closed device '%s' (handle = %d)\n\n", device->shortName, handle);
	#endif	// NDEBUG
			}
		}
		else {
			// No devices found
			g_numDevices= 0;			
		}
		
		// Frees libwebcam
		c_cleanup();		
	}

	return g_numDevices;
}

const char* CCameraV4L2::GetDeviceName (unsigned int id)
{
	if ((int) id>= GetNumDevices()) return NULL;
	return g_deviceNames[id];
}
