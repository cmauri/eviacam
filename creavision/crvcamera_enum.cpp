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

/*
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <fstream>
#include <iostream>
#include <asm/types.h>          //
#include <linux/videodev2.h>
#include <linux/videodev.h>
#include "pwc-ioctl.h"

#include "crvcamera.h"

#include <asm/types.h>
NO #include <v4l2.h>
#include <linux/videodev2.h>
#include <crvimage.h>
*/

#include <stdio.h>
#ifdef WIN32
	#include "videoInput.h"
	#include "crvcamera_wdm.h"
#else
	#include "webcam.h"
	#include "crvcamera_cv.h"
#endif
#include "crvcamera_enum.h"

//
// Debugging code
//
#if !defined(NDEBUG) && !defined(WIN32)
#include <stdlib.h>	// malloc, free
#include <assert.h>
#include <string.h>

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
			snprintf (g_deviceNames[i], CAM_DEVICE_NAME_LENGHT, " (Id:%d) %s\0", i, videoInput::getDeviceName(i));
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
	return new CCameraCV(id, 320, 240);
#endif
}

void CCameraEnum::Cleanup()
{
#ifdef WIN32
	// Release the library
	c_cleanup();
#endif
}

#if 0

bool CCameraV4L2::Open ()
{
	struct stat st;
	struct v4l2_capability capability;
	struct v4l2_format format;
	struct buffer * buffers = NULL;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_frmsizeenum frmsizeenum;
	struct v4l2_streamparm parm;
	struct video_window vwin;

        int min;
        int widthDesired = m_Width;
        	
	if (-1 == stat ("/dev/video0", &st)) {
	    printf ("ERROR: Cannot identify ’%s’: %d, %s\n", "/dev/video0", errno, strerror (errno));
	    return false;
	}
	if (!S_ISCHR (st.st_mode)) {
	    printf ("ERROR: %s is no device\n", "/dev/video0");
	    return false;
	}
	
	m_Fd = open ("/dev/video0", O_RDWR, 0);
	
	if (-1 == m_Id) {
	    printf ("ERROR: Cannot open ’%s’: %d, %s\n", "/dev/video0", errno, strerror (errno));
	    return false;
	}
	
	if (-1 == ioctl (m_Fd, VIDIOC_QUERYCAP, &capability)) {
	    printf ("ERROR: VIDIOC_QUERYCAP\n");
	    Close();
	    return false;
	  
	}
	
	if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
	    printf ("ERROR: is no video capture device\n");
	    Close();
	    return false;
	}  
	
	if (!(capability.capabilities & V4L2_CAP_READWRITE)) {
	    printf("The driver not supports read/write\n");
	    Close();
	    return false;
	}
	
	if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
	    printf("The driver not supports streaming (memory map)\n");
	    Close();
	    return false;
	}
	
        
        //Select frame format
        memset (&fmtdesc, 0, sizeof (fmtdesc));
	  
        fmtdesc.index = 0;
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
        int f = 15;
        
        while (ioctl (m_Fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0) {
            switch (fmtdesc.pixelformat) {
                case V4L2_PIX_FMT_BGR24:
                    f = 1;
                    break;
                    
                case V4L2_PIX_FMT_RGB24:
                    if (f > 2) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 2;
                    }
                    break;
                                        
                case V4L2_PIX_FMT_BGR32:
                    if (f > 3) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 3;
                    }
                    break;
                    
                case V4L2_PIX_FMT_RGB32:
                    if (f > 4) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 4;
                    }
                    break;
            
                case V4L2_PIX_FMT_RGB555:
                    if (f > 5) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 5;
                    }
                    break;
                    
                case V4L2_PIX_FMT_RGB565:
                    if (f > 6) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 6;
                    }
                    break;
                    
                case V4L2_PIX_FMT_RGB555X:
                    if (f > 7) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 7;
                    }
                    break;
                    
                case V4L2_PIX_FMT_RGB565X:
                    if (f > 8) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 8;
                    }
                    break;
                    
                case V4L2_PIX_FMT_RGB444:
                    if (f > 9) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 9;
                    }
                    break;
                    
                case V4L2_PIX_FMT_YUV32:
                    if (f > 10) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 10;
                    }
                    break;
                    
                case V4L2_PIX_FMT_YUV444:
                    if (f > 11) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 11;
                    }
                    break;
                    
                case V4L2_PIX_FMT_YUV555:
                    if (f > 12) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 12;
                    }
                    break;
                    
                case V4L2_PIX_FMT_YUV565:
                    if (f > 13) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 13;
                    }
                    break;
                    
                case V4L2_PIX_FMT_YUV420:
                    if (f > 14) {
                        m_PixelFormat = fmtdesc.pixelformat;
                        f = 14;
                    }
                    break;
            }
                    
	  
            fmtdesc.index++;
        }
	
        
        //Select frame size
        memset (&frmsizeenum, 0, sizeof (frmsizeenum));

        frmsizeenum.index = 0;
        frmsizeenum.pixel_format = fmtdesc.pixelformat;
        
        while (ioctl (m_Fd, VIDIOC_ENUM_FRAMESIZES, &frmsizeenum) == 0) {
            switch (frmsizeenum.type) {
                case V4L2_FRMSIZE_TYPE_DISCRETE:
                    if (frmsizeenum.index == 0) {
                        m_Width = frmsizeenum.discrete.width;
                        m_Height = frmsizeenum.discrete.height;
                    } else {
                        if (abs(frmsizeenum.discrete.width - widthDesired) < abs(m_Width - widthDesired)) {
                            m_Width = frmsizeenum.discrete.width;
                            m_Height = frmsizeenum.discrete.height;
                        }
                    }
                    break;
                
                case V4L2_FRMSIZE_TYPE_STEPWISE:
                    if (frmsizeenum.index == 0) {
                        m_Width = frmsizeenum.stepwise.step_width;
                        m_Height = frmsizeenum.stepwise.step_height;
                    } else {
                        if (abs(frmsizeenum.stepwise.step_width - widthDesired) < abs(m_Width - widthDesired)) {
                            m_Width = frmsizeenum.stepwise.step_width;
                            m_Height = frmsizeenum.stepwise.step_height;
                        }
                    }
                    break;
            }
             frmsizeenum.index++;
        }

        
        
	
	//Set frame format
	memset (&format, 0, sizeof (format));

	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl (m_Fd, VIDIOC_G_FMT, &format)) {
	    printf("ERROR: Unable to get format.\n");
	} else {
	    format.fmt.pix.width = m_Width;
	    format.fmt.pix.height = m_Height;
	    format.fmt.pix.pixelformat = m_PixelFormat;
	    format.fmt.pix.field = V4L2_FIELD_ANY;
	  
	    if (-1 == ioctl (m_Fd, VIDIOC_S_FMT, &format)) {
		printf("ERROR: Unable to set format.\n");
		Close();
		return false;
	    }
	}

	/* Buggy driver paranoia. */
	min = format.fmt.pix.width * 2;
	if (format.fmt.pix.bytesperline < min)
	    format.fmt.pix.bytesperline = min;
	    min = format.fmt.pix.bytesperline * format.fmt.pix.height;
	if (format.fmt.pix.sizeimage < min)
	    format.fmt.pix.sizeimage = min;
	
	
	m_buffer.length = format.fmt.pix.sizeimage;
	m_buffer.start = malloc (format.fmt.pix.sizeimage);
 	
	if (!m_buffer.start) {
	    fprintf (stderr, "Out of memory\n");
	    exit (EXIT_FAILURE);
	}
	


	//Set framerate
	memset(&parm, 0, sizeof (parm));

	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	if (0 == ioctl (m_Fd, VIDIOC_G_PARM, &parm)) {
	    parm.parm.capture.timeperframe.numerator = 1;
	    parm.parm.capture.timeperframe.denominator = m_FrameRate;
	    if (-1 == ioctl(m_Fd, VIDIOC_S_PARM, &parm)) {
                printf("ERROR: VIDIOC_S_PARM");
	    }
	} else {
            //We use not V4L2 ioctl to allow PWC drivers set framerate
            if (ioctl(m_Fd, VIDIOCGWIN, &vwin) == 0) {
                
                if (vwin.flags & PWC_FPS_FRMASK) {
                    vwin.flags &= ~PWC_FPS_FRMASK;
                    vwin.flags |= ((int)m_FrameRate << PWC_FPS_SHIFT);
                    if (ioctl(m_Fd, VIDIOCSWIN, &vwin) == -1)
                        printf("ERROR: VIDIOCGWIN");
                }
            
            } else {
                printf("ERROR: This device doesn't support setting the framerate.\n");
            }
	}
	
	m_image = new CIplImage(format.fmt.pix.width, format.fmt.pix.height, IPL_DEPTH_8U, "BGR");
	
        
	
