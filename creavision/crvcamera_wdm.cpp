/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_wdm.cpp
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
#include "crvcamera_wdm.h"

#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <cassert>
#include <cv.h>
#include "crvimage.h"
#include "videoInput.h"

CCameraWDM::CCameraWDM(int cameraId, int width, int height, float fr)
{
	m_Id= cameraId;
	m_Width= width;
	m_Height= height;
	m_FrameRate= fr;

	m_VI= NULL;	

	videoInput::setVerbose(false);
}

CCameraWDM::~CCameraWDM(void)
{
	Close();
}

bool CCameraWDM::DoOpen ()
{
	// Create a videoInput object
	if (m_VI) return false;

	m_VI= new videoInput;
	assert (m_VI);

	// BGR channel sequence assumed
	m_pImage= new CIplImage (m_Width, m_Height, IPL_DEPTH_8U, "BGR");
	assert (m_pImage);	

	m_VI->setIdealFramerate(m_Id, (int) m_FrameRate);
	if (!m_VI->setupDevice (m_Id, m_Width, m_Height)) {
		Close();
		return false;
	}

	return true;	
}

void CCameraWDM::DoClose ()
{
	if (m_VI) {
		m_VI->stopDevice(m_Id);
		delete m_VI;
		m_VI= NULL;
		delete m_pImage;
	}
}

IplImage *CCameraWDM::DoQueryFrame()
{
	if (!m_VI) return NULL;

	assert (m_pImage->ptr()->imageSize== m_VI->getSize(m_Id));
	
	m_VI->getPixels(m_Id, (unsigned char *) m_pImage->ptr()->imageData, false, false);
	// Set appropiate origin
	m_pImage->ptr()->origin= 1;
	
	return m_pImage->ptr();
}

void CCameraWDM::ShowSettingsDialog ()
{
	if (m_VI) m_VI->showSettingsWindow (m_Id);
}

int CCameraWDM::GetNumDevices() 
{ 
	int numDevices;

	// Try to detect if previously initialized
	if (strlen(GetDeviceName (0))== 0) {
		char buff[20];

		// Not initialized. Add the camera number to the name
        numDevices= videoInput::listDevices (true);

		for (int i= 0; i< numDevices; i++) {
			sprintf (buff, " (Id:%d)\0", i);
			strncat (GetDeviceName (i), buff, 19);			
		}		
	}
	else
		// Already initialized
		numDevices= videoInput::listDevices (true);

	return numDevices;
}

char* CCameraWDM::GetDeviceName (int id)
{ 
	return videoInput::getDeviceName(id); 
}