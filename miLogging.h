
#ifndef MILOGGER_LOGGING_HH
#define MILOGGER_LOGGING_HH 1

#include <memory>
#include <sstream>
#include <string>
#include <sys/time.h>

namespace milogger {
namespace detail {

enum PriorityLevel { VERBOSE, DEBUG, INFO, WARN, ERROR, FATAL };

class CategoryPrivate;

//! Category keeps a logging category and provides a logger for that category on demand.
class Category {
public:
  Category(std::string cat);

  bool isLoggingEnabled(PriorityLevel level);
  void log(PriorityLevel level, const std::string& message);

private:
  std::auto_ptr<CategoryPrivate> p;
};
typedef Category& CategoryRef;

Category getCategory(std::string name);

class LogScope {
public:
  LogScope(CategoryRef cat, const char* fun);
  ~LogScope();

protected:
  CategoryRef mCategory;
  const char* mFunction;
  std::string mExitMessage;
};

class LogTime : public LogScope {
public:
  LogTime(CategoryRef cat, const char* fun);
  ~LogTime();
private:
  struct timeval pre;
};

std::string getCategoryName(const std::string& c, const std::string& f);

} // namespace detail

class LoggingConfig {
public:
  LoggingConfig(const std::string& configuration="");
  ~LoggingConfig();
};

} // namespace milogger

#ifndef MILOGGER_CATEGORY
#warning you should define 'MILOGGER_CATEGORY' before including miLogging.h, setting to default value "milogger"
#define MILOGGER_CATEGORY "milogger"
#endif

namespace /* anonymous */ {
::milogger::detail::Category INTERNAL_milogger(MILOGGER_CATEGORY);
} // anonymous namespace

#define MILOGGER_DETAIL_LOG_CLM(logcategory, loglevel, logmessage)      \
  do {                                                                  \
    if (logcategory.isLoggingEnabled(::milogger::detail::loglevel)) {   \
      std::ostringstream logmessagestream;                              \
      logmessagestream << logmessage;                                   \
      logcategory.log(::milogger::detail::loglevel, logmessagestream.str()); \
    }                                                                   \
  } while(false)


#define MILOGGER_DETAIL_LOG_LM(loglevel, logmessage)                    \
    MILOGGER_DETAIL_LOG_CLM(INTERNAL_milogger, loglevel, logmessage)

#define METLIBS_LOG_FATAL(logmessage)              \
    MILOGGER_DETAIL_LOG_LM(FATAL, logmessage)
#define METLIBS_LOG_ERROR(logmessage)              \
    MILOGGER_DETAIL_LOG_LM(ERROR, logmessage)
#define METLIBS_LOG_WARN(logmessage)               \
    MILOGGER_DETAIL_LOG_LM(WARN, logmessage)
#define METLIBS_LOG_INFO(logmessage)               \
    MILOGGER_DETAIL_LOG_LM(INFO, logmessage)
#define METLIBS_LOG_DEBUG(logmessage)              \
    MILOGGER_DETAIL_LOG_LM(DEBUG, logmessage)

#define MILOGGER_DETAIL_LOG_SCOPE(cls)                                  \
  ::milogger::detail::Category INTERNAL_milogger(::milogger::detail::getCategoryName(MILOGGER_CATEGORY, __func__)); \
  const ::milogger::detail::cls MILOGGER_scope(INTERNAL_milogger, __PRETTY_FUNCTION__)

// modified (maybe wrongly) from http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
#define MILOGGER_DETAIL_ARG3(arg1, arg2, arg3, ...) arg3
#define MILOGGER_DETAIL_SWITCH_HAS_ARG(YES, NO, ...) MILOGGER_DETAIL_ARG3(__VA_ARGS__, NO, YES)
#define MILOGGER_DETAIL_MAKE_COMMA(...) ,
#define MILOGGER_DETAIL_CALL_IF_ARGS(CALL, ...)                        \
  MILOGGER_DETAIL_SWITCH_HAS_ARG(CALL, , MILOGGER_DETAIL_MAKE_COMMA __VA_ARGS__ ())

#define METLIBS_LOG_SCOPE(...)                                          \
  MILOGGER_DETAIL_LOG_SCOPE(LogScope)                                   \
  MILOGGER_DETAIL_CALL_IF_ARGS(; METLIBS_LOG_DEBUG(__VA_ARGS__), __VA_ARGS__)

#define METLIBS_LOG_TIME(...)                                           \
  MILOGGER_DETAIL_LOG_SCOPE(LogTime)                                    \
  MILOGGER_DETAIL_CALL_IF_ARGS(; METLIBS_LOG_DEBUG(__VA_ARGS__), __VA_ARGS__)

#define LOGVAL(x) " " #x "='" << x << "'"

#endif // MILOGGER_LOGGING_HH
