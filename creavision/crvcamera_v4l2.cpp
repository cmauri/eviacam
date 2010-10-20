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
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "pwc-ioctl.h"
#include "colorspaces.h"
/*
#include <getopt.h>             // getopt_long() 
#include <asm/types.h>          // for videodev2.h
//#include <v4l2.h>
*/

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
	
	// Open device
	m_Fd= open (devName, O_RDWR);	
	if (m_Fd== -1) {
		fprintf (stderr, "ERROR: Cannot open ’%s’: %d, %s\n", devName, errno, strerror (errno));
		return false;
	}

	// "Open" device via libwebcam 
	m_libWebcamHandle= c_open_device (m_deviceShortName);
	if (m_libWebcamHandle== 0) {
		fprintf (stderr, "ERROR: Cannot open ’%s’ via libwebcam\n", devName);
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

bool CCameraV4L2::DetectBestImageFormat(TImageFormat& imgformat)
{
	// Here we should implement a "clever" selection according with the 
	// actual camera capabilities using libwebcam
	// Instructions:
	//  - Take a look to crvcamera_enum & webcam.h to see how to iterate
	//	though the camera properties. Command uvcdynctrl coudl also be
	//	useful. You can rely on an open CHandle
	//	in m_libWebcamHandle and the device name in m_deviceShortName.
	//	Don't add new attributes to the class nor use dynamic memory
	//	(use stack allocation instead). You can add new private methods
	//	when necessary.
	//  - The critical parameter is frame_rate, select first a set of image sizes
	//	& pixel formats that support the highgest frame rate. Note that
	//	some camera models doesn't report frame rate so, the code should
	//	manage properly this situation
	//  - Then choose the closest matching resolution to 320x240 and finally
	//	the pixelformat. m_supportedPixelFormats is array of supported
	//	pixelformats in order of preference (first values are better).
	//  - Return false only in case no supported formats are available at all 
	//	(i.e. pixel format not supported).
	//std::vector<TImageFormat> vectForm;
			
	// TODO: TESTING CODE set hardcoded compatible values for my camera
	if (m_usePwc)
		imgformat.pixelformat= V4L2_PIX_FMT_YUV420;
	else
		imgformat.pixelformat= V4L2_PIX_FMT_YUYV;
	imgformat.frame_rate= 30;
	imgformat.width= 320;
	imgformat.height= 240;	
	// TODO: TESTING CODE set hardcoded compatible values for my camera

	return true;
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
	}

	return true;
/*
	// Buggy driver paranoia. 
	min = format.fmt.pix.width * 2;
	if (format.fmt.pix.bytesperline < min)
	    format.fmt.pix.bytesperline = min;
	    min = format.fmt.pix.bytesperline * format.fmt.pix.height;
	if (format.fmt.pix.sizeimage < min)
	    format.fmt.pix.sizeimage = min;
*/
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

	DetectBestImageFormat(imgformat);
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
	//return true;
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

							 static inline void
							 move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v, int rowPixels, unsigned char * rgb)
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

	case EIO:
				      // Se puede ignorar EIO
	default:
	printf("ERROR: read\n");
#endif

}

#if 0
int CCameraV4L2::GetBrightness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BRIGHTNESS;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}
    
    
void CCameraV4L2::SetBrightness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BRIGHTNESS;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetContrast ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_CONTRAST;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_CONTRAST;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetContrast (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_CONTRAST;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_CONTRAST;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetSaturation ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SATURATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SATURATION;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetSaturation (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SATURATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
        memset(&control, 0, sizeof(control));
        control.id = V4L2_CID_SATURATION;
        control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;
        ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetWhiteness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    
    queryctrl.id = V4L2_CID_GAMMA;
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_GAMMA;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetWhiteness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_GAMMA;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_GAMMA;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetHue ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    struct video_picture vpic;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_HUE;

    if (ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl) == 0)  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_HUE;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    } else if (ioctl(m_Fd, VIDIOCGPICT, &vpic) == 0) {
	return vpic.hue;
    }

    return -1;
}


void CCameraV4L2::SetHue (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    struct video_picture vpic;
   
    memset(&queryctrl, 0, sizeof(queryctrl));
    memset(&vpic, 0, sizeof(vpic));
    queryctrl.id = V4L2_CID_HUE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_HUE;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    } else {
	vpic.hue = value;
	ioctl(m_Fd, VIDIOCSPICT, &vpic);
    }    
}


int CCameraV4L2::GetFlicker ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_POWER_LINE_FREQUENCY;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return control.value;
	}
    }
    return -1;
}


void CCameraV4L2::SetFlicker (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    int flicker = 0;
    
     switch (value) {
	case 0:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_DISABLED;
	    break;
	case 1:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_50HZ;
	    break;
	case 2:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_60HZ;
	    break;
    }
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_POWER_LINE_FREQUENCY;
	control.value = flicker;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }   
}


int CCameraV4L2::GetExposure ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_EXPOSURE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_EXPOSURE;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return control.value;
	}
    }
    return -1;
}


void CCameraV4L2::SetExposure (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_EXPOSURE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_EXPOSURE;
	control.value = value;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    } 
 }





int CCameraV4L2::GetBacklightCompensation ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BACKLIGHT_COMPENSATION;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetBacklightCompensation (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BACKLIGHT_COMPENSATION;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}



int CCameraV4L2::GetSharpness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SHARPNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SHARPNESS;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}
    
    
void CCameraV4L2::SetSharpness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SHARPNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SHARPNESS;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


void CCameraV4L2::ShowSettingsDialog ()
{
    SetBrightness(700);
    SetContrast(500);
    SetWhiteness(800);
}
#endif