/*
/// TO SET UP FRAME RATE
// Set the preview rate in case we want to support previews in the future.
	
	capPreviewRate(m_hwndPreview, 33);

	// Attempt to get the capture parameters.
	capDriverGetCaps(m_hwndPreview, &m_capdrivercaps, sizeof(m_capdrivercaps));

	// Default values.
	m_captureparms.dwRequestMicroSecPerFrame = 33333;
	m_captureparms.fMakeUserHitOKToCapture = FALSE;
	m_captureparms.wPercentDropForError = 100;
	m_captureparms.fYield = TRUE;
	m_captureparms.dwIndexSize = 0;
	m_captureparms.wChunkGranularity = 0;
	m_captureparms.fUsingDOSMemory = FALSE;
	m_captureparms.wNumVideoRequested = 3;
	m_captureparms.fCaptureAudio = FALSE;
	m_captureparms.wNumAudioRequested = 0;
	m_captureparms.vKeyAbort = 0;
	m_captureparms.fAbortLeftMouse = FALSE;
	m_captureparms.fAbortRightMouse = FALSE;
	m_captureparms.fLimitEnabled = FALSE;
	m_captureparms.wTimeLimit = 0;
	m_captureparms.fMCIControl = FALSE;
	m_captureparms.fStepMCIDevice = FALSE;
	m_captureparms.dwMCIStartTime = 0;
	m_captureparms.dwMCIStopTime = 0;
	m_captureparms.fStepCaptureAt2x = FALSE;
	m_captureparms.wStepCaptureAverageFrames = 5;
	m_captureparms.dwAudioBufferSize = 0;
	m_captureparms.fDisableWriteCache = FALSE;
	m_captureparms.AVStreamMaster = 0;

	// Attempt to set the capture parameters.
	capCaptureSetSetup(m_hwndPreview, &m_captureparms, sizeof(m_captureparms));

	// Make sure that the values we have are correct.
	capCaptureGetSetup(m_hwndPreview, &m_captureparms, sizeof(m_captureparms));


	capSetVideoFormat ?????

	capSetUserData ???
	*/

	return true;
}


#endif


