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
	// Return the number of cameras detected 
	static int GetNumDevices();
	
	// Return the name of a camera given its number or NULL
	// if the camera doesn't exist.
	static const char *GetDeviceName (unsigned int id);
	
	// Create a camera instace given its number. 
	// Expects the desired frame size and rate.
	// Returns NULL if the camera doesn't exist or another
	// error occurred.
	static CCamera* GetCamera 
		(unsigned int id,
		unsigned int width= 320,
		unsigned int height= 240,
		float frameRate= 30.0f);
		
private:
	CCameraEnum();	// Object instantation forbiden	
};


#endif
