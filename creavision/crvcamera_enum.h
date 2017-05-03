/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_enum.h
// Purpose:  
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
#ifndef CRVCAMERA_ENUM_H_
#define CRVCAMERA_ENUM_H_

class CCamera;

class CCameraEnum
{
public:
	enum { NUM_DRIVERS = 2 };

	/**
		Get the number of cameras detected 

		@param driverId driver identifier
			0: "native driver"
			1: openCV driver

		@return the name of the device or NULL if does not exists
	*/
	static int getNumDevices(int driverId);
	
	/**
		Return the name of a camera

		@param driverId driver identifier
			0: "native driver"
			1: openCV driver
		@param camId number of camera

		@return the name of the device or NULL if does not exists
	*/
	static const char *getDeviceName(int driverId, int camId);
	
	/**
		Create a camera instace
		
		@param driverId driver identifier
			0: "native driver"
			1: openCV driver
		@param camId number of camera
		@width
		@height
		@frameRate

		@return instance of CCamera or NULL if the camera doesn't 
			exist or another
	*/
	static CCamera* getCamera(
		int driverId, int camId, unsigned int width = 320, unsigned int height = 240,
		float frameRate = 30.0f);

private:
	CCameraEnum();
};


#endif
