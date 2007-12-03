#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([miLogger], [4.0.0], [diana@met.no])
AC_CONFIG_SRCDIR([src/logger.cc])

AM_INIT_AUTOMAKE
METNO_SET_VERSION_INFO

AC_LANG(C++)

AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET


# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		milogger.pc
                include/Makefile
                src/Makefile])
AC_OUTPUT
