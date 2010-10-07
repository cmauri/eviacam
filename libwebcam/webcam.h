/**
 * \file
 * Webcam library interface definition.
 *
 * \ingroup libwebcam
 */

/*
 * Copyright (c) 2006-2007 Logitech.
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

#ifndef C_WEBCAM_H
#define C_WEBCAM_H


/*
 * Basic type definitions
 */

/// Device handle for libwebcam
typedef unsigned int CHandle;
/// Function return type for libwebcam
typedef unsigned int CResult;



/*
 * Enums
 */

/**
 * Return values for libwebcam functions.
 */
enum _CResults {
	C_SUCCESS				= 0,	///< Success
	C_NOT_IMPLEMENTED,		///< The function is not implemented
	C_INIT_ERROR,			///< Error during initialization or library not initialized
	C_INVALID_ARG,			///< Invalid argument
	C_INVALID_HANDLE,		///< Invalid handle
	C_INVALID_DEVICE,		///< Invalid device or device cannot be opened
	C_NOT_EXIST,			///< The device to which the handle points no longer exists
	C_NOT_FOUND,			///< Object not found
	C_BUFFER_TOO_SMALL,		///< Buffer too small
	C_SYNC_ERROR,			///< Error during data synchronization
	C_NO_MEMORY,			///< Out of memory
	C_NO_HANDLES,			///< Out of handles
	C_V4L2_ERROR,			///< A Video4Linux2 API call returned an unexpected error
	C_SYSFS_ERROR,			///< A sysfs file access returned an error
	C_PARSE_ERROR,			///< A control could not be parsed
	C_CANNOT_WRITE,			///< Writing not possible (e.g. read-only control)
	C_CANNOT_READ,			///< Reading not possible (e.g. write-only control)
};


/**
 * Control identifiers.
 * This is an exhaustive list of all controls supported by libwebcam. As new
 * controls appear, these should be added to this list.
 */
typedef enum _CControlId {
	// UVC: Basic image control
	CC_BRIGHTNESS		= 1,			///< Image brightness
	CC_CONTRAST,						///< Image contrast
	CC_GAIN,							///< Gain control
	CC_SATURATION,						///< Image color saturation
	CC_HUE,								///< Hue
	CC_GAMMA,							///< Gamma correction
	CC_SHARPNESS,						///< Image sharpness

	// UVC: Advanced image control
	/// White balance color temperature
	CC_WHITE_BALANCE_TEMPERATURE,
	/// Enable/disable automatic while balance color temperature selection
	CC_AUTO_WHITE_BALANCE_TEMPERATURE,
	/// White balance color component control (red, blue)
	CC_WHITE_BALANCE_COMPONENT,
	/// Enable/disable automatic white balance color component selection
	CC_AUTO_WHITE_BALANCE_COMPONENT,
	/// Backlight compensation mode
	CC_BACKLIGHT_COMPENSATION,
	/// Local power line frequency for anti-flicker processing
	CC_POWER_LINE_FREQUENCY,
	/// Enable/disable automatic hue control
	CC_AUTO_HUE,
	/// Mode for automatic exposure control
	CC_AUTO_EXPOSURE_MODE,
	/// Constraints for automatic exposure control (e.g. adaptive vs. constant frame rate)
	CC_AUTO_EXPOSURE_PRIORITY,
	/// Length of exposure (electronic shutter speed)
	CC_EXPOSURE_TIME_ABSOLUTE,
	/// Relative change to the current length of exposure
	CC_EXPOSURE_TIME_RELATIVE,

	// UVC: Optics control
	/// Enable/disable automatic focus adjustment
	CC_AUTO_FOCUS,
	/// Distance to optimally focused target
	CC_FOCUS_ABSOLUTE,
	/// Relative change in distance to currently focused target
	CC_FOCUS_RELATIVE,
	/// Aperture setting
	CC_IRIS_ABSOLUTE,
	/// Relative change to the current aperture setting
	CC_IRIS_RELATIVE,
	/// Objective lens focal length
	CC_ZOOM_ABSOLUTE,
	/// Relative change to the current objective lens focal length
	CC_ZOOM_RELATIVE,
	/// Digital zoom multiplier applied to the optical image
	CC_DIGITAL_ZOOM,

	// UVC: Motion control
	/// Pan angle (rotation on a vertical axis)
	CC_PAN_ABSOLUTE,
	/// Relative change to the current pan angle
	CC_PAN_RELATIVE,
	/// Tilt angle (rotation in a vertical plane)
	CC_TILT_ABSOLUTE,
	/// Relative change to the current tilt angle
	CC_TILT_RELATIVE,
	/// Roll angle (rotation along the image viewing axis)
	CC_ROLL_ABSOLUTE,
	/// Relative change to the current roll angle
	CC_ROLL_RELATIVE,

	// UVC: Misc
	/// Temporarily prevent image from being acquired
	CC_PRIVACY,

	// V4L2: Motion control (new since 2.6.26)
	/// Reset pan angle to a safe default value.
	CC_PAN_RESET,
	/// Reset tilt angle to a safe default value.
	CC_TILT_RESET,

	// V4L2: Unknown controls
	/// Base for unknown V4L2 controls
	CC_V4L2_BASE				= 0x1000,
	/// Base for custom V4L2 driver controls
	CC_V4L2_CUSTOM_BASE			= 0x2000,
	/// Base for unknown V4L2 MPEG controls
	CC_V4L2_MPEG_BASE			= 0x3000,
	/// Base for unknown V4L2 camera class controls
	CC_V4L2_CAMERA_CLASS_BASE	= 0x4000,

	// Logitech (USB vendor ID: 0x046d)
	CC_LOGITECH_BASE = 0x046d0000,
	/// Relative change to the current pan and tilt angles.
	/// This control is specific to Logitech cameras that support panning and
	/// tilting such as the QuickCam Orbit series.
	CC_LOGITECH_PANTILT_RELATIVE,
	/// Reset pan and tilt angles to a safe default value.
	/// This control is specific to Logitech cameras that support panning and
	/// tilting such as the QuickCam Orbit series.
	CC_LOGITECH_PANTILT_RESET,
	// Illumination mode of the first LED.
	CC_LOGITECH_LED1_MODE,
	// Blinking frequency of the first LED.
	CC_LOGITECH_LED1_FREQUENCY,
	// Disable video processing (enable raw mode)
	CC_LOGITECH_DISABLE_PROCESSING,
	// Bits per pixel for raw (Bayer) mode
	CC_LOGITECH_RAW_BITS_PER_PIXEL,


} CControlId;


