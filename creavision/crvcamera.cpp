/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera.cpp
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
#include "crvcamera.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <cassert>
#include <highgui.h>

CCamera::CCamera()
{
	m_Id= -1;
	m_Width= 0;
	m_Height= 0;
	m_RealWidth=0;
	m_RealHeight= 0;
	m_FrameRate= 0;
	m_RealFrameRate= 0.0f;
	m_LastRealFrameRate= 0.0f;
	m_showingLive= false;
	m_lastTimeStamp= GetTime();
	m_horizontalFlip= false;
}

CCamera::~CCamera(void)
{
}

// Return timestamp in ms
int CCamera::GetTime (void)
{
	struct timeb now;
	
	ftime(&now);

	return now.time*1000 + now.millitm;
}

void CCamera::OnQueryFrame(IplImage *pImage)
{
	// Camera window
	if (m_showingLive)
	{
		char winName[20];
		sprintf (winName, "Camera%d", m_Id);
		cvShowImage( winName, pImage );
		//cvWaitKey( 10 );
	}

	// Update real size
	m_RealWidth= pImage->width;
	m_RealHeight= pImage->height;

	// Update real FPS
	int now= GetTime();
	m_elapsedTime= now - m_lastTimeStamp;
	m_lastTimeStamp= now;
	m_LastRealFrameRate= m_RealFrameRate;

	float weight= ((float) m_elapsedTime / 1000.0f) * 1.5f;
	if (weight> 1.0f) weight= 1.0f;
	if (m_elapsedTime> 0)
		m_RealFrameRate= (1000.0f / (float) m_elapsedTime) * weight + m_LastRealFrameRate * (1.0f - weight);
	else
		m_RealFrameRate= 0;
}

void CCamera::ShowLive ()
{
	if (m_showingLive) return;

	char winName[20];
	sprintf (winName, "Camera%d", m_Id);
	cvNamedWindow( winName );

	m_showingLive= true;
}

void CCamera::CloseLive ()
{
	if (!m_showingLive) return;

	char winName[20];
	sprintf (winName, "Camera%d", m_Id);
	cvDestroyWindow( winName );

	m_showingLive= false;
}

float CCamera::GetFrameRate ()
{
	return m_FrameRate;
}

float CCamera::GetRealFrameRate ()
{
	return m_RealFrameRate;
}
