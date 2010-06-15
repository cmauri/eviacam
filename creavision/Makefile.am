noinst_HEADERS = crvimage.h crvskindetection.h crvcamera.h \
	crvcamera_cv.h crvcolor.h crvgeomtry.h \
	crvmisc.h crvhistogram.h crvnormroi.h
	
dist_noinst_DATA = crvcamera_wdm.cpp videoInput.cpp \
	fixvfwfps/fixvfwfps.vcproj  fixvfwfps/main.cpp \
	CreaVision_eviacam.vcproj crvcamera_wdm.h videoInput.h

libcreavision_la_LIBADD = $(OPENCV_LIBS)
	
noinst_LTLIBRARIES = libcreavision.la

libcreavision_la_SOURCES = crvcamera.cpp crvcamera_cv.cpp\
	crvcolor.cpp crvgeomtry.cpp crvhistogram.cpp \
	crvimage.cpp crvmisc.cpp crvskindetection.cpp \
	crvnormroi.cpp
