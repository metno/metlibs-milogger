#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([miLogger], [4.0.0], [diana@met.no])
AM_INIT_AUTOMAKE
METNO_SET_VERSION_INFO
AC_CONFIG_SRCDIR([src/logger.cc])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 milogger.pc
                 include/Makefile
                 src/Makefile])
AC_OUTPUT
