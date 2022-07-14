
ADD_COMPILE_OPTIONS (-W -Wall)
IF (DO_NOT_USE_STD_ATOMIC)
  ADD_DEFINITIONS(-DMILOGGER_DO_NOT_USE_STD_ATOMIC)
ENDIF ()
IF (DO_NOT_USE_POOL_ALLOCATOR)
  ADD_DEFINITIONS(-DMILOGGER_DO_NOT_USE_POOL_ALLOCATOR)
ENDIF ()

INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})

SET(milogger_SOURCES
  miLoggingLogger.cc
  miLoggingMemory.cc
  miLoggingNone.cc
  miLoggingScopes.cc
  miLoggingStringRecord.cc
  miLoggingSystem.cc
  miLoggingSimple.cc
  miLoggingUtils.cc
)

SET(milogger_HEADERS
  miLogging.h
  miLoggerVersion.h

  miLoggingLogger.h
  miLoggingMemory.h
  miLoggingScopes.h
  miLoggingSystem.h
)

IF (${PC_LOG4CPP_FOUND})
  ADD_DEFINITIONS(${PC_LOG4CPP_CFLAGS_OTHER} -DHAVE_LOG4CPP)
  INCLUDE_DIRECTORIES(${PC_LOG4CPP_INCLUDE_DIRS})
  LINK_DIRECTORIES(${PC_LOG4CPP_LIBRARY_DIRS})
  LIST(APPEND milogger_SOURCES
    miLoggingLog4Cpp.cc
  )
  LIST(APPEND milogger_HEADERS
    miLoggingLog4Cpp.h
  )
ENDIF()

########################################################################

ADD_LIBRARY(milogger SHARED
  ${milogger_SOURCES}

  # FIXME avoid mentioning these files twice -- they have to be here for IDEs
  miLogging.h
  miLoggerVersion.h
)

SET_TARGET_PROPERTIES(milogger PROPERTIES
  VERSION ${MILOGGER_PVERSION_FULL}
  SOVERSION "${MILOGGER_VERSION_MAJOR}"
  OUTPUT_NAME "${lib_name}"
)

TARGET_LINK_LIBRARIES(milogger
  ${PC_LOG4CPP_LIBRARIES}
  ${Boost_LIBRARIES}
)

INSTALL(TARGETS milogger
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

INSTALL(FILES ${milogger_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/metlibs/miLogger
)
