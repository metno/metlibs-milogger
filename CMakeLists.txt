PROJECT(metlibs-milogger)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(MetnoUtils)
INCLUDE(MetnoGTestUtils)

METNO_PVERSION_DEFINES(MILOGGER "src/miLoggerVersion.h")

SET(DO_NOT_USE_STD_ATOMIC FALSE CACHE BOOL "disable use of std::atomic")
SET(DO_NOT_USE_POOL_ALLOCATOR FALSE CACHE BOOL "disable use of boost pool allocator")

PKG_CHECK_MODULES(PC_LOG4CPP
  log4cpp
)
IF (${PC_LOG4CPP_FOUND})
  SET(PKGS log4cpp)
ENDIF ()

FIND_PACKAGE(Boost REQUIRED)

SET(lib_name "metlibs-milogger")

METNO_GEN_PKGCONFIG("${lib_name}.pc.in" "${lib_name}.pc"
  "${PKGS}" "" "" "")

CONFIGURE_FILE(miLogger.pc.in miLogger.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/miLogger.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
