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

class videoInput;
class CIplImage;

class CCameraWDM : public CCamera
{
public:
	CCameraWDM(int cameraId= -1, int width= 320, int height= 240, float fr= 30.0f);
	virtual ~CCameraWDM (void);

	static int GetNumDevices();
	static char* GetDeviceName (int id);

	virtual bool DoOpen();
	virtual void DoClose();	
	virtual IplImage *DoQueryFrame();

	virtual bool HasSettingsDialog() { return true; }
	virtual void ShowSettingsDialog ();

private:
	int m_Id;
	int m_Width, m_Height;
	float m_FrameRate;
	videoInput* m_VI;
	int m_numDevices;
	CIplImage* m_pImage;
};

#endif
