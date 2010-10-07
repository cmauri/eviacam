/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_enum.h
// Purpose:  
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
#ifndef CRVCAMERA_ENUM_H_
#define CRVCAMERA_ENUM_H_

class CCamera;

class CCameraEnum
{
public:
	
	static int GetNumDevices();
	static char *GetDeviceName (unsigned int id);
	static CCamera* GetCamera (unsigned int id);
	static void Cleanup();
private:
	enum { MAX_CAM_DEVICES= 10, CAM_DEVICE_NAME_LENGHT= 50 };
	static int g_numDevices;
	static char g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGHT];
#ifndef WIN32
	// Linux only. Store device localtion (e.g. video0)
	enum { CAM_DEVICE_SHORT_NAME_LENGHT= 32, CAM_DEVICE_DRIVER_NAME_LENGHT= 20 };
	static char g_deviceShortNames[MAX_CAM_DEVICES][CAM_DEVICE_SHORT_NAME_LENGHT];
	static char g_deviceDriverNames[MAX_CAM_DEVICES][CAM_DEVICE_DRIVER_NAME_LENGHT];
#endif
};


#endif
