# Process this file with autoconf to produce a configure script.
AC_INIT([Amoebax], [0.2.1], [jordi@emma-soft.com])
AC_COPYRIGHT([Copyright (c) 2006, 2007 Emma's Software.])
AM_INIT_AUTOMAKE([dist-bzip2])
AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

# Check host.
AC_CANONICAL_HOST
case "${host}" in
    *-apple-darwin*)
        AC_DEFINE([IS_OSX_HOST], [1], [Set to 1 if you are compiling on an OS X host.])
        osx_host="yes"
        ;;
    *-gp2x*)
        AC_DEFINE([IS_GP2X_HOST], [1], [Set to 1 if you are compiling on a GP2X host.])
        gp2x_host="yes"
        ;;
    arm-*-mingw32*)
        AC_DEFINE([IS_WINCE_HOST], [1], [Set to 1 if you are compiling on a Windows CE host.])
        wince_host="yes"
        ;;
    *-*-mingw32* | *-*-cygwin*)
        AC_DEFINE([IS_WIN32_HOST], [1], [Set to 1 if you are compiling on a Windows host.])
        win32_host="yes"
        ;;
esac
AM_CONDITIONAL([IS_OSX_HOST], [test "x${osx_host}" = "xyes"])
AM_CONDITIONAL([IS_GP2X_HOST], [test "x${gp2x_host}" = "xyes"])
AM_CONDITIONAL([IS_WIN32_HOST], [test "x${win32_host}" = "xyes"])
AM_CONDITIONAL([IS_WINCE_HOST], [test "x${wince_host}" = "xyes"])

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
# Support for Objective-C in OS X.
AC_PROG_OBJC
AC_SUBST([OBJC])
AC_SUBST([OBJCFLAGS])
_AM_DEPENDENCIES([OBJC])
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
AM_BINRELOC
ES_LIB_SDL([1.2.7], [amoebax_sdl_found="yes"])
if test "x${amoebax_sdl_found}" != "xyes" ; then
    AC_MSG_ERROR([SDL library not found.])
fi
ES_LIB_SDL_SUBLIB([image], [IMAGE], [amoebax_sdl_image_found="yes"])
if test "x${amoebax_sdl_image_found}" != "xyes" ; then
    AC_MSG_ERROR([SDL_image library not found.])
fi
ES_LIB_SDL_SUBLIB([mixer], [MIXER], [amoebax_sdl_mixer_found="yes"])
if test "x${amoebax_sdl_mixer_found}" != "xyes" ; then
    AC_MSG_ERROR([SDL_mixer library not found.])
fi

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

# Set the right extension for GP2X executable files.
if test "x${gp2x_host}" = "xyes"; then
    EXEEXT=".gpe"
fi

AC_CONFIG_FILES([   \
Makefile    \
autopackage/Makefile    \
autopackage/default.apspec  \
data/Makefile   \
data/InfoPlist.strings \
data/Info.plist \
data/AmoebaxMain.nib/Makefile   \
data/dlls/Makefile   \
data/fonts/Makefile   \
data/graphics/Makefile   \
data/graphics/gp2x/Makefile   \
data/music/Makefile   \
data/sfx/Makefile   \
doc/Doxyfile    \
doc/Makefile    \
m4/Makefile \
src/Makefile])
AC_OUTPUT
