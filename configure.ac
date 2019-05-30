#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Gol resurrection], [0.5], [contact@xaviersala.net])
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_SRCDIR([src/gol.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AX_PROG_LUA([5.1], [5.4])

# Checks for libraries.
dnl Check for SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
        :,
        AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

AC_CHECK_LIB(SDL_ttf, TTF_Quit, have_sdl_ttf=yes)
AC_CHECK_LIB(SDL_image, IMG_Load, have_sdl_image=yes)
AC_CHECK_LIB(SDL_gfx, SDL_setFramerate, have_sdl_gfx=yes)

CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS -lSDL_ttf -lSDL_image -lSDL_gfx"

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])
AX_LUA_HEADERS

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([atexit sqrt])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
