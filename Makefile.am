# $Id$

pkgname = miLogger

include $(top_srcdir)/metlibs.mk

lib_LTLIBRARIES = libmiLogger.la

libmiLogger_la_SOURCES = logger.cc LogHandler.cpp
libmiLogger_la_LDFLAGS = ${VERSION_INFO} 
#	${LOG4CPP_LDFLAGS}

libmiLogger_la_CPPFLAGS = $(AM_CPPFLAGS) ${LOG4CPP_CPPFLAGS}

#libmiLogger_la_LIBADD = \
#        ${LOG4CPP_LIBS}

pkg_includedir = ${includedir}/miLogger
pkg_include_HEADERS = logger.h LogHandler.h

