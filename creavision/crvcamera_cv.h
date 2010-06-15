/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_cv.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
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
#ifndef CRVCAMERA_CV_H_
#define CRVCAMERA_CV_H_

#include "crvcamera.h"
#include <cv.h>
#include <highgui.h>

#define MAX_CV_DEVICES 10

class CCameraCV : public CCamera
{
public:
	CCameraCV(int cameraId= -1, int width= 320, int height= 240, float fr= 30.0f);
	virtual ~CCameraCV (void);

	virtual bool Open();
	virtual void Close();	
	virtual IplImage *QueryFrame();

	static int GetNumDevices();
	static char *GetDeviceName (int id);

private:
	CvCapture* m_pCvCapture;
	static bool g_cvInitialized;
	static int g_numDevices;
	static char g_deviceNames[MAX_CV_DEVICES][50];
};

#if defined(WIN32)
// Workround to enable capture at 30fps for some camera models
// Should be called with administrative rights
// Return 0 if ok, -1 if permission denied
int VfwCamFpsWorkaround ();
#endif

#endif
