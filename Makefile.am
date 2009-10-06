# $Id: Makefile.am 2786 2009-09-23 11:39:02Z dages $

pkgname = miLogger

include $(top_srcdir)/metlibs.mk

lib_LTLIBRARIES = libmiLogger.la

libmiLogger_la_SOURCES = logger.cc
libmiLogger_la_LDFLAGS = ${VERSION_INFO}

pkg_includedir = ${prefix}/include/miLogger
pkg_include_HEADERS = logger.h

