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
#include <string>

// Forward declarations
class CCameraControlV4l2;

class CCameraV4L2 : public CCamera
{
public:
	// Device name without trailing "/dev/"
	CCameraV4L2(const char* device_name, bool usePwc);
	virtual ~CCameraV4L2 (void);

	virtual bool DoOpen();
	virtual void DoClose();	
	virtual IplImage *DoQueryFrame();

	virtual bool HasCameraControls() { return true; }
	virtual unsigned int GetCameraControlsCount();
	virtual CCameraControl* GetCameraControl(unsigned int numControl);
#if !defined(NDEBUG)
	void Dump();
#endif
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
	int m_Fd;	// TODO: remove // File descritor for open
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
	std::vector<CCameraControlV4l2> m_cameraControls;

	// Private methods
	bool InternalOpen();
	ECaptureMethod DetectCaptureMethod();
	bool DetectBestImageFormat();
	bool SetImageFormat();
	bool EnableVideo(bool enable);
	bool AllocateBuffers();
	bool DeallocateBuffers();
	bool RequestBuffers(enum v4l2_memory mem);
	bool UnRequestBuffers(enum v4l2_memory mem);
	void UnmapBuffers();
	bool DecodeToRGB (void* src, void* dst, int width, int height, uint32_t pixelformat);
	void AddSupportedPixelFormats ();
	bool PopulateCameraControls ();
};

// Class that models each camera control
class CCameraControlV4l2 : public CCameraControl {
public:
	CCameraControlV4l2 (CHandle handle, const CControl& control);
	
	virtual ECameraControlId GetId() const { return LibwebcamId2ECameraControlId(m_id); }
	// Get the name of the control provided by the driver
	virtual const char* GetName() const { return m_name.c_str(); }
	virtual ECameraControlType GetType() const { return CControlType2ECameraControlType (m_type); }

	// Get/set the current value. For boolean controls 0 and 1 are the only
	// possible values. For choice controls 0 represents the first option.
	// Set method returns true on success, false otherwise
	virtual int GetValue() const;
	virtual bool SetValue(int value);

	virtual int GetDefaultValue() const { return m_default; }
	virtual int GetMinimumValue() const { return m_min; }
	virtual int GetMaximumValue() const { return m_max; }

	// For choices only
	virtual const char* GetChoiceName(unsigned int numOption) const;
	
	// Check whether a certain contorl id is supported
	static bool CheckSupportedLibwebcamId (CControlId id);
#if !defined(NDEBUG)
	void Dump();
#endif
private:
	static ECameraControlId LibwebcamId2ECameraControlId (CControlId id);
	static ECameraControlType CControlType2ECameraControlType (CControlType type);
	
	CHandle m_handle;
	CControlId m_id;	// The identifier of the control
	std::string m_name;	// The name of the control
	CControlType m_type;	// The type of the control
	int m_default, m_min, m_max;
	std::vector<std::string> m_choices;
};

#endif
