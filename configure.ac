AC_INIT([eviacam], 2.1.0)
AC_CONFIG_SRCDIR([src/eviacamapp.cpp])
AC_PREREQ(2.61)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE
COPYRIGHT="© 2008-16"
PUBLISHER="César Mauri Loba"

AC_SUBST(COPYRIGHT, [`echo $COPYRIGHT`])
AC_DEFINE_UNQUOTED([COPYRIGHT], ["$COPYRIGHT"], [Copyright notice])
AC_SUBST(PUBLISHER, [`echo $PUBLISHER`])
AC_DEFINE_UNQUOTED([PUBLISHER], ["$PUBLISHER"], [Publisher info])

# Use libtool for building static libraries only
AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

ALL_LINGUAS="ca es gl it tr ast de oc fr ar he ru pt_BR ja el lt zh_TW zh_CN"

AM_GNU_GETTEXT([external])

# ----------------- Debug mode switch --------------------
AC_ARG_ENABLE(debug,
[  --enable-debug          turn on debug build],
[case "${enableval}" in
yes) debug=true ;;
full) debug=true ;;
no)  debug=false ;;
*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AC_MSG_NOTICE([build in debug mode... $debug])
# ----------------- Debug mode switch --------------------

# ------------- Check for updates switch  ----------------
# This adds the option of compiling without automatic check for
# updates which is undesired for some builds (e.g. debian)
AC_ARG_ENABLE(updates_check,
  [  --disable-updates-check disable check for updates feature],
  [case "${enableval}" in
     yes | no ) WITH_UPDATES_CHECK="${enableval}" ;;
     *) AC_MSG_ERROR(bad value ${enableval} for --disable-updates-check) ;;
   esac],
  [WITH_UPDATES_CHECK="no"]
)
AM_CONDITIONAL([WITH_UPDATES_CHECK], [test "x$WITH_UPDATES_CHECK" = "xyes"])

# Define UPDATES_CHECK in config.h if we're going to compile it
if test "x$WITH_UPDATES_CHECK" = "xyes"; then
    AC_DEFINE([ENABLE_UPDATES_CHECK], [], [Enable check for updates feature])
    AC_MSG_NOTICE([check for updates feature... yes])
else
    AC_MSG_NOTICE([check for updates feature... no])
fi
# ------------- Check for updates switch  ----------------

# ------------- Check dependencies switch  ----------------
# This adds the option of compiling without automatic check for
# updates which is undesired for some builds (e.g. debian)
AC_ARG_ENABLE(dependencies_check,
  [  --disable-dependencies-check do not check dependencies (e.g. if building for Win)],
  [case "${enableval}" in
     yes | no ) WITH_DEPENDENCIES_CHECK="${enableval}" ;;
     *) AC_MSG_ERROR(bad value ${enableval} for --disable-dependencies-check) ;;
   esac],
  [WITH_DEPENDENCIES_CHECK="yes"]
)
AM_CONDITIONAL([WITH_DEPENDENCIES_CHECK], [test "x$WITH_DEPENDENCIES_CHECK" = "xyes"])

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
    AC_MSG_NOTICE([check dependencies... yes])
else
    AC_MSG_NOTICE([check dependencies... no])
fi
# ------------- Check dependencies switch  ----------------

# Checks for libraries.
# FIXME: Replace `main' with a function of each library

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
	AC_CHECK_LIB([X11], [main])
	AC_CHECK_LIB([Xext], [main], , AC_MSG_ERROR([libXext is required.]) )
	AC_CHECK_LIB([Xtst], [main], , AC_MSG_ERROR([libXtst is required.]) )
	AC_CHECK_FUNCS([pow])
	AC_CHECK_FUNCS([sqrt])
fi

# -----------------       gtk+-2.0  --------------------
# TODO: check if gtk-2.0-devel is really needed
# Put this early on, since gtk may rely on it.
AC_PATH_XTRA

GTKCONFIG='pkg-config gtk+-2.0'
AC_DEFUN([GTKTEST],
[
	AC_REQUIRE([AC_PROG_AWK])
	AC_MSG_CHECKING([gdk version])
	if gdkversion=`pkg-config gtk+-2.0 --modversion`; then
		AC_MSG_RESULT([$gdkversion])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([gtk+-2.0 is required.])
	fi])

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
	# Call test func
	GTKTEST

	GTK_CPPFLAGS="`$GTKCONFIG --cflags`"
	GTK_LIBS="`$GTKCONFIG --libs`"