/**
 * Flags specifying control properties and capabilities.
 */
typedef enum _CControlFlags {
	/// The control can be read, i.e. its current value can be queried.
	/// This flag is usually not set for action controls.
	CC_CAN_READ				= 1 << 0,
	/// The control can be changed, i.e. it can be set to a new value.
	/// Note that this flag does not imply CC_CAN_READ. Action controls accept
	/// values but cannot necessarily be queried.
	CC_CAN_WRITE			= 1 << 1,
	/// The control supports value change notification.
	CC_CAN_NOTIFY			= 1 << 2,

	// The control is a custom vendor control and not standardized in V4L2 or UVC.
	CC_IS_CUSTOM			= 1 << 8,
	// Set values are interpreted as being relative to the current value.
	CC_IS_RELATIVE			= 1 << 9,
	// The control triggers an action.
	CC_IS_ACTION			= 1 << 10,

} CControlFlags;


/**
 * Types for controls.
 * These types give both hints to graphical user interfaces on how to display
 * the control to the user as well as to the implementation in terms of data
 * length required to store control values.
 */
typedef enum _CControlType {
	/// The control uses raw binary data.
	/// Usually (but not necessarily) this includes values that cannot be easily
	/// mapped to simple data types.
	CC_TYPE_RAW				= 1,
	/// The control knows only one enabled/disabled state.
	/// Controls of this type are typically displayed as checkboxes.
	CC_TYPE_BOOLEAN,
	/// The control accepts a defined list of values.
	/// Controls of this type are typically displayed as combo boxes or lists.
	CC_TYPE_CHOICE,
	/// The control accepts 8-bit integer values.
	CC_TYPE_BYTE,
	/// The control accepts 16-bit integer values.
	CC_TYPE_WORD,
	/// The control accepts 32-bit integer values.
	CC_TYPE_DWORD,

} CControlType;


/**
 * Event identifiers.
 */
typedef enum _CEventId {
	CE_CONTROL_INVALID		= 0,

} CEventId;


