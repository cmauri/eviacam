/**
 * \file
 * Declarations for compatibility with different V4L2 versions.
 *
 * \ingroup libwebcam
 */

/*
 * Copyright (c) 2006-2009 Logitech.
 *
 * This file is part of libwebcam.
 * 
 * libwebcam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libwebcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libwebcam.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef C_COMPAT_H
#define C_COMPAT_H

#include "incvideodev.h"
#include <linux/videodev2.h>

#ifdef USE_UVCVIDEO
#define __user
#include "uvcvideo.h"
#endif

#ifndef UVC_CONTROL_SET_CUR
/* Control flags */
#define UVC_CONTROL_SET_CUR	(1 << 0)
#define UVC_CONTROL_GET_CUR	(1 << 1)
#define UVC_CONTROL_GET_MIN	(1 << 2)
#define UVC_CONTROL_GET_MAX	(1 << 3)
#define UVC_CONTROL_GET_RES	(1 << 4)
#define UVC_CONTROL_GET_DEF	(1 << 5)
/* Control should be saved at suspend and restored at resume. */
#define UVC_CONTROL_RESTORE	(1 << 6)
/* Control can be updated by the camera. */
#define UVC_CONTROL_AUTO_UPDATE	(1 << 7)

#define UVC_CONTROL_GET_RANGE	(UVC_CONTROL_GET_CUR | UVC_CONTROL_GET_MIN | \
				 UVC_CONTROL_GET_MAX | UVC_CONTROL_GET_RES | \
				 UVC_CONTROL_GET_DEF)
#endif

#ifndef UVCIOC_CTRL_ADD
struct uvc_xu_control_info {
	__u8 entity[16];
	__u8 index;
	__u8 selector;
	__u16 size;
	__u32 flags;
};
#endif
// uvcvideo.h

// Old uvcvideo pre-r178 control IDs
#define UVC_CID_BACKLIGHT_COMPENSATION			(V4L2_CID_PRIVATE_BASE+0)
#define UVC_CID_POWER_LINE_FREQUENCY			(V4L2_CID_PRIVATE_BASE+1)
#define UVC_CID_SHARPNESS						(V4L2_CID_PRIVATE_BASE+2)
#define UVC_CID_HUE_AUTO						(V4L2_CID_PRIVATE_BASE+3)
#define UVC_CID_FOCUS_AUTO						(V4L2_CID_PRIVATE_BASE+4)
#define UVC_CID_FOCUS_ABSOLUTE					(V4L2_CID_PRIVATE_BASE+5)
#define UVC_CID_FOCUS_RELATIVE					(V4L2_CID_PRIVATE_BASE+6)
#define UVC_CID_PAN_RELATIVE					(V4L2_CID_PRIVATE_BASE+7)
#define UVC_CID_TILT_RELATIVE					(V4L2_CID_PRIVATE_BASE+8)
#define UVC_CID_PANTILT_RESET					(V4L2_CID_PRIVATE_BASE+9)
#define UVC_CID_EXPOSURE_AUTO					(V4L2_CID_PRIVATE_BASE+10)
#define UVC_CID_EXPOSURE_ABSOLUTE				(V4L2_CID_PRIVATE_BASE+11)
#define UVC_CID_EXPOSURE_AUTO_PRIORITY			(V4L2_CID_PRIVATE_BASE+14)
#define UVC_CID_WHITE_BALANCE_TEMPERATURE_AUTO	(V4L2_CID_PRIVATE_BASE+12)
#define UVC_CID_WHITE_BALANCE_TEMPERATURE		(V4L2_CID_PRIVATE_BASE+13)


// videodev2.h

// Class IDs
#ifndef V4L2_CTRL_CLASS_USER
#define V4L2_CTRL_CLASS_USER			0x00980000
#endif
#ifndef V4L2_CTRL_CLASS_MPEG
#define V4L2_CTRL_CLASS_MPEG			0x00990000
#endif
#ifndef V4L2_CTRL_CLASS_CAMERA
#define V4L2_CTRL_CLASS_CAMERA			0x009a0000
#endif