fi
# -----------------   gtk+-2.0      --------------------

# -----------------       wxWidgets --------------------
WXCONFIG=wx-config
AC_ARG_WITH(wx-config,
[[  --with-wx-config=FILE     Use the given path to wx-config when determining
                            wxWidgets configuration; defaults to "wx-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        WXCONFIG=$withval
    fi
])

wxversion=0

AC_DEFUN([WXTEST],
[
	AC_REQUIRE([AC_PROG_AWK])
	AC_MSG_CHECKING([wxWidgets version])
	if wxversion=`$WXCONFIG --version`; then
		AC_MSG_RESULT([$wxversion])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
	fi])

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
	# Call WXTEST func
	WXTEST

	# Verify minimum requires
	vers=`echo $wxversion | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`
	if test -n "$vers" && test "$vers" -ge 2008003; then
		if ! "$debug"; then
			WX_CPPFLAGS="`$WXCONFIG --cppflags`"
			WX_CXXFLAGS="`$WXCONFIG --cxxflags | sed -e 's/-fno-exceptions//'`"
			WX_LIBS="`$WXCONFIG --libs`"
		else
			WX_CPPFLAGS="`$WXCONFIG --cppflags`"
			WX_CXXFLAGS="`$WXCONFIG --cxxflags | sed -e 's/-fno-exceptions//'`"
			WX_LIBS="`$WXCONFIG --libs`"
		fi
	else
		AC_MSG_ERROR([wxWidgets 2.8.3 or newer is required])
	fi
fi
# -----------------       wxWidgets --------------------

# -----------------       opencv  --------------------
OPENCVCONFIG='pkg-config opencv'
AC_DEFUN([OPENCVTEST],
[
	AC_REQUIRE([AC_PROG_AWK])
	AC_MSG_CHECKING([opencv version])
	if opencvversion=`pkg-config opencv --modversion`; then
		AC_MSG_RESULT([$opencvversion])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([opencv is required.])
	fi])

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
	# Call OPENCVTEST func
	OPENCVTEST

	OPENCVVERSION=`echo $opencvversion | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`
	AC_SUBST(OPENCVVERSION)
	OPENCV_CPPFLAGS="`$OPENCVCONFIG --cflags`"
	OPENCV_LIBS=" -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_objdetect -lopencv_video"
fi
# -----------------       opencv  --------------------

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([sys/timeb.h])

if test "x$WITH_DEPENDENCIES_CHECK" = "xyes"; then
	AC_CHECK_HEADER([libv4l1-videodev.h],
		[AC_SUBST(VIDEODEV_INCLUDE_FILE, libv4l1-videodev.h)],
		[AC_CHECK_HEADER([linux/videodev.h],
			[AC_SUBST(VIDEODEV_INCLUDE_FILE, linux/videodev.h)],
			[AC_MSG_ERROR([Could not find linux/videodev.h nor libv4l1-videodev.h])])
		]
	)
fi
		
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

COMPFLAGS="-fexceptions $WX_CPPFLAGS $OPENCV_CPPFLAGS $GTK_CPPFLAGS"
if ! "$debug"; then
	COMPFLAGS="$COMPFLAGS -DNDEBUG -O2"
else
	COMPFLAGS="$COMPFLAGS -Wall -Wconversion -DDEBUG -g -O0"
fi

CXXFLAGS="$CXXFLAGS $COMPFLAGS"
LDFLAGS="$LDFLAGS -Wl,--as-needed"

AC_SUBST(WX_LIBS)
AC_SUBST(OPENCV_LIBS)
AC_SUBST(GTK_LIBS)

# libtool variables
Xsed="sed -e s/^X//"
LTLIBOBJS=`echo X"$LIBOBJS"|\
           [$Xsed -e "s,\.[^.]* ,.lo ,g;s,\.[^.]*$,.lo,"]`
AC_SUBST(LTLIBOBJS)

AC_OUTPUT(
	[Makefile] 
	[po/Makefile.in] 
	[man/Makefile] 
	[doc/Makefile] 
	[wxutil/Makefile] 
	[creavision/Makefile] 
	[creavision/libwebcam/Makefile] 
	[simplelog/Makefile] 
	[wxcamwindow/Makefile] 
	[src/Makefile] 
	[src/eviacamloader.c]
	[win32/installer.iss]
	[creavision/libwebcam/incvideodev.h]
)
