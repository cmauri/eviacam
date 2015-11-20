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
#include "crvimage.h"
#include <cv.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <cassert>

// Return timestamp in ms
static long long GetTime (void)
{
	struct timeb now;	
	ftime(&now);
	return now.time*1000 + now.millitm;
}

CCamera::CCamera() throw (camera_exception)
{
	m_RealWidth=0;
	m_RealHeight= 0;
	m_RealFrameRate= 0.0f;
	m_LastRealFrameRate= 0.0f;
	m_lastTimeStamp= GetTime();
	m_horizontalFlip= false;
}

CCamera::~CCamera(void)
{
	// Should call Close but is not done here
	// because DoClose is virtual. We rely on
	// derived classes
}

bool CCamera::Open()
{
	bool retval= DoOpen();
	if (retval) m_lastTimeStamp= GetTime();
	return retval;
}

void CCamera::Close()
{
	DoClose();
}

IplImage* CCamera::QueryFrame()
{
	IplImage* pImage= DoQueryFrame();
	if (!pImage) return NULL;

	PostQueryFrame(pImage);

	return pImage;
}

void CCamera::PostQueryFrame(IplImage* pImage)
{
	// Update real size
	m_RealWidth= pImage->width;
	m_RealHeight= pImage->height;

	// Update real FPS
	long long now= GetTime();
	long long elapsedTime= now - m_lastTimeStamp;
	m_lastTimeStamp= now;
	m_LastRealFrameRate= m_RealFrameRate;

	float weight= ((float) elapsedTime / 1000.0f) * 1.5f;
	if (weight> 1.0f) weight= 1.0f;
	if (elapsedTime> 0)
		m_RealFrameRate= (1000.0f / (float) elapsedTime) * weight + m_LastRealFrameRate * (1.0f - weight);
	else
		m_RealFrameRate= 0;

	// Flip image when needed to provide an image with top-left origin
	if ( pImage->origin == 1 ) {
		if (m_horizontalFlip) 
			cvFlip (pImage, NULL, -1);
		else 
			cvFlip (pImage, NULL, 0);

		pImage->origin= 0;
	}
	else 
		if (m_horizontalFlip) 
			cvFlip (pImage, NULL, 1);
}
