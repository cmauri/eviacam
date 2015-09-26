SUBDIRS = libwebcam

noinst_HEADERS = crvimage.h crvskindetection.h crvcamera.h \
	crvcamera_cv.h crvcolor.h crvgeomtry.h pwc-ioctl.h \
	crvmisc.h crvhistogram.h crvnormroi.h crvcamera_v4l2.h \
	crvcamera_enum.h colorspaces.h defs.h
	
libcreavision_la_LIBADD = $(OPENCV_LIBS) -lv4l2

noinst_LTLIBRARIES = libcreavision.la

AM_CPPFLAGS = -I$(top_builddir)/creavision/libwebcam -Ilibwebcam -I$(top_srcdir)/simplelog

libcreavision_la_SOURCES = crvcamera.cpp crvcamera_cv.cpp crvcamera_v4l2.cpp \
	crvcolor.cpp crvgeomtry.cpp crvhistogram.cpp \
	crvimage.cpp crvmisc.cpp crvskindetection.cpp \
	crvnormroi.cpp crvcamera_enum.cpp colorspaces.cpp
