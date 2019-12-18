SUBDIRS = libwebcam

noinst_HEADERS = crvimage.h crvcamera.h colorspaces.h crvcamera_cv.h pwc-ioctl.h \
	crvnormroi.h crvcamera_v4l2.h crvcamera_enum.h defs.h
	
libcreavision_la_LIBADD = $(OPENCV_LIBS) -lv4l2

noinst_LTLIBRARIES = libcreavision.la

AM_CPPFLAGS = -I$(top_builddir)/creavision/libwebcam -Ilibwebcam -I$(top_srcdir)/simplelog

libcreavision_la_SOURCES = crvcamera.cpp crvcamera_cv.cpp crvcamera_v4l2.cpp \
	crvimage.cpp colorspaces.cpp crvnormroi.cpp crvcamera_enum.cpp
