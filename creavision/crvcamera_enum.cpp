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

#include <stdio.h>
#ifdef WIN32
	#include "videoInput.h"
	#include "crvcamera_wdm.h"
#else
	#include <string.h>
	#include "webcam.h"
	#include "crvcamera_cv.h"
	#include "crvcamera_v4l2.h"
#endif
#include "crvcamera_enum.h"

//
// Debugging code
//
#if !defined(NDEBUG) && !defined(WIN32)
#include <stdlib.h>	// malloc, free
#include <assert.h>

void print_device_info (CHandle handle, char *device_name)
{
	assert(handle || device_name);
	unsigned int size = sizeof(CDevice) + (device_name ? strlen(device_name) : 32) + 84;
	CDevice *info = (CDevice *)malloc(size);
	assert(info);

	if(handle)
		printf("  Getting device information for handle %d ...\n", handle);
	else if(device_name)
		printf("  Getting device information for device name '%s' ...\n", device_name);
	
	int ret = c_get_device_info(handle, device_name, info, &size);
	if(ret) {
		fprintf(stderr, "  Failed to c_get_device_info (%d).\n", ret);
	}
	else {
		printf("    { shortName = '%s', name = '%s', driver = '%s', location = '%s', vid = %04x, pid = %04x, bcd = %d }\n",
				info->shortName, info->name, info->driver, info->location, info->usb.vendor, info->usb.product, info->usb.release);
	}

	free(info);
}

void print_control_info(CControl *control)
{
	printf("      { id = %d, name = '%s', type = %d, flags = %d",
			control->id, control->name, control->type, control->flags);
	if(control->type == CC_TYPE_CHOICE) {
		printf(", choice = {");
		for(unsigned int index = 0; index < control->choices.count; index++) {
			printf(" '%s'[%d]", control->choices.list[index].name, control->choices.list[index].index);
		}
		printf(" }");
	}
	else {
		printf(", min = %d, max = %d, def = %d, step = %d",
				control->min.value, control->max.value, control->def.value, control->step.value);
	}
	printf(" }\n");
}

void print_device_controls(CHandle handle)
{
	unsigned int size = 0, count = 0;

	printf("  Getting control information for handle %d ...\n", handle);
	CResult ret = c_enum_controls(handle, NULL, &size, &count);
	if(ret == C_BUFFER_TOO_SMALL) {
		CControl *controls = (CControl *)malloc(size);
		ret = c_enum_controls(handle, controls, &size, &count);
		if(ret) fprintf(stderr, "Unable to c_enum_controls (%d).\n", ret);
/*		printf("    Buffer size = %d (%d controls of %d bytes)\n", size, count, sizeof(CControl));*/

		for(unsigned int i = 0; i < count; i++) {
			CControl *control = &controls[i];
			printf("    Control found: %s\n", control->name);
			print_control_info(control);
		}

		free(controls);
	}
	else {
		printf("  No controls found (ret = %d).\n", ret);
	}
}

#endif // DEBUG
//
// End Debugging code
//

//
// Static attributes
//
int CCameraEnum::g_numDevices= -1;
char CCameraEnum::g_deviceNames[MAX_CAM_DEVICES][CAM_DEVICE_NAME_LENGHT];
#ifndef WIN32
char CCameraEnum::g_deviceShortNames[MAX_CAM_DEVICES][CAM_DEVICE_SHORT_NAME_LENGHT];
char CCameraEnum::g_deviceDriverNames[MAX_CAM_DEVICES][CAM_DEVICE_DRIVER_NAME_LENGHT];
#endif

// 
// Static member functions
//
int CCameraEnum::GetNumDevices()
{
//
#ifdef WIN32
// 			
	if (g_numDevices== -1) { // || strlen(videoInput::getDeviceName(0))== 0) {
		
		// First initialization
		g_numDevices= videoInput::listDevices (true);
		if (g_numDevices> MAX_CAM_DEVICES) g_numDevices= MAX_CAM_DEVICES;
		//char buff[CV_DEVICE_NAME_LENGHT];
		for (int i= 0; i< g_numDevices; i++) {
			// Prepend device number and append device name
			_snprintf (g_deviceNames[i], CAM_DEVICE_NAME_LENGHT, " (Id:%d) %s\0", i, videoInput::getDeviceName(i));
		}		
	}
//
#else
//
	if (g_numDevices== -1) {
		CResult retval= c_init();
		if(retval) { 
			fprintf(stderr, "Unable to c_init (%d).\n", retval);
			return 0;
		}
		
		// Enumerate devices
		unsigned int size = 0, count = 0;
		retval= c_enum_devices(NULL, &size, &count);
		if(retval == C_BUFFER_TOO_SMALL) {
			char devices_buffer[size];
			CDevice *devices = (CDevice *) devices_buffer;

			retval = c_enum_devices(devices, &size, &count);
			if(retval) { 
				fprintf(stderr, "Unable to c_enum_devices (%d).\n", retval);
				return 0;
			}
			// Adjust & store number of maximum devices
			if (count> MAX_CAM_DEVICES) count= MAX_CAM_DEVICES;
			g_numDevices= count;	
		
			// Store information about detected devices
			for(unsigned int i = 0; i< count; ++i) {
				CDevice *device = &devices[i];
				
				// Prepend device number and append device name
				unsigned int j= count - 1 - i;
				snprintf (g_deviceNames[j], CAM_DEVICE_NAME_LENGHT, " (Id:%d) %s", j, device->name);		
				snprintf (g_deviceShortNames[j], CAM_DEVICE_SHORT_NAME_LENGHT, "%s", device->shortName);
				snprintf (g_deviceDriverNames[j], CAM_DEVICE_DRIVER_NAME_LENGHT, "%s", device->driver);

	#ifndef NDEBUG
				//printf("  Device found: %s\n", device->name);
				CHandle handle = c_open_device(device->shortName);
				if(handle == 0) {
					fprintf(stderr, "  Failed to open device '%s'.\n", device->shortName);
					continue;
				}
				printf("  Opened device '%s' successfully (handle = %d)\n", device->shortName, handle);
				print_device_info(handle, NULL);
				print_device_controls(handle);
				c_close_device(handle);
				handle = 0;
				printf("  Closed device '%s' (handle = %d)\n\n", device->shortName, handle);
	#endif		
			}

					
		}
		else {
			// No devices found
			g_numDevices= 0;
			//printf("No devices found.\n");
		}
	}
//
#endif
//
	return g_numDevices;
}

char* CCameraEnum::GetDeviceName (unsigned int id)
{
	if ((int) id>= g_numDevices) return NULL;
	return g_deviceNames[id];
}

CCamera* CCameraEnum::GetCamera (unsigned int id)
{
	if ((int) id>= g_numDevices) return NULL;
#ifdef WIN32
	return new CCameraWDM(id, 320, 240);
#else
	// Depending on the driver instantiates the appropiate class
	if (strcasestr(g_deviceDriverNames[id], "uvc")!= NULL) 
		// Camera supported by uvc driver
		return new CCameraV4L2 (g_deviceShortNames[id], false);
	else if (strcasestr(g_deviceDriverNames[id], "pwc")!= NULL) 
		// Camera supported by pwc driver
		return new CCameraV4L2 (g_deviceShortNames[id], true); 
	else
		return new CCameraV4L2 (g_deviceShortNames[id], false);
		//return new CCameraCV(id, 320, 240);
#endif
}

void CCameraEnum::Cleanup()
{
#ifndef WIN32
	// Release the library
	c_cleanup();
#endif
}



