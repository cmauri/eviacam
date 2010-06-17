/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_v4l2.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     17/05/2010
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
#include "crvcamera_v4l2.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <cassert>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fstream>
#include <iostream>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <linux/videodev.h>
#include "pwc-ioctl.h"



//bool CCameraV4L2::g_cvInitialized= false;
int CCameraV4L2::g_numDevices= 0;
char CCameraV4L2::g_deviceNames[MAX_CV_DEVICES][50];


CCameraV4L2::CCameraV4L2(int cameraId, int width, int height, float fr)
{
	m_Id= cameraId;
	m_Fd = 0;
	m_Width= width;
	m_Height= height;
	m_FrameRate= fr;
	m_image = NULL;
	m_buffer.length = 0;
	m_buffer.start = 0;
	m_PixelFormat = 0;
}

CCameraV4L2::~CCameraV4L2(void)
{
	Close ();	
}


void CCameraV4L2::Close ()
{
	if (m_Fd != 0) {
	    close(m_Fd);
	    m_Fd = 0;
	}
	
	if (m_image != NULL) {
	    delete(m_image);
	    m_image = NULL;
	}
	
	if (m_buffer.start != NULL) {
	    free(m_buffer.start);
	    m_buffer.start = NULL;
	}
}



int CCameraV4L2::GetBrightness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BRIGHTNESS;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}
    
    
void CCameraV4L2::SetBrightness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BRIGHTNESS;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetContrast ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_CONTRAST;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_CONTRAST;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetContrast (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_CONTRAST;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_CONTRAST;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetSaturation ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SATURATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SATURATION;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetSaturation (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SATURATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
        memset(&control, 0, sizeof(control));
        control.id = V4L2_CID_SATURATION;
        control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;
        ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetWhiteness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    
    queryctrl.id = V4L2_CID_GAMMA;
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_GAMMA;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetWhiteness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_GAMMA;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_GAMMA;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


int CCameraV4L2::GetHue ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    struct video_picture vpic;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_HUE;

    if (ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl) == 0)  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_HUE;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    } else if (ioctl(m_Fd, VIDIOCGPICT, &vpic) == 0) {
	return vpic.hue;
    }

    return -1;
}


void CCameraV4L2::SetHue (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    struct video_picture vpic;
   
    memset(&queryctrl, 0, sizeof(queryctrl));
    memset(&vpic, 0, sizeof(vpic));
    queryctrl.id = V4L2_CID_HUE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_HUE;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    } else {
	vpic.hue = value;
	ioctl(m_Fd, VIDIOCSPICT, &vpic);
    }    
}


int CCameraV4L2::GetFlicker ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_POWER_LINE_FREQUENCY;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return control.value;
	}
    }
    return -1;
}


void CCameraV4L2::SetFlicker (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    int flicker = 0;
    
     switch (value) {
	case 0:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_DISABLED;
	    break;
	case 1:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_50HZ;
	    break;
	case 2:
	    flicker = V4L2_CID_POWER_LINE_FREQUENCY_60HZ;
	    break;
    }
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_POWER_LINE_FREQUENCY;
	control.value = flicker;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }   
}


int CCameraV4L2::GetExposure ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_EXPOSURE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_EXPOSURE;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return control.value;
	}
    }
    return -1;
}


void CCameraV4L2::SetExposure (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_EXPOSURE;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_EXPOSURE;
	control.value = value;
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    } 
 }





int CCameraV4L2::GetBacklightCompensation ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BACKLIGHT_COMPENSATION;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}


void CCameraV4L2::SetBacklightCompensation (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_BACKLIGHT_COMPENSATION;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}



int CCameraV4L2::GetSharpness ()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SHARPNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SHARPNESS;
	if (-1 != ioctl (m_Fd, VIDIOC_G_CTRL, &control))  {
	    return (control.value - queryctrl.minimum) * 1000 / (queryctrl.maximum - queryctrl.minimum);
	}
    }
    return -1;
}
    
    
void CCameraV4L2::SetSharpness (int value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control;
    
    memset(&queryctrl, 0, sizeof(queryctrl));
    queryctrl.id = V4L2_CID_SHARPNESS;
    
    if (0 == ioctl (m_Fd, VIDIOC_QUERYCTRL, &queryctrl))  {
	memset(&control, 0, sizeof(control));
	control.id = V4L2_CID_SHARPNESS;
	control.value = (value * (queryctrl.maximum - queryctrl.minimum) / 1000) + queryctrl.minimum;	
	ioctl (m_Fd, VIDIOC_S_CTRL, &control);
    }    
}


void CCameraV4L2::ShowSettingsDialog ()
{
    SetBrightness(700);
    SetContrast(500);
    SetWhiteness(800);
}