/**
 * Flags specifying properties of events.
 */
typedef enum _CEventFlags {
	CE_INVALID				= 0,

} CEventFlags;


/**
 * Types of frame size specifications.
 */
typedef enum _CFrameSizeTypes {
	/// One discrete width/height combination.
	CF_SIZE_DISCRETE		= 1,
	/// Minimum and maximum values for both width and height.
	CF_SIZE_CONTINUOUS,
	/// Minimum, maximum, and step values for both width and height.
	CF_SIZE_STEPWISE,

} CFrameSizeTypes;


/**
 * Types of frame interval specification.
 */
typedef enum _CFrameIntervalTypes {
	/// One discrete time interval.
	CF_INTERVAL_DISCRETE		= 1,
	/// Minimum and maximum values for the time interval.
	CF_INTERVAL_CONTINUOUS,
	/// Minimum, maximum, and step values for the time interval.
	CF_INTERVAL_STEPWISE,

} CFrameIntervalTypes;


/**
 * Flags specifying control properties and capabilities.
 */
typedef enum _CDynctrlFlags {
	/// Disable XML schema validation.
	CD_DONT_VALIDATE		= 1 << 0,
	/// Collect errors and warnings during the process
	CD_REPORT_ERRORS		= 1 << 1,
	/// Return meta data during the XML parsing process
	CD_RETRIEVE_META_INFO	= 1 << 2,

} CDynctrlFlags;


/**
 * Severity levels for messages returned by the dynamic control functions
 */
typedef enum _CDynctrlMessageSeverity {
	CD_SEVERITY_ERROR		= 2,		///< Error
	CD_SEVERITY_WARNING		= 3,		///< Warning
	CD_SEVERITY_INFO		= 4,		///< Informational

} CDynctrlMessageSeverity;



/*
 * Structs
 */

/**
 * USB specific information about a device.
 */
typedef struct _CUSBInfo {
	/// The vendor ID.
	unsigned short	vendor;
	/// The product ID.
	unsigned short	product;
	/// The product revision number.
	unsigned short	release;

} CUSBInfo;


/**
 * Webcam device information.
 */
typedef struct _CDevice {
	/// A unique short name.
	/// For V4L2 devices this is the name of the device file. For example, if
	/// a device appears as '/dev/video0', the short name is 'video0'.
	/// This member is never NULL.
	char			* shortName;
	
	/// The human-readable name of the device.
	/// This member is never NULL.
	char			* name;

	/// The name of the driver providing the camera interface.
	/// This member is never NULL.
	char			* driver;

	/// The location of the camera.
	/// This string is dependent on the implementation that provides the device.
	/// It could either be a string provided by the driver or a device name.
	/// This member is never NULL.
	char			* location;

	/// USB related information about the camera.
	CUSBInfo		usb;

} CDevice;


/**
 * A raw control value.
 */
typedef struct _CControlRawValue {
	/// Pointer to the raw data.
	void			* data;
	/// Size of the raw data.
	unsigned int	size;

} CControlRawValue;


/**
 * A value of a control.
 */
typedef struct _CControlValue {
	/// The type of the control to which the value belongs.
	/// This member is used to determine which of the members of the union are
	/// valid. The type is a duplicate of the corresponding CControl#type value.
	CControlType	type;

	union {
		/// The value of the control for number based controls.
		/// This member is valid for the following control types:
		/// #CC_TYPE_BOOLEAN, #CC_TYPE_CHOICE, #CC_TYPE_BYTE,
		/// #CC_TYPE_WORD, #CC_TYPE_DWORD
		int				value;

		/// The value of the control for raw cntrols.
		/// This member is valid only for control type #CC_TYPE_RAW.
		CControlRawValue	raw;
	};

} CControlValue;


/**
 * A list of choices for a choice type of control.
 * @see CC_TYPE_CHOICE
 */
typedef struct _CControlChoice {
	/// The index of the choice within the list of choices.
	/// Note that this index is not necessarily zero-based and can therefore @a not
	/// be used as an index into CControl#choices#list[].
	int				index;

	/// The name of the choice.
	/// This member is never NULL.
	char			* name;

} CControlChoice;


/**
 * A description of a camera control.
 */
