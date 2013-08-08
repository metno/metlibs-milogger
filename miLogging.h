
#ifndef MILOGGER_LOGGING_HH
#define MILOGGER_LOGGING_HH 1

// FIXME this way of providing logging functions might not be suitable for a shared lib

#include <sstream>
#include <string>
#include <sys/time.h>

// ########################################################################
// ########## log4cpp version
// ########################################################################

#if !defined(NO_LOG4CPP) || defined(HAVE_LIBLOG4CPP)
#include <log4cpp/Priority.hh>

namespace log4cpp {
class Category;
}

namespace milogger {
namespace detail {
typedef log4cpp::Category& Category;
typedef log4cpp::Priority::PriorityLevel PriorityLevel;
typedef log4cpp::Priority Priority;
} // namespace detail
} // namespace milogger

#else // !NO_LOG4CPP

// ########################################################################
// ########## iostream version -- not thread-safe at present
// ########################################################################

namespace milogger {
namespace detail {
typedef std::string Category;
struct Priority { typedef enum { DEBUG, INFO, WARN, ERROR, FATAL } PriorityLevel; };
typedef Priority::PriorityLevel PriorityLevel;
} // namespace detail
} // namespace milogger

#endif // NO_LOG4CPP

// ########################################################################
// ########## end of log4cpp switch
// ########################################################################

namespace milogger {

class LoggingConfig {
public:
  LoggingConfig(const std::string& configuration="");
  ~LoggingConfig();
};

namespace detail {

Category getCategory(const std::string& categoryName="", const std::string& functionName="");
bool isLoggingEnabled(Category category, PriorityLevel level);
void logMessage(Category category, PriorityLevel level, const std::string& message);

class LogScope {
public:
  LogScope(Category cat, const char* fun);
  ~LogScope();
protected:
  Category mCategory;
  const char* mFunction;
};

class LogTime : public LogScope {
public:
  LogTime(Category cat, const char* fun);
  ~LogTime();
private:
  struct timeval pre;
};

} // namespace detail
} // namespace milogger

#ifndef MILOGGER_CATEGORY
#warning you should define 'MILOGGER_CATEGORY' before including miLogging.h, setting to default value "milogger"
#define MILOGGER_CATEGORY "milogger"
#endif

namespace /* anonymous */ {
::milogger::detail::Category INTERNAL_milogger = ::milogger::detail::getCategory(MILOGGER_CATEGORY);
} // anonymous namespace

#define MILOGGER_DETAIL_LOG_CLM(logcategory, loglevel, logmessage)      \
  do {                                                                  \
    if (::milogger::detail::isLoggingEnabled(logcategory, ::milogger::detail::Priority::loglevel)) { \
      std::ostringstream logmessagestream;                              \
      logmessagestream << logmessage;                                   \
      ::milogger::detail::logMessage(logcategory, ::milogger::detail::Priority::loglevel, logmessagestream.str()); \
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
  ::milogger::detail::Category INTERNAL_milogger = ::milogger::detail::getCategory(MILOGGER_CATEGORY, __func__); \
  const ::milogger::detail::cls MILOGGER_scope(INTERNAL_milogger, __PRETTY_FUNCTION__)

#define METLIBS_LOG_SCOPE()                     \
  MILOGGER_DETAIL_LOG_SCOPE(LogScope)

#ifdef M_TIME
#define METLIBS_LOG_TIME()                      \
  MILOGGER_DETAIL_LOG_SCOPE(LogTime)
#else // M_TIME
#define METLIBS_LOG_TIME()                      \
  METLIBS_LOG_SCOPE()
#endif // M_TIME

#define LOGVAL(x) " " #x "='" << x << "'"

#endif // MILOGGER_LOGGING_HH
