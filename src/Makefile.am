# these are the headers for your project
noinst_HEADERS = wgetkey.h \
	cautostart.h \
	keyboardcode.h \
	clickwindow.h \
	clickwindowbitmap.h \
	clickwindowcontroller.h \
	clickwindowtext.h \
	cmotioncalibration.h \
	eviacamapp.h \
	langnames-utf8.h \
	visionpipeline.h \
	viacamcontroller.h \
	wabout.h \
	wconfiguration.h \
	wconfirmcalibration.h \
	wmotioncalibrationx.h \
	wmotioncalibrationy.h \
	wviacam.h \
	cvisualalert.h \
	wwizard.h \
	wwizardmanager.h \
	wcameradialog.h \
	eviacamdefs.h \
	hotkeymanager.h \
	dwellclick.h \
	gestureclick.h \
	pointeraction.h \
	mousecommand.h \
	paths.h \
	checkupdates.h \
	checkupdates_ui.h \
	website.h \
	newtrackerinformationdlg.h \
	checkupdates_manager.h

dist_noinst_DATA= \
	eviacamloader.c.in \
	gui.pjd \
	gui.rc \
	resources/eviacam.ico

nobase_dist_pkgdata_DATA = \
	resources/clickWindowBitmap.xrc \
	resources/clickWindowTextVertical.xrc \
	resources/clickWindowBitmapVertical.xrc \
	resources/clickWindowText.xrc \
	resources/clickwindowoff.xpm \
	resources/help.xpm \
	resources/preferences.xpm \
	resources/clickwindow.xpm \
	resources/keyboard.xpm \
	resources/user.xpm \
	resources/eviacam_gray_bg.xpm \
	resources/eviacam_mini.xpm \
	resources/eviacam_small.xpm \
	resources/on.xpm \
	resources/off.xpm \
	resources/buttonDblLeftDisabled.xpm \
	resources/buttonDblLeftLocked.xpm \
	resources/buttonDblLeftSelectedLocked.xpm \
	resources/buttonDblLeftSelected.xpm \
	resources/buttonDblLeft.xpm \
	resources/buttonDragDisabled.xpm \
	resources/buttonDragLocked.xpm \
	resources/buttonDragSelectedLocked.xpm \
	resources/buttonDragSelected.xpm \
	resources/buttonDrag.xpm \
	resources/buttonHideMainWindow.xpm \
	resources/buttonLeftDisabled.xpm \
	resources/buttonLeftLocked.xpm \
	resources/buttonLeftSelectedLocked.xpm \
	resources/buttonLeftSelected.xpm \
	resources/buttonLeft.xpm \
	resources/buttonMiddleDisabled.xpm \
	resources/buttonMiddleLocked.xpm \
	resources/buttonMiddleSelectedLocked.xpm \
	resources/buttonMiddleSelected.xpm \
	resources/buttonMiddle.xpm \
	resources/buttonNoClickSelected.xpm \
	resources/buttonNoClick.xpm \
	resources/buttonRightDisabled.xpm \
	resources/buttonRightLocked.xpm \
	resources/buttonRightSelectedLocked.xpm \
	resources/buttonRightSelected.xpm \
	resources/buttonRight.xpm \
	resources/buttonShowMainWindow.xpm \
	resources/usermoveh64.xpm \
	resources/usermovev64.xpm

EXTRA_DIST = eviacam.desktop resources/eviacam.png

desktopdir		=	$(datadir)/applications/
desktop_DATA	=	eviacam.desktop

pixmapdir		=	$(datadir)/pixmaps/
pixmap_DATA	=	resources/eviacam.png

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
eviacam_SOURCES = wgetkey.cpp \
	cautostart.cpp \
	keyboardcode.cpp \
	clickwindow.cpp \
	clickwindowbitmap.cpp \
	clickwindowcontroller.cpp \
	clickwindowtext.cpp \
	cmotioncalibration.cpp \
	eviacamapp.cpp \
	visionpipeline.cpp \
	viacamcontroller.cpp \
	wabout.cpp \
	wconfiguration.cpp \
	wconfirmcalibration.cpp \
	wmotioncalibrationx.cpp \
	wmotioncalibrationy.cpp \
	wviacam.cpp \
	cvisualalert.cpp \
	wwizard.cpp \
	wwizardmanager.cpp \
	wcameradialog.cpp \
	hotkeymanager.cpp \
	dwellclick.cpp \
	gestureclick.cpp \
	pointeraction.cpp \
	paths.cpp \
	checkupdates.cpp \
	checkupdates_ui.cpp \
	website.cpp \
	newtrackerinformationdlg.cpp \
	checkupdates_manager.cpp
	
eviacam_CXXFLAGS = -DLOCALEDIR=\"$(localedir)\" -DOPENCVVERSION=$(OPENCVVERSION)
eviacam_LDFLAGS = $(WX_LIBS) $(OPENCV_LIBS) -lXtst -lXext -lX11

AM_CPPFLAGS = \
	-I$(top_srcdir)/creavision \
	-I$(top_srcdir)/simplelog \
	-I$(top_srcdir)/wxcamwindow \
	-I$(top_srcdir)/wxutil

eviacam_LDADD = \
	$(top_builddir)/wxutil/libwxutil.la \
	$(top_builddir)/simplelog/libsimplelog.la \
	$(top_builddir)/creavision/libcreavision.la \
	$(top_builddir)/creavision/libwebcam/libwebcam.la \
	$(top_builddir)/wxcamwindow/libwxcamwindow.la
