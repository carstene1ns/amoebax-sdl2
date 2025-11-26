# _ES_FRAMEWORK_PATH(FRAMEWORK-NAME, HEADER-FILE[, ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
# Check for the path to Framework FRAMEWORK-NAME by looking HEADER-FILE in it.
AC_DEFUN([_ES_FRAMEWORK_PATH],
[
    _es_frameworks_dirs="/System/Library/Frameworks /Library/Frameworks ~/Library/Frameworks"
    _es_framework_headers_dir="no"
    for _es_frameworks_dir in ${_es_frameworks_dirs} ; do
        _es_tmp_headers_dir="${_es_frameworks_dir}/[$1].framework/Headers"
        _es_framework_save_CFLAGS="${CFLAGS}"
        _es_framework_save_CXXFLAGS="${CXXFLAGS}"
        CFLAGS="${CFLAGS} -I${_es_tmp_headers_dir}"
        CXXFLAGS="${CXXFLAGS} -I${_es_tmp_headers_dir}"
        AC_COMPILE_IFELSE(AC_LANG_PROGRAM([#include <$2>], []),
            [_es_framework_headers_dir=${_es_tmp_headers_dir}], [])
        CFLAGS="${_es_framework_save_CFLAGS}"
        CXXFLAGS="${_es_framework_save_CXXFLAGS}"
        if test "x${_es_framework_headers_dir}" != "xno" ; then
            break
        fi
    done

    if test "x${_es_lib_sdl_osx_headers_dir}" = "xno" ; then
        FRAMEWORK_HEADERS_DIR=""
        ifelse([$4], [], :, [$4])
    else
        FRAMEWORK_HEADERS_DIR="${_es_framework_headers_dir}"
        ifelse([$3], [], :, [$3])
    fi
])#_ES_FRAMEWORK_PATH

# ES_LIB_SDL([MIN-VERSION[, ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]]])
# -------------------------------------------------------------------
# Checks for SDL library.
# If found, defines SDL_CFLAGS and SDL_LIBS.
#
# Based on the original SDL's sdl.m4 file.
AC_DEFUN([ES_LIB_SDL],
[AC_REQUIRE([AC_CANONICAL_HOST])[]dnl
AC_ARG_WITH([sdl-prefix],
            AS_HELP_STRING([--with-sdl-prefix],
                           [prefix where SDL is installed (optional)]),
            [_es_lib_sdl_prefix=$withval], [_es_lib_sdl_prefix=""])
AC_ARG_WITH([sdl-exec-prefix],
            AS_HELP_STRING([--with-sdl-exec-prefix],
                           [prefix where SDL binaries are installed (optional)]),
            [_es_lib_sdl_exec_prefix=$withval], [_es_lib_sdl_exec_prefix=""])

_es_lib_sdl_found="no"
_es_lib_sdl_min_version=ifelse([$1], [], [0.11.0], [$1])
_es_lib_sdl_min_version_major=`echo ${_es_lib_sdl_min_version} | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
_es_lib_sdl_min_version_minor=`echo ${_es_lib_sdl_min_version} | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
_es_lib_sdl_min_version_micro=`echo ${_es_lib_sdl_min_version} | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

case "${host}" in
    *-apple-darwin*)
        _es_lib_sdl_osx=yes
        ;;
esac
# On Mac OS X, just try to use the SDL framework.
if test "x${_es_lib_sdl_osx}" = "xyes" ; then
    AC_MSG_CHECKING([for SDL, version >= ${_es_lib_sdl_min_version}])
    _ES_FRAMEWORK_PATH([SDL], [SDL.h],
                       [_es_lib_sdl_osx_headers_dir="${FRAMEWORK_HEADERS_DIR}"],
                       [_es_lib_sdl_osx_headers_dir="no"])

    if test "x${_es_lib_sdl_osx_headers_dir}" = "xno" ; then
        SDL_CFLAGS=""
        SDL_LIBS=""

        AC_MSG_RESULT([no])
        ifelse([$3], [], :, [$3])
    else
        SDL_CFLAGS="-I${_es_lib_sdl_osx_headers_dir}"
        SDL_LIBS="-framework SDL -framework Cocoa"
        _es_lib_sdl_found="yes"
    fi
else
# On others platforms use the sdl-config program.
    if test "x${_es_lib_sdl_exec_prefix}" != "x" ; then
        _es_lib_sdl_args="${_es_lib_sdl_args} --exec-prefix=${_es_lib_sdl_exec_prefix}"
        _es_lib_sdl_path="${_es_lib_sdl_exec_prefix}/bin:${_es_lib_sdl_path}"
    fi

    if test "x${_es_lib_sdl_prefix}" != "x" ; then
        _es_lib_sdl_args="${_es_lib_sdl_args} --prefix=${_es_lib_sdl_prefix}"
        _es_lib_sdl_path="${_es_lib_sdl_prefix}/bin:${_es_lib_sdl_path}"
    fi
    AC_PATH_PROG([SDL_CONFIG], [sdl-config], [no], [${_es_lib_sdl_path}:${PATH}])
    AC_MSG_CHECKING([for SDL, version >= ${_es_lib_sdl_min_version}])
    if test "x${SDL_CONFIG}" != "xno" ; then
        SDL_CFLAGS=`${SDL_CONFIG} ${_es_lib_sdl_args} --cflags`
        SDL_LIBS=`${SDL_CONFIG} ${_es_lib_sdl_args} --libs`
        _es_lib_sdl_found="yes"
    else
        SDL_CFLAGS=""
        SDL_LIBS=""

        AC_MSG_RESULT([no])
        ifelse([$3], [], :, [$3])
    fi
fi

if test "x${_es_lib_sdl_found}" = "xyes" ; then
    _es_lib_sdl_save_CFLAGS="${CFLAGS}"
    _es_lib_sdl_save_CXXFLAGS="${CXXFLAGS}"
    _es_lib_sdl_save_LIBS="${LIBS}"
    CFLAGS="${CFLAGS} ${SDL_CFLAGS}"
    CXXFLAGS="${CXXFLAGS} ${SDL_CFLAGS}"
    LIBS="${LIBS} ${SDL_LIBS}"
    AC_LINK_IFELSE(AC_LANG_PROGRAM([#include <SDL.h>
#include <SDL_version.h>


#if SDL_MAJOR_VERSION * 1000 + SDL_MINOR_VERSION * 100 + SDL_PATCHLEVEL < ${_es_lib_sdl_min_version_major} * 1000 + ${_es_lib_sdl_min_version_minor} * 100 + ${_es_lib_sdl_min_version_micro}
#error "SDL version not valid!"
#endif

#ifdef main
#undef main
#endif
#if IS_WINCE_HOST
int SDL_main (int a, char **v) { return 0; }
#endif],
                                 []),
                   _es_lib_sdl_linked="yes", _es_lib_sdl_linked="no")
    CFLAGS="${_es_lib_sdl_save_CFLAGS}"
    CXXFLAGS="${_es_lib_sdl_save_CXXFLAGS}"
    LIBS="${_es_lib_sdl_save_LIBS}"
    if test "x${_es_lib_sdl_linked}" = "xyes" ; then
        AC_SUBST([SDL_CFLAGS])
        AC_SUBST([SDL_LIBS])
        AC_MSG_RESULT([yes])
        ifelse([$2], [], :, [$2])
    else
        AC_MSG_RESULT([no])
        ifelse([$3], [], :, [$3])
    fi
fi
])# ES_LIB_SDL

# ES_LIB_SDL_SUBLIB(SUBLIBRARY-NAME, VARIABLE-SUFFIX[, ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
# ----------------------------------------------------------
# Checks for a sublibrary of SDL (usually called SDL_name).
# If found, defines SDL_[$VARIABLE-SUFFIX]_CFLAGS and SDL_[$VARIABLE_SUFFIX]_LIBS.
AC_DEFUN([ES_LIB_SDL_SUBLIB],
[AC_REQUIRE([ES_LIB_SDL])[]dnl
_es_lib_sdl_sublibrary_found="no"

case "${host}" in
    *-apple-darwin*)
        _es_lib_sdl_sublibrary_osx="yes"
        ;;
    *-gp2x*)
        _es_lib_sdl_sublibrary_gp2x="yes"
        ;;
esac
# On Mac OS X, just try to use the SDL_sublibrary framework.
if test "x${_es_lib_sdl_sublibrary_osx}" = "xyes" ; then
    AC_MSG_CHECKING([for SDL_$1])
    _es_lib_sdl_sublibrary_save_CFLAGS="${CFLAGS}"
    _es_lib_sdl_sublibrary_save_CXXFLAGS="${CXXFLAGS}"
    CFLAGS="${CFLAGS} ${SDL_CFLAGS}"
    CXXFLAGS="${CXXFLAGS} ${SDL_CFLAGS}"
    _ES_FRAMEWORK_PATH([SDL_$1], [SDL_$1.h],
                       [_es_lib_sdl_sublibrary_osx_headers_dir="${FRAMEWORK_HEADERS_DIR}"],
                       [_es_lib_sdl_sublibrary_osx_headers_dir="no"])
    CFLAGS="${_es_lib_sdl_sublibrary_save_CFLAGS}"
    CXXFLAGS="${_es_lib_sdl_sublibrary_save_CXXFLAGS}"

    if test "x${_es_lib_sdl_sublibrary_osx_headers_dir}" = "xno" ; then
        SDL_$2[]_CFLAGS=""
        SDL_$2[]_LIBS=""

        AC_MSG_RESULT([no])
        ifelse([$4], [], :, [$4])
    else
        SDL_$2[]_CFLAGS="-I${_es_lib_sdl_sublibrary_osx_headers_dir}"
        SDL_$2[]_LIBS="-framework SDL_$1"
        _es_lib_sdl_sublibrary_found="yes"
    fi
# On others platforms try to compile the test program using SDL_CFLAGS
# and SDL_LIBS.
else
    # I seem to have problems in GP2X if I don't include them when linking.
    if test "x${_es_lib_sdl_sublibrary_gp2x}" = "xyes" ; then
        case $1 in
            image)
            _es_lib_sdl_sublibrary_extra_libs="-ljpeg -lpng -lz"
            ;;
            mixer)
            _es_lib_sdl_sublibrary_extra_libs="-lvorbisidec"
            ;;
            ttf)
            _es_lib_sdl_sublibrary_extra_libs="-lfreetype"
            ;;
        esac
    fi
    AC_MSG_CHECKING([for SDL_$1])
    _es_lib_sdl_sublibrary_save_CFLAGS="${CFLAGS}"
    _es_lib_sdl_sublibrary_save_CXXFLAGS="${CXXFLAGS}"
    CFLAGS="${CFLAGS} ${SDL_CFLAGS}"
    CXXFLAGS="${CXXFLAGS} ${SDL_CFLAGS}" 
    AC_COMPILE_IFELSE(AC_LANG_PROGRAM([#include <SDL_$1.h>], []),
        [_es_lib_sdl_sublibrary_found="yes"], [_es_lib_sdl_sublibrary_found="no"])
    CFLAGS="${_es_lib_sdl_sublibrary_save_CFLAGS}"
    CXXFLAGS="${_es_lib_sdl_sublibrary_save_CXXFLAGS}"

    if test "x${_es_lib_sdl_sublibrary_found}" = "xno" ; then
        SDL_$2[]_CFLAGS=""
        SDL_$2[]_LIBS=""
        AC_MSG_RESULT([no])
        ifelse([$4], [], :, [$4])
    else
        SDL_$2[]_CFLAGS=""
        SDL_$2[]_LIBS="-lSDL_$1 ${_es_lib_sdl_sublibrary_extra_libs}"
    fi
fi

if test "x${_es_lib_sdl_sublibrary_found}" = "xyes" ; then
    _es_lib_sdl_sublibrary_save_CFLAGS="${CFLAGS}"
    _es_lib_sdl_sublibrary_save_CXXFLAGS="${CXXFLAGS}"
    _es_lib_sdl_sublibrary_save_LIBS="${LIBS}"
    CFLAGS="${CFLAGS} ${SDL_CFLAGS} ${SDL_$2[]_CFLAGS}"
    CXXFLAGS="${CXXFLAGS} ${SDL_CFLAGS} ${SDL_$2[]_CFLAGS}"
    LIBS="${LIBS} ${SDL_LIBS} ${SDL_$2[]_LIBS}"
    AC_LINK_IFELSE(AC_LANG_PROGRAM([#include <SDL.h>
#include <SDL_$1.h>

#ifdef main
#undef main
#endif
#if IS_WINCE_HOST
int SDL_main (int a, char **v) { return 0; }
#endif],
                                   []),
                   [_es_lib_sdl_sublibrary_linked="yes"],
                   [_es_lib_sdl_sublibrary_linked="no"])
    CFLAGS="${_es_lib_sdl_sublibrary_save_CFLAGS}"
    CXXFLAGS="${_es_lib_sdl_sublibrary_save_CXXFLAGS}"
    LIBS="${_es_lib_sdl_sublibrary_save_LIBS}"

    if test "x${_es_lib_sdl_sublibrary_linked}" = "xyes" ; then
        AC_MSG_RESULT([yes])
        ifelse([$3], [], :, [$3])
    else
        AC_MSG_RESULT([no])
        ifelse([$4], [], :, [$4])
    fi
fi
])# ES_LIB_SDL_SUBLIBRARY