/*
 * Turn a YUV4:2:0 block into an RGB block
 *
 * Video4Linux seems to use the blue, green, red channel
 * order convention-- rgb[0] is blue, rgb[1] is green, rgb[2] is red.
 *
 * Color space conversion coefficients taken from the excellent
 * http://www.inforamp.net/~poynton/ColorFAQ.html
 * In his terminology, this is a CCIR 601.1 YCbCr -> RGB.
 * Y values are given for all 4 pixels, but the U (Pb)
 * and V (Pr) are assumed constant over the 2x2 block.
 *
 * To avoid floating point arithmetic, the color conversion
 * coefficients are scaled into 16.16 fixed-point integers.
 * They were determined as follows:
 *
 *  double brightness = 1.0;  (0->black; 1->full scale)
 *  double saturation = 1.0;  (0->greyscale; 1->full color)
 *  double fixScale = brightness * 256 * 256;
 *  int rvScale = (int)(1.402 * saturation * fixScale);
 *  int guScale = (int)(-0.344136 * saturation * fixScale);
 *  int gvScale = (int)(-0.714136 * saturation * fixScale);
 *  int buScale = (int)(1.772 * saturation * fixScale);
 *  int yScale = (int)(fixScale);
 */

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))

static inline void
move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v,
           int rowPixels, unsigned char * rgb)
{
	const int rvScale = 91881;
	const int guScale = -22553;
	const int gvScale = -46801;
	const int buScale = 116129;
	const int yScale  = 65536;
	int r, g, b;
	g = guScale * u + gvScale * v;
    //  if (force_rgb) {
    //      r = buScale * u;
    //      b = rvScale * v;
    //  } else {
	    r = rvScale * v;
	    b = buScale * u;
    //  }

	yTL *= yScale; yTR *= yScale;
	yBL *= yScale; yBR *= yScale;

	/* Write out top two pixels */
	rgb[0] = LIMIT(b+yTL); rgb[1] = LIMIT(g+yTL);
	rgb[2] = LIMIT(r+yTL);

	rgb[3] = LIMIT(b+yTR); rgb[4] = LIMIT(g+yTR);
	rgb[5] = LIMIT(r+yTR);

	/* Skip down to next line to write out bottom two pixels */
	rgb += 3 * rowPixels;
	rgb[0] = LIMIT(b+yBL); rgb[1] = LIMIT(g+yBL);
	rgb[2] = LIMIT(r+yBL);

	rgb[3] = LIMIT(b+yBR); rgb[4] = LIMIT(g+yBR);
	rgb[5] = LIMIT(r+yBR);
}



// Consider a YUV420P image of 8x2 pixels.
//
// A plane of Y values    A B C D E F G H
//                        I J K L M N O P
//
// A plane of U values    1   2   3   4
// A plane of V values    1   2   3   4 ....
//
// The U1/V1 samples correspond to the ABIJ pixels.
//     U2/V2 samples correspond to the CDKL pixels.
//
/* Converts from planar YUV420P to RGB24. */
static void
yuv420p_to_rgb24(int width, int height,
           unsigned char *pIn0, unsigned char *pOut0)
{
	const int numpix = width * height;
	const int bytes = 24 >> 3;
	int i, j, y00, y01, y10, y11, u, v;
	unsigned char *pY = pIn0;
	unsigned char *pU = pY + numpix;
	unsigned char *pV = pU + numpix / 4;
	unsigned char *pOut = pOut0;

	for (j = 0; j <= height - 2; j += 2) {
	    for (i = 0; i <= width - 2; i += 2) {
		y00 = *pY;
		y01 = *(pY + 1);
		y10 = *(pY + width);
		y11 = *(pY + width + 1);
		u = (*pU++) - 128;
		v = (*pV++) - 128;

		move_420_block(y00, y01, y10, y11, u, v, width, pOut);

		pY += 2;
		pOut += 2 * bytes;

	    }
	    pY += width;
	    pOut += width * bytes;
	}
}



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




IplImage *CCameraV4L2::QueryFrame()
{
	assert(m_Fd != 0);
	assert(m_buffer.length != 0);
	assert(m_buffer.start != 0);
	assert(m_image != NULL);
	
	IplImage * pIpl = m_image->ptr();
  
	if (-1 == read (m_Fd, m_buffer.start, m_buffer.length)) {
	      switch (errno) {
		  case EAGAIN:
		      return 0;

		  case EIO:
				      // Se puede ignorar EIO
		  default:
		      printf("ERROR: read\n");
	      }
	}
    
  
        if (m_PixelFormat == V4L2_PIX_FMT_YUV420) {
            yuv420p_to_rgb24(m_Width, m_Height, static_cast<unsigned char *> (m_buffer.start),                    reinterpret_cast<unsigned char *> (m_image->ptr()->imageData));
        }
    

	OnQueryFrame (m_image->ptr());
	
	return m_image->ptr();
	
	
}

int CCameraV4L2::GetNumDevices()
{
	int i;
	CvCapture* tmpCapture;

	//if (!g_cvInitialized) { cvInitSystem (0, NULL); g_cvInitialized= true; }

	// Detect number of connected devices
	/*
	for (i= 0; i< MAX_CV_DEVICES; i++)
	{
		 tmpCapture= cvCreateCameraCapture (i);
		 if (tmpCapture== NULL) break;

		 cvReleaseCapture (&tmpCapture);

		 // Generate device name
		 sprintf (g_deviceNames[i], "Camera: (Id:%d)", i);
	}
	*/
	
	sprintf (g_deviceNames[0], "Camera: (Id:%d)", 0);
	g_numDevices = 1;
	
	
	
	
	//g_numDevices= i;

	return g_numDevices;
}

char* CCameraV4L2::GetDeviceName (int id)
{
	if (id>= g_numDevices) return NULL;
	
	return g_deviceNames[id];
}
