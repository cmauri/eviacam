cmake_minimum_required(VERSION 2.6)
project(libwebcam)

IF(NOT CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE (FATAL_ERROR "libwebcam is only available for linux systems")
ENDIF(NOT CMAKE_SYSTEM_NAME MATCHES "Linux")

find_file(VIDEODEV_INCLUDE_FILE
	videodev.h 
	/usr/include/linux
)

if (NOT VIDEODEV_INCLUDE_FILE)
find_file(VIDEODEV_INCLUDE_FILE
	libv4l1-videodev.h
	/usr/include/
)
endif()

if (NOT VIDEODEV_INCLUDE_FILE)
	message (FATAL_ERROR "Cannot find videodev.h nor libv4l1-videodev.h")
endif()

configure_file ("incvideodev.h.in" "${CMAKE_BINARY_DIR}/include/incvideodev.h" )

set(libwebcam_SRCS
	compat.h
	dynctrl-logitech.h
	libwebcam.c
	libwebcam.h
	uvcvideo.h
	webcam.h
)

add_library (libwebcam STATIC ${libwebcam_SRCS})
#TARGET_LINK_LIBRARIES(creavision ${OpenCV_LIBS} )
