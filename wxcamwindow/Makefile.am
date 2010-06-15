noinst_HEADERS = camwindow.h capturethread.h visiblenormroi.h wxnormroi.h

dist_noinst_DATA = wxcamwindow.vcproj

noinst_LTLIBRARIES = libwxcamwindow.la

libwxcamwindow_la_SOURCES = camwindow.cpp capturethread.cpp visiblenormroi.cpp \
	wxnormroi.cpp

INCLUDES = -I$(top_srcdir)/creavision

libwxcamwindow_la_LIBADD = $(top_builddir)/creavision/libcreavision.la
