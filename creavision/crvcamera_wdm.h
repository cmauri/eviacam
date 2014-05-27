/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_wdm.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/01/2009
// Copyright:   (C) 2009 Cesar Mauri Loba - CREA Software Systems
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
#ifndef CRVCAMERA_WDM_H_
#define CRVCAMERA_WDM_H_

#include "crvcamera.h"
#include "crvimage.h"
class videoInput;

class CCameraWDM : public CCamera
{
public:
	CCameraWDM(int cameraId= -1, unsigned int width= 320, 
		unsigned int height= 240, float fr= 30.0f) throw(camera_exception);
	virtual ~CCameraWDM (void);

	static int GetNumDevices();
	static const char* GetDeviceName (unsigned int id);

	virtual bool DoOpen();
	virtual void DoClose();	
	virtual IplImage *DoQueryFrame();
	virtual bool DoQueryFrame(CIplImage& image);

	virtual bool HasSettingsDialog() { return true; }
	virtual void ShowSettingsDialog ();

private:
	enum { MAX_CAM_DEVICES= 10, CAM_DEVICE_NAME_LENGHT= 50 };
	static int g_numDevices;
	static char g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGHT];

	int m_Id;
	unsigned int m_Width, m_Height;
	float m_FrameRate;
	videoInput* m_VI;
	CIplImage m_Image;
};

#endif
