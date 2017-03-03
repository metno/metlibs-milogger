
#ifndef MILOGGER_LOGGING_HH
#define MILOGGER_LOGGING_HH 1

#include "miLoggingLogger.h"

#ifndef MILOGGER_CATEGORY
#warning you should define 'MILOGGER_CATEGORY' before including miLogging.h, setting to default value "milogger"
#define MILOGGER_CATEGORY "milogger"
#endif

namespace /* anonymous */ {
// this will most likely be initialized before "main"
::milogger::LoggerTag MILOGGER_logger(MILOGGER_CATEGORY);
} // anonymous namespace

#define MILOGGER_DETAIL_LOG_CLM(logger, severity, message)              \
  do {                                                                  \
    if (milogger::RecordPtr MILOGGER_record = logger.createRecord(::milogger::severity)) { \
      MILOGGER_record->stream() << message;                             \
      logger.submitRecord(MILOGGER_record);                            \
    }                                                                   \
  } while(false)

#define MILOGGER_DETAIL_LOG_LM(severity, message)               \
    MILOGGER_DETAIL_LOG_CLM(MILOGGER_logger, severity, message)

#define METLIBS_LOG_FATAL(message)              \
    MILOGGER_DETAIL_LOG_LM(FATAL, message)
#define METLIBS_LOG_ERROR(message)              \
    MILOGGER_DETAIL_LOG_LM(ERROR, message)
#define METLIBS_LOG_WARN(message)               \
    MILOGGER_DETAIL_LOG_LM(WARN, message)
#define METLIBS_LOG_INFO(message)               \
    MILOGGER_DETAIL_LOG_LM(INFO, message)
#define METLIBS_LOG_DEBUG(message)              \
    MILOGGER_DETAIL_LOG_LM(DEBUG, message)
#define METLIBS_LOG_DEBUG_ENABLED()                     \
  (!!MILOGGER_logger.createRecord(::milogger::DEBUG))

#define LOGVAL(x) " " #x "='" << x << "'"


#include "miLoggingScopes.h"

#endif // MILOGGER_LOGGING_HH
