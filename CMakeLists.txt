cmake_minimum_required(VERSION 2.6)
project(libwebcam)

IF(NOT CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE (FATAL_ERROR "libwebcam is only available for linux systems")
ENDIF(NOT CMAKE_SYSTEM_NAME MATCHES "Linux")

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