typedef struct _CControl {
	/// The identifier of the control
	CControlId		id;
	/// The human-readable name of the control
	char			* name;
	/// The type of the control
	CControlType	type;
	/// Flags describing properties and capabilities of the control
	CControlFlags	flags;

	/// The current value of the control
	CControlValue	value;
	/// The default value of the control
	CControlValue	def;

	union {
		/// Additional control attributes for non-choice controls.
		/// The members of this struct are only valid for control types other
		/// than #CC_TYPE_CHOICE.
		struct {
			/// The minimum value of the control
			CControlValue	min;
			/// The maximum value of the control
			CControlValue	max;
			/// The resolution of the control values
			CControlValue	step;
		};

		/// List of the possible choices that this control allows.
		/// The members of this struct are only valid if the type is #CC_TYPE_CHOICE.
		struct {
			/// The number of choices in the list
			unsigned int	count;
			/// The list of choice values
			CControlChoice	* list;
			
			/// A buffer that contains the names of the #CControlChoice items.
			/// The @a name fields of the items point to strings within this buffer,
			/// so there is no need to work with this member directly except
			/// freeing the buffer if the control is disposed of.
			char			* names;

		} choices;
	};

} CControl;


/**
 * A description of a pixel format.
 */
typedef struct _CPixelFormat {
	/// A FourCC code identifying the pixel format.
	/// Note that this member contains an extra byte which will contain a \\0
	/// character when returned by c_enum_pixel_formats(). This allows the field
	/// to be treated like a NUL terminated string.
	char			fourcc[5];

	/// A human-readable description of the pixel format.
	char			* name;

	/// The MIME type corresponding to the pixel format.
	/// This member can be NULL.
	char			* mimeType;

} CPixelFormat;


/**
 * A description of one or multiple frame sizes.
 */
typedef struct _CFrameSize {
	/// Type of the frame size(s) described (discrete, continuous, step-wise)
	CFrameSizeTypes	type;
	
	union {
		/// Discrete frame size.
		/// This member is only valid if @a type is #CF_SIZE_DISCRETE.
		struct {
			/// Width
			unsigned int	width;
			/// Height
			unsigned int	height;
		};

		/// Continuous or step-wise frame size.
		/// This member is only valid if @a type is #CF_SIZE_CONTINUOUS or
		/// #CF_SIZE_STEPWISE.
		struct {
			/// Minimum width
			unsigned int	min_width;
			/// Maximum width
			unsigned int	max_width;
			/// Step size for the width (usually 1 if @a type is #CF_SIZE_CONTINUOUS)
			unsigned int	step_width;
			/// Minimum height
			unsigned int	min_height;
			/// Maximum height
			unsigned int	max_height;
			/// Step size for the height (usually 1 if @a type is #CF_SIZE_CONTINUOUS)
			unsigned int	step_height;
		};
	};

} CFrameSize;


/**
 * A description of one or multiple frame intervals.
 * Each frame interval is described as a fraction of a second.
 */
typedef struct _CFrameInterval {
	/// Type of the frame interval(s) described (discrete, continuous, step-wise)
	CFrameIntervalTypes	type;

	union {
		/// Discrete frame interval.
		/// This member is only valid if @a type is #CF_INTERVAL_DISCRETE.
		struct {
			/// Numerator of the frame interval
			unsigned int	n;
			/// Denominator of the frame interval
			unsigned int	d;
		};

		/// Continuous or step-wise frame interval.
		/// This member is only valid if @a type is #CF_INTERVAL_CONTINUOUS or
		/// #CF_INTERVAL_STEPWISE.
		struct {
			/// Numerator of the minimum frame interval
			unsigned int	min_n;
			/// Denominator of the minimum frame interval
			unsigned int	min_d;
			/// Numerator of the maximum frame interval
			unsigned int	max_n;
			/// Denominator of the maximum frame interval
			unsigned int	max_d;
			/// Numerator of the frame interval step size (usually 1 if @a type is #CF_INTERVAL_CONTINUOUS
			unsigned int	step_n;
			/// Denominator of the frame interval step size (usually 1 if @a type is #CF_INTERVAL_CONTINUOUS
			unsigned int	step_d;
		};
	};

} CFrameInterval;


/**
 * An event descriptor.
 */
