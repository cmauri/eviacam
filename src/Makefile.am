# these are the headers for your project
noinst_HEADERS = wgetkey.h cautostart.h ckeyboardcode.h clickwindow.h \
	clickwindowbitmap.h clickwindowcontroller.h clickwindowtext.h \
	cmotioncalibration.h eviacamapp.h langnames-utf8.h motiontracker.h \
	mouseoutput.h version.h viacamcontroller.h wabout.h \
	wconfiguration.h wconfirmcalibration.h wmotioncalibrationx.h \
	wmotioncalibrationy.h wviacam.h cvisualalert.h wwizard.h \
	wwizardmanager.h wcameradialog.h

dist_noinst_DATA= version.h.in eviacamloader.c.in langnames-utf16.h gui.pjd gui.rc eviacam.vcproj \
	icons/on.xpm icons/eviacam.ico  icons/eviacam_mini.ico  \
	icons/clickwindowoff.xpm  icons/help.xpm icons/preferences.xpm \
	icons/clickwindow.xpm icons/keyboard.xpm icons/user.xpm \
	icons/eviacam_mini.xpm icons/off.xpm \
	icons/buttonDblLeftDisabled.xpm icons/buttonDblLeftLocked.xpm \
	icons/buttonDblLeftSelectedLocked.xpm icons/buttonDblLeftSelected.xpm \
	icons/buttonDblLeft.xpm icons/buttonDragDisabled.xpm icons/buttonDragLocked.xpm \
	icons/buttonDragSelectedLocked.xpm icons/buttonDragSelected.xpm icons/buttonDrag.xpm \
	icons/buttonHideMainWindow.xpm icons/buttonLeftDisabled.xpm icons/buttonLeftLocked.xpm \
	icons/buttonLeftSelectedLocked.xpm icons/buttonLeftSelected.xpm icons/buttonLeft.xpm \
	icons/buttonNoClickSelected.xpm icons/buttonNoClick.xpm \
	icons/buttonRightDisabled.xpm icons/buttonRightLocked.xpm icons/buttonRightSelectedLocked.xpm \
	icons/buttonRightSelected.xpm icons/buttonRight.xpm icons/buttonShowMainWindow.xpm \
	icons/usermoveh64.xpm icons/usermovev64.xpm

EXTRA_DIST = eviacam.desktop icons/eviacam.xpm
dist_pkgdata_DATA= eviacam.desktop
	
desktopdir		=	$(datadir)/applications/
desktop_DATA	=	eviacam.desktop

pixmapdir		=	$(datadir)/pixmaps/
pixmap_DATA	=	icons/eviacam.xpm

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = eviacam eviacamloader

eviacamloader_SOURCES = eviacamloader.c
install-exec-hook:
	chmod u+s $(DESTDIR)$(bindir)/eviacamloader


# the application source, library search path, and link libraries
eviacam_SOURCES = wgetkey.cpp cautostart.cpp ckeyboardcode.cpp clickwindow.cpp \
	clickwindowbitmap.cpp clickwindowcontroller.cpp clickwindowtext.cpp cmotioncalibration.cpp \
	eviacamapp.cpp motiontracker.cpp mouseoutput.cpp \
	viacamcontroller.cpp wabout.cpp wconfiguration.cpp wconfirmcalibration.cpp \
	wmotioncalibrationx.cpp wmotioncalibrationy.cpp wviacam.cpp cvisualalert.cpp wwizard.cpp wwizardmanager.cpp wcameradialog.cpp
	
eviacam_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
eviacam_LDFLAGS = $(WX_LIBS) $(OPENCV_LIBS) -lXtst -lXext -lX11

INCLUDES = -I$(top_srcdir)/creavision \
	-I$(top_srcdir)/wxcamwindow -I$(top_srcdir)/wxutil

eviacam_LDADD = $(top_builddir)/wxutil/libwxutil.la \
		$(top_builddir)/creavision/libcreavision.la \
		$(top_builddir)/libwebcam/libwebcam.la \
		$(top_builddir)/wxcamwindow/libwxcamwindow.la
