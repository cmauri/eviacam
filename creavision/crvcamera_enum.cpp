/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_enum.cpp
// Purpose:  	Detect, enumerate and create camera object
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     1/10/2010
// Copyright:   (C) 2008-15 Cesar Mauri Loba - CREA Software Systems
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
#include "crvcamera_wdm.h"
#define NATIVE_CAM_CLASS CCameraWDM

#else // linux
#include <string.h>
#include "webcam.h"
#include "crvcamera_cv.h"
#include "crvcamera_v4l2.h"
#define NATIVE_CAM_CLASS CCameraV4L2

#endif

#include "crvcamera_enum.h"
#include "simplelog.h"

#define CAMERA_CV_ID 1

int CCameraEnum::getNumDevices(int driverId)
{
	if (driverId == CAMERA_CV_ID) {
		return CCameraCV::GetNumDevices();
	}
	else {
		return NATIVE_CAM_CLASS::GetNumDevices();
	}
}

const char* CCameraEnum::getDeviceName(int driverId, int camId)
{
	if (driverId == CAMERA_CV_ID) {
		return CCameraCV::GetDeviceName(camId);
	}
	else {
		return NATIVE_CAM_CLASS::GetDeviceName(camId);
	}
}

CCamera* CCameraEnum::getCamera(int driverId, int camId, unsigned int width, unsigned int height, float frameRate)
{
	if (camId>= getNumDevices(driverId)) return NULL;

	try{
		if (driverId == CAMERA_CV_ID) {
			return new CCameraCV(camId, width, height, frameRate);
		}
		else {
			return new NATIVE_CAM_CLASS(camId, width, height, frameRate);
		}
	}
	catch (camera_exception &e)	{
		slog_write (SLOG_PRIO_ERR, "error initializing camera: %s\n", e.what());
	}

	return NULL;
}

