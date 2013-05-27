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
INCLUDE_DIRECTORIES (libwebcam)
add_subdirectory(libwebcam)
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
#	set (OpenCV_DIR "/usr")
endif(UNIX)

FIND_PACKAGE( OpenCV REQUIRED core video highgui imgproc)

if(WIN32)
	if (MSVC)
		configure_file("${OpenCV_DIR}/bin/Debug/opencv_core220d.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Debug COPYONLY)
		configure_file("${OpenCV_DIR}/bin/opencv_core220.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Release COPYONLY)
		
		configure_file("${OpenCV_DIR}/bin/Debug/opencv_highgui220d.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Debug COPYONLY)
		configure_file("${OpenCV_DIR}/bin/opencv_highgui220.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Release COPYONLY)
		
		configure_file("${OpenCV_DIR}/bin/Debug/opencv_imgproc220d.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Debug COPYONLY)
		configure_file("${OpenCV_DIR}/bin/opencv_imgproc220.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Release COPYONLY)
		
		configure_file("${OpenCV_DIR}/bin/Debug/opencv_video220d.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Debug COPYONLY)
		configure_file("${OpenCV_DIR}/bin/opencv_video220.dll" ${RUNTIME_OUTPUT_DIRECTORY}/Release COPYONLY)
	endif(MSVC)

	install (FILES 
		"${OpenCV_DIR}/bin/opencv_core220.dll"
		"${OpenCV_DIR}/bin/opencv_highgui220.dll"
		"${OpenCV_DIR}/bin/opencv_imgproc220.dll"
		"${OpenCV_DIR}/bin/opencv_video220.dll"
		DESTINATION ${LIBRUNTIMEDIR}
	)
endif(WIN32)

add_library (creavision STATIC ${creavision_SRCS})
TARGET_LINK_LIBRARIES(creavision ${OpenCV_LIBS} )
TARGET_LINK_LIBRARIES(creavision simplelog)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
	TARGET_LINK_LIBRARIES(creavision libwebcam v4l2)
ELSEIF(WIN32)
	TARGET_LINK_LIBRARIES(creavision ole32.lib strmiids.lib uuid.lib dxguid.lib)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
