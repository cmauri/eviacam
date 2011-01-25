cmake_minimum_required(VERSION 2.6)
project(creavision)

set(creavision_SRCS
	crvcamera.h
	crvcamera.cpp
	crvcamera_enum.h
	crvcamera_enum.cpp	
	crvcolor.cpp
	crvcolor.h
	crvgeomtry.cpp
	crvgeomtry.h
	crvhistogram.cpp
	crvhistogram.h
	crvimage.cpp
	crvimage.h
	crvmisc.cpp
	crvmisc.h
	crvnormroi.cpp
	crvnormroi.h
	crvskindetection.cpp
	crvskindetection.h
)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
FIND_PACKAGE( GLIB2 REQUIRED )
INCLUDE_DIRECTORIES (${GLIB2_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (../libwebcam)
set(creavision_SRCS ${creavision_SRCS}
	crvcamera_v4l2.cpp
	crvcamera_v4l2.h
	colorspaces.cpp
	colorspaces.h
	pwc-ioctl.h
	defs.h
)
ELSEIF(WIN32)
set(creavision_SRCS ${creavision_SRCS}
	crvcamera_wdm.cpp
	crvcamera_wdm.h
	videoInput.cpp
	videoInput.h
)
ELSE(CMAKE_SYSTEM_NAME MATCHES "Linux")
set(creavision_SRCS ${creavision_SRCS}
	crvcamera_cv.cpp
	crvcamera_cv.h
)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(UNIX)
	# For UNIX platforms we assume OpenCV is under /usr which 
	# would not be always true
	set (OpenCV_DIR "/usr")
endif(UNIX)

FIND_PACKAGE( OpenCV REQUIRED )

message(STATUS "OpenCV_FOUND. ${OpenCV_FOUND}")
message(STATUS "OpenCV_LIBS. ${OpenCV_LIBS}")
message(STATUS "OpenCV_INCLUDE_DIR. ${OpenCV_INCLUDE_DIR}")

add_library (creavision SHARED ${creavision_SRCS})
TARGET_LINK_LIBRARIES(creavision ${OpenCV_LIBS} )
if(WIN32)
	TARGET_LINK_LIBRARIES(creavision ole32.lib strmiids.lib uuid.lib dxguid.lib)
endif(WIN32)
