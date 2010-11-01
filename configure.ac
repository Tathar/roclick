#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([roclick], [1.1], [tathar.dev@gmail.com])
AM_INIT_AUTOMAKE([roclick], [1.1])
AC_CONFIG_SRCDIR([src/roclick.c])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h inttypes.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/param.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostname memmove memset select socket strchr strrchr strtoul atexit getcwd strdup])


# Checks for X11.
AC_CHECK_HEADERS([X11/Xlib.h], [
    AC_CHECK_LIB(X11, XOpenDisplay, [
	LIBS="$LIBS -lX11"
	AC_DEFINE(HAVE_LIBXLIB, 1, [Define to 1 if you have the 'Xlib' library (-lX11).])
    ], AC_MSG_ERROR(The Xlib library is required in order to build the program!))
], AC_MSG_ERROR(The Xlib header is required in order to build the program!))

# Checks for xcb.
AC_CHECK_HEADERS([xcb/xcb.h], [
    AC_CHECK_LIB(xcb, xcb_connect, [
	LIBS="$LIBS -lxcb"
	AC_DEFINE(HAVE_LIBXCB, 1, [Define to 1 if you have the 'xcb' library (-lxcb).])
    ], AC_MSG_ERROR(The xcb library is required in order to build the program!))
], AC_MSG_ERROR(The xcb header is required in order to build the program!))

# Checks for xcb_atom.
AC_CHECK_HEADERS([xcb/xcb_atom.h], [
    AC_CHECK_LIB(xcb-atom, STRING, [
	LIBS="$LIBS -lxcb-atom"
	AC_DEFINE(HAVE_LIBXCBATOM, 1, [Define to 1 if you have the 'xcb-atom' library (-lxcb-atom).])
    ], AC_MSG_ERROR(The xcb-atom library is required in order to build the program!))
], AC_MSG_ERROR(The xcb-atom header is required in order to build the program!))


# Checks for guile.
AC_CHECK_HEADERS([libguile.h], [
    AC_CHECK_LIB(guile, scm_c_define_gsubr, [
	CFLAGS="$CFLAGS `guile-config compile`"
	LIBS="$LIBS `guile-config link`"
	AC_DEFINE(HAVE_LIBGUILE, 1, [Define to 1 if you have the 'guile' library (-lguile).])
    ], AC_MSG_ERROR(The guile library is required in order to build the program!))
], AC_MSG_ERROR(The guile header is required in order to build the program!))

if test "x$GCC" = "xyes"; then
  CFLAGS="$CFLAGS -Wall"
fi
#-DHAVE_CONFIG_H -DPIC
		
AC_CONFIG_FILES([Makefile libXRecPlay/Makefile libXRecPlay/xcb/Makefile src/Makefile])
AC_OUTPUT