typedef struct _CEvent {
	/// Unique identifier of the event
	CEventId		id;

	/// Human-readable name of the event
	char			* name;

	/// Flags descripting properties of the event
	CEventFlags		flags;

} CEvent;


/**
 * Message returned by the dynamic control configuration parser.
 */
typedef struct _CDynctrlMessage {
	/// Line number that the message concerns
	int				line;

	/// Column that the message concerns
	int				col;

	/// Severity level
	CDynctrlMessageSeverity	severity;

	/// Pointer to the message text
	char			* text;

} CDynctrlMessage;


/**
 * Version number in the "major.minor" format.
 */
typedef struct _CVersionNumber {
	/// Major version number
	unsigned int	major;

	/// Minor version number
	unsigned int	minor;

} CVersionNumber;


/**
 * Statistics on the number of list elements that were processed.
 */
typedef struct _CDynctrlInfoListStats {
	/// Number of elements in the list that were processed successfully
	unsigned int	successful;

	/// Number of elements in the list that could not be processed successfully
	unsigned int	failed;

} CDynctrlInfoListStats;


/**
 * Input flags and output data used/returned by the control configuration parser.
 */
typedef struct _CDynctrlInfo {
	/// Flags controlling operations related to dynamic controls
	CDynctrlFlags	flags;

	/// Processing statistics
	struct {
		/// Statistics on the constants that were processed
		CDynctrlInfoListStats	constants;

		/// Statistics on the controls that were processed
		CDynctrlInfoListStats	controls;

		/// Statistics on the mappings that were processed
		CDynctrlInfoListStats	mappings;

	}				stats;

	/// Meta information about the XML control configuration
	struct {
		/// File format version
		CVersionNumber	version;

		/// Name of the configuration author
		char			* author;

		/// Contact information of the configuration author (e.g. e-mail address)
		char			* contact;

		/// Copyright information about the configuration file
		char			* copyright;

		/// Revision number of the configuration file (configuration version)
		CVersionNumber	revision;

	}				meta;

	/// Number of messages in the @a messages array
	unsigned int	message_count;

	/// Pointer to the array with the messages concerning the operation
	CDynctrlMessage	* messages;

} CDynctrlInfo;



/*
 * Type definitions
 */

/**
 * Prototype for event handlers
 */
typedef void (*CEventHandler)(CHandle hDevice, CEventId event_id, void *context);



/*
 * Functions
 */

#ifdef __cplusplus
extern "C" {
#endif

extern CResult		c_init (void);
extern void			c_cleanup (void);

extern CHandle		c_open_device (const char *device_name);
extern void			c_close_device (CHandle hDevice);

extern CResult		c_enum_devices (CDevice *devices, unsigned int *size, unsigned int *count);
extern CResult		c_get_device_info (CHandle hDevice, const char *device_name, CDevice *info, unsigned int *size);

extern CResult		c_enum_pixel_formats (CHandle hDevice, CPixelFormat *formats, unsigned int *size, unsigned int *count);
extern CResult		c_enum_frame_sizes (CHandle hDevice, const CPixelFormat *pixelformat, CFrameSize *sizes, unsigned int *size, unsigned int *count);
extern CResult		c_enum_frame_intervals (CHandle hDevice, const CPixelFormat *pixelformat, const CFrameSize *framesize, CFrameInterval *intervals, unsigned int *size, unsigned int *count);

extern CResult		c_enum_controls (CHandle hDevice, CControl *controls, unsigned int *size, unsigned int *count);
extern CResult		c_set_control (CHandle hDevice, CControlId control_id, const CControlValue *value);
extern CResult		c_get_control (CHandle hDevice, CControlId control_id, CControlValue *value);

extern CResult		c_enum_events (CHandle hDevice, CEvent *events, unsigned int *size, unsigned int *count);
extern CResult		c_subscribe_event (CHandle hDevice, CEventId event_id, CEventHandler handler, void *context);
extern CResult		c_unsubscribe_event (CHandle hDevice, CEventId event_id);

#ifndef DISABLE_UVCVIDEO_DYNCTRL
extern CResult		c_add_control_mappings_from_file (const char *file_name, CDynctrlInfo *info);
#endif

extern char			*c_get_error_text (CResult error);
extern char			*c_get_handle_error_text (CHandle hDevice, CResult error);

#ifdef __cplusplus
}
#endif


#endif /* C_WEBCAM_H */
