noinst_HEADERS = \
	waittime.h \
	mousecontrol.h \
	wxappbar.h \
	configbase.h \
	configmanager.h \
	timeutil.h \
	warnbaroverlap.h

AM_CPPFLAGS = -I$(top_srcdir)/simplelog
METASOURCES = AUTO

noinst_LTLIBRARIES = libwxutil.la

libwxutil_la_LDFLAGS = $(all_libraries)

libwxutil_la_LIBADD = $(GTK_LIBS)

libwxutil_la_SOURCES = \
	wxappbar.cpp \
	mousecontrol.cpp \
	waittime.cpp \
	configbase.cpp \
	configmanager.cpp \
	timeutil.cpp \
	warnbaroverlap.cpp
