
#define MILOGGER_CATEGORY "miLogger.miLogging" // get rid of warning
#include "miLogging.h"

#include <iomanip>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace milogger {
namespace detail {

LogScope::LogScope(Category cat, const char* fun)
    : mCategory(cat)
    , mFunction(fun)
{
  MILOGGER_DETAIL_LOG_CLM(mCategory, INFO, "++ enter " << mFunction);
}

LogScope::~LogScope()
{
  MILOGGER_DETAIL_LOG_CLM(mCategory, INFO, "++ leave " << mFunction);
}

LogTime::LogTime(Category cat, const char* fun)
  : LogScope(cat, fun)
{
    gettimeofday(&pre, 0);
}

LogTime::~LogTime()
{
  struct timeval post;
  gettimeofday(&post, 0);
  const double s = (((double)post.tv_sec*1000000.0 + (double)post.tv_usec)-((double)pre.tv_sec*1000000.0 + (double)pre.tv_usec))/1000000.0;
  MILOGGER_DETAIL_LOG_CLM(mCategory, INFO, "-- spent " << std::setprecision(6) << std::setw(10) << s << "s");
}

} // namespace detail
} // namespace milogger

namespace /* anonymous*/ {
std::string getCategoryName(const std::string& c, const std::string& f)
{
  if (c.empty())
    return "";
  else if (f.empty())
    return c;
  else
    return (c + "." + f);
}
} // namespace anonymous

// ########################################################################
// ########## log4cpp version
// ########################################################################

#if !defined(NO_LOG4CPP) || defined(HAVE_LIBLOG4CPP)

#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>

#include <boost/foreach.hpp>

namespace /* anonymous */ {

class FlushingOstreamAppender : public log4cpp::OstreamAppender {
public:
    FlushingOstreamAppender(const std::string& name, std::ostream* stream)
        : OstreamAppender(name, stream) { }

protected:
    virtual void _append(const log4cpp::LoggingEvent& event)
        { log4cpp::OstreamAppender::_append(event); if (_stream) (*_stream) << std::flush; }

};

log4cpp::Appender* makeConsoleAppender()
{
    log4cpp::Appender *a = new FlushingOstreamAppender("console", &std::cout);
    log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("%d %p %c: %m%n");
    a->setLayout(layout);
    return a;
}

void fixConsoleLoggers()
{
    std::vector<log4cpp::Category*>* allCat = log4cpp::Category::getCurrentCategories();
    if (not allCat)
        return;
    
    BOOST_FOREACH(log4cpp::Category* cat, *allCat) {
        // cat->setAdditivity(false);
        log4cpp::AppenderSet allApp = cat->getAllAppenders();
        BOOST_FOREACH(log4cpp::Appender* app, allApp) {
            log4cpp::OstreamAppender* oApp = dynamic_cast<log4cpp::OstreamAppender*>(app);
            if (!oApp)
                continue;
            cat->removeAppender(oApp);
            cat->addAppender(makeConsoleAppender());
        }
    }
}

} // anonymous namespace

namespace milogger {

LoggingConfig::LoggingConfig(const std::string& l4c_p)
{
    struct stat sb;
    if (stat(l4c_p.c_str(), &sb) == 0) {
        log4cpp::PropertyConfigurator::configure(l4c_p);
        fixConsoleLoggers();
    } else {
        log4cpp::Appender *a = makeConsoleAppender();

        log4cpp::Category& root = log4cpp::Category::getRoot();
        root.setPriority(log4cpp::Priority::WARN);
        root.addAppender(a);
    }
}

LoggingConfig::~LoggingConfig()
{
    log4cpp::Category::shutdown();
}

namespace detail {

Category getCategory(const std::string& c, const std::string& f)
{
  if (c.empty())
    return log4cpp::Category::getRoot();
  else 
    return log4cpp::Category::getInstance(getCategoryName(c, f));
}

bool isLoggingEnabled(Category c, PriorityLevel p)
{
  return c.isPriorityEnabled(p);
}

void logMessage(Category c, PriorityLevel p, const std::string& message)
{
  c.getStream(p) << message;
}

} // namespace detail
} // namespace milogger

#else // !NO_LOG4CPP

// ########################################################################
// ########## iostream version -- not thread-safe at present
// ########################################################################

#include <cassert>
#include <iostream>

namespace milogger {

namespace detail {

bool initialized = false;
PriorityLevel PRIORITY = Priority::WARN;

Category getCategory(const std::string& c, const std::string& f)
{
  return getCategoryName(c, f);
}

bool isLoggingEnabled(Category, PriorityLevel p)
{
  return p >= PRIORITY;
}

void logMessage(Category c, PriorityLevel p, const std::string& message)
{
  // otherwise we do not know that std::cerr is initialized
  if (!initialized)
    return;

  std::cerr << c << ' ';
  switch (p) {
  case Priority::DEBUG: std::cerr << "DEBUG"; break;
  case Priority::INFO : std::cerr << "INFO "; break;
  case Priority::WARN : std::cerr << "WARN "; break;
  case Priority::ERROR: std::cerr << "ERROR"; break;
  case Priority::FATAL: std::cerr << "FATAL"; break;
  }
  std::cerr << ": " << message << std::endl;
}

} // namespace detail

LoggingConfig::LoggingConfig(const std::string&)
{
  assert(not detail::initialized);
  detail::initialized = true;
}

LoggingConfig::~LoggingConfig()
{
  assert(detail::initialized);
  detail::initialized = false;
}

} // namespace milogger

#endif // NO_LOG4CPP

// ########################################################################
// ########## end of log4cpp switch
// ########################################################################
