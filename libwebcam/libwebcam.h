/**
 * \file
 * Webcam library declarations.
 *
 * \ingroup libwebcam
 */

/*
 * Copyright (c) 2006-2008 Logitech.
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

#ifndef C_LIBWEBCAM_H
#define C_LIBWEBCAM_H


#include <assert.h>


/*
 * Features
 */

/// Whether private controls of the Linux UVC driver should be used or not
#define	USE_UVCVIDEO
#ifdef USE_UVCVIDEO

	/// Whether to compile in support functions for the Linux UVC driver's dynamic
	/// controls.
	#define	ENABLE_UVCVIDEO_DYNCTRL
	#ifdef	ENABLE_UVCVIDEO_DYNCTRL

		/// Whether to include the automatically generated Logitech dynamic controls
		/// header file.
		#define USE_LOGITECH_DYNCTRL

		/// Whether to include support for raw controls.
		/// Note that this requires V4L2_CTRL_TYPE_STRING to be available (i.e. V4L2 >= 2.6.32).
		/// This is disabled by default but CMake enables it if it finds V4L2_CTRL_TYPE_STRING.
		//#define ENABLE_RAW_CONTROLS

	#endif

#endif


/*
 * Constants
 */

/// Whether to use the V4L2_CTRL_FLAG_NEXT_CTRL flag when enumerating V4L2 controls
#define	ENABLE_V4L2_ADVANCED_CONTROL_ENUMERATION

/// Ignore EEXIST errors for the UVCIOC_CTRL_ADD and UVCIOC_CTRL_MAP ioctls for
/// all but the first device. This is required if the driver uses global controls
/// instead of per-device controls.
#define DYNCTRL_IGNORE_EEXIST_AFTER_PASS1

/// The maximum number (plus 1) of handles libwebcam supports
#define	MAX_HANDLES						32

/// Debug option to disable locking
#define	DISABLE_LOCKING					1
/// Debug option to add verbosity to locking and unlocking
#define	DEBUG_LOCKING					1

/// The name used for controls whose name could not be retrieved.
#define	UNKNOWN_CONTROL_NAME			"Unknown control"
/// Number of retries for failed V4L2 ioctl requests.
/// This is a workaround for faulty devices.
#define	CONTROL_IO_ERROR_RETRIES		2

/// Size of a GUID in bytes
#define GUID_SIZE		16



/*
 * Macros
 */

/// Returns the given handle structure
#define GET_HANDLE(handle)		(handle_list.handles[(handle)])
/// Returns true if the given handle is open (valid or invalid)
#define HANDLE_OPEN(handle)		((handle) < MAX_HANDLES && GET_HANDLE(handle).open)
/// Returns true if the given handle is open and valid
#define HANDLE_VALID(handle)	(HANDLE_OPEN(handle) && GET_HANDLE(handle).device)

/// Returns the maximum number of characters that a menu-type control choice
/// can have in V4L2.
#define V4L2_MENU_CTRL_MAX_NAME_SIZE	sizeof(((struct v4l2_querymenu *)NULL)->name)

/// Converts the given characters into a FourCC code
#define MAKE_FOURCC(c1,c2,c3,c4) \
	(unsigned int)((long)c1 | (long)c2 << 8 | (long)c3 << 16 | (long)c4 << 24)

/// Format string to print a GUID byte array with printf
#define GUID_FORMAT		"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x"
/// Argument macro to print a GUID byte array with printf
#define GUID_ARGS(guid) \
        (guid)[3],  (guid)[2],  (guid)[1],  (guid)[0], \
        (guid)[5],  (guid)[4], \
        (guid)[7],  (guid)[6], \
        (guid)[8],  (guid)[9], \
        (guid)[10], (guid)[11], (guid)[12], \
        (guid)[13], (guid)[14], (guid)[15]



/*
 * Structures
 */

/**
 * An internal control description associated with a device.
 */
typedef struct _Control {
	/// Control description
	CControl		control;
	/// V4L2 ioctl mapping (non-0 for V4L2 controls)
	int				v4l2_control;
	/// Pointer to the next control in the list
	struct _Control	* next;

} Control;

/**
 * Base structure that contains a list of controls and associated data.
 */
typedef struct _ControlList {
	/// The first control in the list
	Control			* first;
	/// The mutex used to serialize access to the control list
	pthread_mutex_t mutex;
	/// The number of controls contained in the list
	int				count;

} ControlList;

