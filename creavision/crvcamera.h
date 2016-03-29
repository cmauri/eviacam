/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
// Copyright:   (C) 2008-10 Cesar Mauri Loba - CREA Software Systems
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
#ifndef CRVCAMERA_H_
#define CRVCAMERA_H_

#include <stdexcept>

// Forward declarations
class CCameraControl;
typedef struct _IplImage IplImage;
class CIplImage;

// Class to signal errors during camera object construction
class camera_exception : public std::runtime_error
{
public:
	camera_exception (const char* msg) : std::runtime_error (msg) {}
};

#if defined (_MSC_VER)
#pragma warning(disable: 4290)
#endif

class CCamera
{
public:
	CCamera() throw (camera_exception);
	virtual ~CCamera (void);

	// 
	// Open, close and capture operations.
	//
	bool Open();
	void Close();
	// TODO: return const ptr
	IplImage* QueryFrame();

	//
	// Capture information. Intended only for informational
	// purposes. Values returned here may not be accurate
	// (i.e. may change during capture).
	//
	int GetRealWidth () {return m_RealWidth; }
	int GetRealHeight () {return m_RealHeight; }
	float GetRealFrameRate () {return m_RealFrameRate; }

	//
	// Image flip control
	//
	bool GetHorizontalFlip () { return m_horizontalFlip; }
	void SetHorizontalFlip (bool value) { m_horizontalFlip= value; }

	//
	// Settings dialogue provided by the driver
	//
	virtual bool HasSettingsDialog() { return false; }
	virtual void ShowSettingsDialog () {}

	//
	// Settings directly supported by the camera class.
	//
	virtual bool HasCameraControls() { return false; }
	virtual unsigned int GetCameraControlsCount() { return 0; }
	virtual CCameraControl* GetCameraControl(unsigned int) { return 0; }
	
protected:
	// Open, close and capture operations implemented using NVI idiom
	virtual bool DoOpen()= 0;
	virtual void DoClose()= 0;
	virtual IplImage* DoQueryFrame()= 0;
	//virtual bool DoQueryFrame(CIplImage& image)= 0;

private:
	// Make CCamera non-copyable
	CCamera( const CCamera& );	// not implemented
	CCamera& operator=( const CCamera& );     // not implemented

	void PostQueryFrame(IplImage* pImage);

	int m_RealWidth, m_RealHeight;
	float m_RealFrameRate, m_LastRealFrameRate;
	long long m_lastTimeStamp;
	bool m_horizontalFlip;
};

// Class that models each camera control
class CCameraControl {
public:
	enum ECameraControlId {
		CAM_BRIGHTNESS= 0,
		CAM_CONTRAST,
		CAM_GAIN,
		CAM_SATURATION,
		CAM_HUE,
		CAM_GAMMA,
		CAM_SHARPNESS,
		CAM_WHITE_BALANCE_TEMPERATURE,		// White balance color temperature
		CAM_AUTO_WHITE_BALANCE_TEMPERATURE,	// Enable/disable automatic balance color
		CAM_WHITE_BALANCE_COMPONENT,		// White balance color component control (red, blue)
		CAM_AUTO_WHITE_BALANCE_COMPONENT,	// Enable/disable automatic white balance color component selection		
		CAM_BACKLIGHT_COMPENSATION,
		CAM_POWER_LINE_FREQUENCY,	// Local power line frequency for anti-flicker processing
		CAM_AUTO_HUE,			// Enable/disable automatic hue control
		CAM_AUTO_EXPOSURE_MODE,		// Mode for automatic exposure control
		CAM_AUTO_EXPOSURE_PRIORITY,	// Constraints for automatic exposure control (e.g. adaptive vs. constant frame rate)
		CAM_EXPOSURE_TIME_ABSOLUTE,	// Length of exposure (electronic shutter speed)
		CAM_EXPOSURE_TIME_RELATIVE,	// Relative change to the current length of exposure
		// Optics control
		CAM_AUTO_FOCUS,		// Enable/disable automatic focus
		CAM_FOCUS_ABSOLUTE,	// Distance to the focused target
		CAM_FOCUS_RELATIVE,	// Relative change in distance to currently focused target
		CAM_IRIS_ABSOLUTE, 	// Aperture setting	
		CAM_IRIS_RELATIVE, 	// Relative change to the current aperture setting	
		CAM_ZOOM_ABSOLUTE, 	// Objective lens focal length	
		CAM_ZOOM_RELATIVE, 	// Relative change to the current objective lens focal length	
		CAM_DIGITAL_ZOOM,  	// Digital zoom multiplier applied to the optical image
		// Motion control	
		CAM_PAN_ABSOLUTE,	// Pan angle (rotation on a vertical axis)	
		CAM_PAN_RELATIVE,	// Relative change to the current pan angle	
		CAM_TILT_ABSOLUTE,	// Tilt angle (rotation in a vertical plane)	
		CAM_TILT_RELATIVE,	// Relative change to the current tilt angle	
		CAM_ROLL_ABSOLUTE,	// Roll angle (rotation along the image viewing axis)	
		CAM_ROLL_RELATIVE,	// Relative change to the current roll angle
		CAM_PRIVACY,		// Temporarily prevent image from being acquired
		CAM_PAN_RESET,		// Reset pan angle to a safe default value.
		CAM_TILT_RESET,		// Reset tilt angle to a safe default value.
		// Logitech custom contols
		CAM_LOGITECH_PANTILT_RELATIVE,	// Relative change to the current pan and tilt angles.
		CAM_LOGITECH_PANTILT_RESET,	// Reset pan and tilt angles to a safe default value.
		CAM_LOGITECH_LED1_MODE, 		// Illumination mode of the first LED.
		CAM_LOGITECH_LED1_FREQUENCY, 	// Blinking frequency of the first LED.
		CAM_LOGITECH_DISABLE_PROCESSING, // Disable video processing (enable raw mode)
		CAM_LOGITECH_RAW_BITS_PER_PIXEL, 	// Bits per pixel for raw (Bayer) mode
				
		CAM_LATEST_ENTRY,
		CAM_ERROR_ENTRY= CAM_LATEST_ENTRY
	};
	
	// Types of controls
	enum ECameraControlType { 
		CCTYPE_BOOLEAN, 
		CCTYPE_CHOICE, 
		CCTYPE_NUMBER,
		CCTYPE_BUTTON
	};

	virtual ~CCameraControl() {};
	
	virtual ECameraControlId GetId() const= 0;
	// Get the name of the control provided by the driver
	virtual const char* GetName() const= 0;
	virtual ECameraControlType GetType() const= 0;
	
	// Get/set the current value. For boolean controls 0 and 1 are the only
	// possible values. For choice controls 0 represents the first option.
	// Set method returns true on success, false otherwise
	virtual int GetValue() const= 0;
	virtual bool SetValue(int value)= 0;
	
	virtual int GetDefaultValue() const= 0;
	
	virtual int GetMinimumValue() const= 0;
	virtual int GetMaximumValue() const= 0;
	
	// For choices only
	virtual const char* GetChoiceName(unsigned int) const { return 0; }
};



#endif
