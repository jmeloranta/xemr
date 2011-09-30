#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([xemr], [0.8])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([xemr.lsm])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_LANG_C
#AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MAKE_SET
AC_PROG_F77

# Checks for libraries.
#AC_CHECK_LIB(lapack, zgemv_,AC_MSG_RESULT([lapack library found.]),AC_MSG_ERROR([lapack library not installed.]))

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])
AC_CHECK_HEADERS([meas/meas.h],[AC_DEFINE([LIBMEAS],1,
	[Define to 1 if you have <meas/meas.h>])],
	[AC_MSG_RESULT([libmeas not installed. Measurement interface disabled.])])
AC_CHECK_HEADERS([fftw.h],[AC_DEFINE([HAVE_FFTW2_H],1,
	[Define to 1 if you have <fftw.h>])],
	[AC_MSG_ERROR([Missing fftw.h header. Is libfftw2 installed?])])
AC_CHECK_HEADERS([forms.h],[AC_DEFINE([HAVE_XFORMS_H],1,
	[Define to 1 if you have <forms.h>])],
	[AC_MSG_ERROR([Missing forms.h header. Is xforms installed?])])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero pow re_comp sqrt strncasecmp strchr strrchr strstr])

AC_OUTPUT([Makefile xemr/Makefile ui/Makefile libepr/Makefile util/Makefile])