/**
 * Internal device information.
 */
typedef struct _Device {
	/// Device information
	CDevice			device;
	/// Short V4L2 device name (e.g. 'video0')
	char			v4l2_name[NAME_MAX];
	/// Number of handles associated with this device
	int				handles;
	/// List of controls supported by this device
	ControlList		controls;
	/// Boolean whether the device is still valid, i.e. exists in the system.
	/// Devices marked as invalid will be cleared out by cleanup_device_list().
	int				valid;
	/// Next device in the global device list
	struct _Device	* next;

} Device;

/**
 * Base structure that contains a list of devices and associated data.
 */
typedef struct _DeviceList {
	/// The first device in the list
	Device			* first;
	/// The mutex used to serialize access to the device list
	pthread_mutex_t mutex;
	/// The number of devices contained in the list
	int				count;

} DeviceList;

/**
 * Information associated with a device handle.
 *
 * Note that a handle can have three different states:
 * - Closed/free: The device handle is not being used by an application.
 * - Valid: The device handle is open by an application and points to a valid device.
 * - Invalid: The device handle is open by an application but does not point to a
 *     (valid) device anymore. This happens when a device is removed from the system
 *     but there are still open handles around.
 * An "open" handle can be both valid or invalid.
 */
typedef struct _Handle {
	/// Pointer to the device structure associated with this handle
	Device			* device;
	/// True if the handle is open (valid or invalid)
	int				open;
	/// The number of the last system error (e.g. a V4L2 return value)
	int				last_system_error;
	
} Handle;

/**
 * Base structure that contains an array of device handles and associated data.
 */
typedef struct _HandleList {
	/// The array of handles
	Handle			handles[MAX_HANDLES];
	/// The mutex used to serialize access to the handle list
	pthread_mutex_t	mutex;
	/// The index of the first free handle.
	/// Zero if there are now free handles at this time.
	int				first_free;
	
} HandleList;



/*
 * Globals
 */

extern int initialized;
extern HandleList handle_list;

extern void print_error (char *format, ...);
extern int open_v4l2_device(char *device_name);



/*
 * Helper functions
 */

/**
 * Acquire a mutex.
 * 
 * This function is identical to pthread_mutex_lock except that it has different
 * return values and supports some debug flags.
 *
 * @return
 * 		C_SUCCESS if the mutex was successfully acquired
 * 		C_SYNC_ERROR if an error occured while trying to acquire the mutex
 */
static inline CResult lock_mutex (pthread_mutex_t *mutex)
{
#ifndef DISABLE_LOCKING
#ifdef DEBUG_LOCKING
	fprintf(stderr, "Acquiring mutex 0x%08x ...\n", (unsigned int)mutex);
#endif
	int ret = pthread_mutex_lock(mutex);
#ifdef DEBUG_LOCKING
	fprintf(stderr, "Acquisition of mutex 0x%08x %s.\n", (unsigned int)mutex,
			ret ? "failed" : "successful");
#endif
	assert(ret == 0);
	return ret ? C_SYNC_ERROR : C_SUCCESS;
#else
	return C_SUCCESS;
#endif
}


/**
 * Release a mutex.
 * 
 * This function is identical to pthread_mutex_unlock except that it has different
 * return values and supports some debug flags.
 *
 * @return
 * 		C_SUCCESS if the mutex was successfully released
 * 		C_SYNC_ERROR if an error occured while trying to release the mutex
 */
static inline void unlock_mutex (pthread_mutex_t *mutex)
{
#ifndef DISABLE_LOCKING
#ifdef DEBUG_LOCKING
	fprintf(stderr, "Releasing mutex 0x%08x ...\n", (unsigned int)mutex);
#endif
	int ret = pthread_mutex_unlock(mutex);
#ifdef DEBUG_LOCKING
	fprintf(stderr, "Release of mutex 0x%08x %s.\n", (unsigned int)mutex,
			ret ? "failed" : "successful");
#endif
	assert(ret == 0);
#endif
}


/**
 * Copies a variable-length string to the part of an enumeration buffer that is
 * reserved for dynamic data.
 *
 * This function is used by the enumeration functions.
 */
static inline void copy_string_to_buffer (char **target, char *source, void *buffer, unsigned int *offset)
{
	unsigned int length = strlen(source);
	*target = (char *)buffer + *offset;
	memcpy(*target, source, length + 1);
	*offset += length + 1;
}


#endif /* C_LIBWEBCAM_H */