// Macros
#ifndef V4L2_CTRL_ID2CLASS
#define V4L2_CTRL_ID2CLASS(id)			((id) & 0x0fff0000UL)
#endif

// User controls from newer V4L2 versions
#ifndef V4L2_CID_POWER_LINE_FREQUENCY
#define V4L2_CID_POWER_LINE_FREQUENCY		(V4L2_CID_BASE+24)
#endif
#ifndef V4L2_CID_HUE_AUTO
#define V4L2_CID_HUE_AUTO					(V4L2_CID_BASE+25)
#endif
#ifndef V4L2_CID_WHITE_BALANCE_TEMPERATURE
#define V4L2_CID_WHITE_BALANCE_TEMPERATURE	(V4L2_CID_BASE+26)
#endif
#ifndef V4L2_CID_SHARPNESS
#define V4L2_CID_SHARPNESS					(V4L2_CID_BASE+27)
#endif
#ifndef V4L2_CID_BACKLIGHT_COMPENSATION
#define V4L2_CID_BACKLIGHT_COMPENSATION		(V4L2_CID_BASE+28)
#endif

// Camera class controls from newer V4L2 versions
#ifndef V4L2_CID_CAMERA_CLASS_BASE
#define V4L2_CID_CAMERA_CLASS_BASE		(V4L2_CTRL_CLASS_CAMERA | 0x900)
#endif
#ifndef V4L2_CID_EXPOSURE_AUTO
#define V4L2_CID_EXPOSURE_AUTO			(V4L2_CID_CAMERA_CLASS_BASE+1)
#endif
#ifndef V4L2_CID_EXPOSURE_ABSOLUTE
#define V4L2_CID_EXPOSURE_ABSOLUTE		(V4L2_CID_CAMERA_CLASS_BASE+2)
#endif
#ifndef V4L2_CID_EXPOSURE_AUTO_PRIORITY
#define V4L2_CID_EXPOSURE_AUTO_PRIORITY	(V4L2_CID_CAMERA_CLASS_BASE+3)
#endif
#ifndef V4L2_CID_PAN_RELATIVE
#define V4L2_CID_PAN_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+4)
#endif
#ifndef V4L2_CID_TILT_RELATIVE
#define V4L2_CID_TILT_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+5)
#endif
#ifndef V4L2_CID_PAN_RESET
#define V4L2_CID_PAN_RESET				(V4L2_CID_CAMERA_CLASS_BASE+6)
#endif
#ifndef V4L2_CID_TILT_RESET
#define V4L2_CID_TILT_RESET				(V4L2_CID_CAMERA_CLASS_BASE+7)
#endif
#ifndef V4L2_CID_PAN_ABSOLUTE
#define V4L2_CID_PAN_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+8)
#endif
#ifndef V4L2_CID_TILT_ABSOLUTE
#define V4L2_CID_TILT_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+9)
#endif
#ifndef V4L2_CID_FOCUS_ABSOLUTE
#define V4L2_CID_FOCUS_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+10)
#endif
#ifndef V4L2_CID_FOCUS_RELATIVE
#define V4L2_CID_FOCUS_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+11)
#endif
#ifndef V4L2_CID_FOCUS_AUTO
#define V4L2_CID_FOCUS_AUTO				(V4L2_CID_CAMERA_CLASS_BASE+12)
#endif
#ifndef V4L2_CID_ZOOM_ABSOLUTE
#define V4L2_CID_ZOOM_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+13)
#endif
#ifndef V4L2_CID_ZOOM_RELATIVE
#define V4L2_CID_ZOOM_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+14)
#endif
#ifndef V4L2_CID_ZOOM_CONTINUOUS
#define V4L2_CID_ZOOM_CONTINUOUS		(V4L2_CID_CAMERA_CLASS_BASE+15)
#endif
#ifndef V4L2_CID_PRIVACY
#define V4L2_CID_PRIVACY				(V4L2_CID_CAMERA_CLASS_BASE+16)
#endif


#endif /* C_COMPAT_H */
