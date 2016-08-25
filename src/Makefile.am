# $Id$

pkgname = miLogger

lib_LTLIBRARIES = libmiLogger.la

libmiLogger_la_SOURCES = logger.cc LogHandler.cpp miLogging.cc
libmiLogger_la_LDFLAGS = -fPIC ${VERSION_INFO}

libmiLogger_la_CPPFLAGS = -fPIC $(AM_CPPFLAGS)
libmiLogger_la_LIBADD =

if WITH_LOG4CPP
miLogging.cc: miLogging_log4cpp.cc
libmiLogger_la_CPPFLAGS += ${LOG4CPP_CPPFLAGS}
if ENABLE_SHARED_LIBRARY
libmiLogger_la_LIBADD  += ${LOG4CPP_LIBS}
libmiLogger_la_LDFLAGS += ${LOG4CPP_LDFLAGS}
endif # ENABLE_SHARED_LIBRARY
else # !WITH_LOG4CPP
miLogging.cc: miLogging_iostream.cc
libmiLogger_la_CPPFLAGS += -DNO_LOG4CPP
endif # !WITH_LOG4CPP

pkg_includedir = ${includedir}/miLogger
pkg_include_HEADERS = logger.h LogHandler.h miLogging.h

EXTRA_DIST= miLogging_iostream.cc miLogging_log4cpp.cc

INSTALL=install -p
