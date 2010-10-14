/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_v4l2.h
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
#ifndef CRVCAMERA_V4L2_H_
#define CRVCAMERA_V4L2_H_

#include <inttypes.h>
#include <linux/videodev2.h>
#include "crvcamera_enum.h"
#include "crvcamera.h"
#include "crvimage.h"
#include "webcam.h"
#include <vector>

class CCameraV4L2 : public CCamera
{
public:
	// Device name without trailing "/dev/"
	CCameraV4L2(const char* device_name, bool usePwc);
	virtual ~CCameraV4L2 (void);

	virtual bool Open();
	virtual void Close();	
	virtual IplImage *QueryFrame();

private:
	// Private types
	enum ECaptureMethod { CAP_NONE= 0, CAP_READ, CAP_STREAMING_MMAP, CAP_STREAMING_USR };
	enum { STREAMING_CAPTURE_NBUFFERS= 2 };
	struct TImageFormat { 
		unsigned int frame_rate;
		unsigned int width;
		unsigned int height;
		uint32_t pixelformat;	// Four character code
	};

	// Device name without trailing "/dev/"
	char m_deviceShortName[CCameraEnum::CAM_DEVICE_SHORT_NAME_LENGHT];
	int m_Fd;	// File descritor for open
	CHandle m_libWebcamHandle;	
	ECaptureMethod m_captureMethod;
	bool m_usePwc;
	bool m_isStreaming;
	bool m_buffersReady;
	struct v4l2_buffer m_captureBuffersInfo[STREAMING_CAPTURE_NBUFFERS];
	void* m_captureBuffersPtr[STREAMING_CAPTURE_NBUFFERS];
	CIplImage m_resultImage;
	TImageFormat m_currentFormat;
	std::vector<uint32_t> m_supportedPixelFormats;
			
	// Private methods
	bool DoOpen();
	ECaptureMethod DetectCaptureMethod();
	bool DetectBestImageFormat(TImageFormat& imgformat);
	bool SetImageFormat(const TImageFormat& imgformat);
	bool EnableVideo(bool enable);
	bool AllocateBuffers();
	bool DeallocateBuffers();
	bool RequestBuffers(enum v4l2_memory mem);
	bool UnRequestBuffers(enum v4l2_memory mem);
	void UnmapBuffers();
	bool DecodeToRGB (void* src, void* dst, int width, int height, uint32_t pixelformat);
	void AddSupportedPixelFormats ();
public:
#if 0
	// 
	// Virtual desired methods
	//
	virtual bool HasSettingsDialog() { return true; }
	
	virtual int GetBrightness ();
	virtual void SetBrightness (int value);
	virtual int GetContrast ();
	virtual void SetContrast (int value);
	virtual int GetSaturation ();
	virtual void SetSaturation (int value);
	virtual int GetWhiteness ();
	virtual void SetWhiteness (int value);
	virtual int GetHue ();
	virtual void SetHue (int value);
	virtual int GetFlicker ();
	virtual void SetFlicker (int value);
	virtual int GetExposure ();
	virtual void SetExposure (int value);
	virtual int GetBacklightCompensation ();
	virtual void SetBacklightCompensation (int value);
	virtual int GetSharpness ();
	virtual void SetSharpness (int value);

	virtual void ShowSettingsDialog ();
#endif


};


#endif
