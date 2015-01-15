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
class CCameraControlV4L2;

class CCameraV4L2 : public CCamera
{
public:
	CCameraV4L2(int cameraId= -1, unsigned int width= 320, 
		unsigned int height= 240, float fr= 30.0f) throw(camera_exception);
	virtual ~CCameraV4L2 (void);

	
	virtual bool HasCameraControls() { return true; }
	virtual unsigned int GetCameraControlsCount();
	virtual CCameraControl* GetCameraControl(unsigned int numControl);
	
	static int GetNumDevices();
	static const char* GetDeviceName (unsigned int id);
	void Dump();

protected:
	virtual bool DoOpen();
	virtual void DoClose();	
	virtual IplImage *DoQueryFrame();
	virtual bool DoQueryFrame(CIplImage& image);

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
	
	int m_Id;
	TImageFormat m_desiredFormat;
	TImageFormat m_currentFormat;
	CHandle m_libWebcamHandle;	
	ECaptureMethod m_captureMethod;
	bool m_isStreaming;
	bool m_buffersReady;
	struct v4l2_buffer m_captureBuffersInfo[STREAMING_CAPTURE_NBUFFERS];
	void* m_captureBuffersPtr[STREAMING_CAPTURE_NBUFFERS];
	CIplImage m_resultImage;	
	std::vector<uint32_t> m_supportedPixelFormats;
	std::vector<CCameraControlV4L2> m_cameraControls;

	//
	// Static members
	//
	enum { MAX_CAM_DEVICES= 10, CAM_DEVICE_NAME_LENGHT= 50 };
	static int g_numDevices;
	static char g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGHT];
	enum { CAM_DEVICE_SHORT_NAME_LENGHT= 32, CAM_DEVICE_DRIVER_NAME_LENGHT= 20 };
	static char g_deviceShortNames[MAX_CAM_DEVICES][CAM_DEVICE_SHORT_NAME_LENGHT];
	static char g_deviceDriverNames[MAX_CAM_DEVICES][CAM_DEVICE_DRIVER_NAME_LENGHT];
	// Instance count for automatic libwebcam cleanup
	static int g_numInstances;
	// Instances inc/dec
	void InstanceCreated() throw(camera_exception);
	void InstanceDestroyed();

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
class CCameraControlV4L2 : public CCameraControl {
public:
	CCameraControlV4L2 (CHandle handle, const CControl& control);
	
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
	
	// Check whether a certain control id is supported
	static bool CheckSupportedLibwebcamId (CControlId id);

	void Dump();

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
