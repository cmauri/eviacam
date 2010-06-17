/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_v4l2.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     17/05/2010
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
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

#include "crvcamera.h"
//#include <cv.h>
//#include "highgui.h"

#include <asm/types.h>
//#include <v4l2.h>
#include <linux/videodev2.h>
#include <crvimage.h>

#define MAX_CV_DEVICES 10



class CCameraV4L2 : public CCamera
{
public:
	CCameraV4L2(int cameraId= -1, int width= 320, int height= 240, float fr= 30.0f);
	virtual ~CCameraV4L2 (void);

	virtual bool Open();
	virtual void Close();	
	virtual IplImage *QueryFrame();

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
	
	static int GetNumDevices();
	static char *GetDeviceName (int id);
	

	

private:
	//CvCapture* m_pCvCapture;
	//static bool g_cvInitialized;
	static int g_numDevices;
	static char g_deviceNames[MAX_CV_DEVICES][50];
	
	struct buffer {    
	  void *                  start;
	  size_t                  length;
	};
	
	int m_Fd;
	int m_PixelFormat;
	
	struct buffer m_buffer;
	
	CIplImage * m_image;
};


#endif
