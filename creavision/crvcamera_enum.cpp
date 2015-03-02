/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_enum.cpp
// Purpose:  	Detect, enumerate and create camera object
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     1/10/2010
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

#ifdef WIN32
	#include "crvcamera_cv.h"

	#define CAMCLASS CCameraCV
#else
	#include <string.h>
	#include "webcam.h"
	#include "crvcamera_cv.h"
	#include "crvcamera_v4l2.h"

	#define CAMCLASS CCameraV4L2
#endif
#include "crvcamera_enum.h"
#include "simplelog.h"

// 
// Static member functions
//
int CCameraEnum::GetNumDevices()
{
	return CAMCLASS::GetNumDevices();
}

const char* CCameraEnum::GetDeviceName (unsigned int id)
{
	return CAMCLASS::GetDeviceName((int) id);
}

#define _unused(x) ((void)x)

CCamera* CCameraEnum::GetCamera (unsigned int id, unsigned int width,
		unsigned int height, float frameRate)
{
	if ((int) id>= GetNumDevices()) return NULL;

	try{
		return new CAMCLASS(id, width, height, frameRate);		
	} 
	catch (camera_exception &e)	{
		slog_write (SLOG_PRIO_ERR, "error initializing camera: %s\n", e.what());
	}
	
	return NULL;
}
