
#ifndef MILOGGER_LOGGING_SCOPES_HH
#define MILOGGER_LOGGING_SCOPES_HH 1

#include "miLoggingLogger.h"

namespace milogger {
namespace scopes {

class LogEnterExitScope : nocopies {
public:
  LogEnterExitScope(LoggerTag& logger, Severity severity, const char* scopename);
  ~LogEnterExitScope();

protected:
  LoggerTag& mLogger;
  Severity mSeverity;
  const char* mScopeName;
  double mTime;
};


class LogEnterExitTime : public LogEnterExitScope {
public:
  LogEnterExitTime(LoggerTag& logger, Severity severity, const char* scopename);
  ~LogEnterExitTime();
};

} // namespace scopes
} // namespace milogger

// ########################################################################

#ifndef MILOGGER_DISABLE_FUNCTION_LOGGERS
#define MILOGGER_DETAIL_LOG_SCOPE(cls)                                  \
  static ::milogger::LoggerTag MILOGGER_logger(MILOGGER_CATEGORY, __func__); \
  const ::milogger::cls MILOGGER_scope(MILOGGER_logger, ::milogger::DEBUG, __PRETTY_FUNCTION__)
#else // !MILOGGER_DISABLE_FUNCTION_LOGGERS
#define MILOGGER_DETAIL_LOG_SCOPE(cls)                                  \
  const ::milogger::cls MILOGGER_scope(MILOGGER_logger, ::milogger::DEBUG, __PRETTY_FUNCTION__)
#endif // !MILOGGER_DISABLE_FUNCTION_LOGGERS

// modified (maybe wrongly) from http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
#define MILOGGER_DETAIL_ARG3(arg1, arg2, arg3, ...) arg3
#define MILOGGER_DETAIL_SWITCH_HAS_ARG(YES, NO, ...) MILOGGER_DETAIL_ARG3(__VA_ARGS__, NO, YES)
#define MILOGGER_DETAIL_MAKE_COMMA(...) ,
#define MILOGGER_DETAIL_CALL_IF_ARGS(CALL, ...)                         \
  MILOGGER_DETAIL_SWITCH_HAS_ARG(CALL, , MILOGGER_DETAIL_MAKE_COMMA __VA_ARGS__ ())

#define METLIBS_LOG_SCOPE(...)                                          \
  MILOGGER_DETAIL_LOG_SCOPE(scopes::LogEnterExitScope)                  \
  MILOGGER_DETAIL_CALL_IF_ARGS(; METLIBS_LOG_DEBUG(__VA_ARGS__), __VA_ARGS__)

#define METLIBS_LOG_TIME(...)                                           \
  MILOGGER_DETAIL_LOG_SCOPE(scopes::LogEnterExitTime)                   \
  MILOGGER_DETAIL_CALL_IF_ARGS(; METLIBS_LOG_DEBUG(__VA_ARGS__), __VA_ARGS__)

#endif // MILOGGER_LOGGING_SCOPES_HH
