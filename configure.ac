# Process this file with autoconf to produce a configure script.
AC_INIT([Amoebax], [0.2.1], [jordi@emma-soft.com])
AC_COPYRIGHT([Copyright (c) 2006, 2007 Emma's Software.])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

# Check host.
AC_CANONICAL_HOST
case "${host}" in
    *-apple-darwin*)
        AC_DEFINE([IS_OSX_HOST], [1], [Set to 1 if you are compiling on an OS X host.])
        osx_host="yes"
        ;;
    *-*-mingw32* | *-*-cygwin*)
        AC_DEFINE([IS_WIN32_HOST], [1], [Set to 1 if you are compiling on a Windows host.])
        win32_host="yes"
        ;;
esac
AM_CONDITIONAL([IS_OSX_HOST], [test "x${osx_host}" = "xyes"])
AM_CONDITIONAL([IS_WIN32_HOST], [test "x${win32_host}" = "xyes"])

# Parameters
AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug],
                             [turn on debugging (default is NO)]))
if test "x$enable_debug" != "xyes"; then
    DEBUGFLAGS="-DNDEBUG"
else
    DEBUGFLAGS="-DDEBUG"
fi

# Check for programs.
AC_PROG_MAKE_SET
AC_PROG_CXX
AC_LANG([C++])
# Support for resource files in Windows.
AC_CHECK_TOOL([WINDRES], [windres], [no])
AM_CONDITIONAL([build_win32_resource], [test "x$WINDRES" != "xno"])
# Check for WiX files in order to create Windows installer script.
AC_PATH_PROG([CANDLE], [candle], [no])
AC_PATH_PROG([LIGHT], [light], [no])
AM_CONDITIONAL([build_win32_msi], [test "x$CANDLE" != "xno" -a "x$LIGHT" != "xno"])
# Support for documentation.
AC_PATH_PROG([DOXYGEN], [doxygen], [no])
AM_CONDITIONAL(build_reference, [test "x$DOXYGEN" != "xno"])

# Check for header files.
# Check for libraries.
AM_PATH_SDL([1.2.7], , AC_MSG_ERROR([SDL library not found.]))
PKG_CHECK_MODULES([SDL_IMAGE], [SDL_image])
PKG_CHECK_MODULES([SDL_MIXER], [SDL_mixer])
AC_SUBST([SDL_IMAGE_CFLAGS])
AC_SUBST([SDL_IMAGE_LIBS])
AC_SUBST([SDL_MIXER_CFLAGS])
AC_SUBST([SDL_MIXER_LIBS])

# Check for types.
# Check for structures.
# Check for compiler characteristics.
# Check for libraries functions.
# Check for system services.

# Set the CFLAGS and CXXFLAGS
CFLAGS="$CFLAGS $DEBUGFLAGS"
CXXFLAGS="$CXXFLAGS $DEBUGFLAGS"

AC_CONFIG_FILES([   \
Makefile    \
data/Makefile   \
data/InfoPlist.strings \
data/Info.plist \
data/AmoebaxMain.nib/Makefile   \
data/fonts/Makefile   \
data/graphics/Makefile   \
data/music/Makefile   \
data/sfx/Makefile   \
doc/Doxyfile    \
doc/Makefile    \
src/Makefile])
AC_OUTPUT
