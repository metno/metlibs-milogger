
#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace /* anonymous*/ {

log4cpp::Category& getCategoryL4C(const std::string& name)
{
  if (name.empty())
    return log4cpp::Category::getRoot();
  else
    return log4cpp::Category::getInstance(name);
}

log4cpp::Priority::PriorityLevel levelToL4C(::milogger::detail::PriorityLevel pl)
{
  switch (pl) {
  case ::milogger::detail::VERBOSE: return log4cpp::Priority::DEBUG;
  case ::milogger::detail::DEBUG:   return log4cpp::Priority::DEBUG;
  case ::milogger::detail::INFO :   return log4cpp::Priority::INFO;
  case ::milogger::detail::WARN :   return log4cpp::Priority::WARN;
  case ::milogger::detail::ERROR:   return log4cpp::Priority::ERROR;
  case ::milogger::detail::FATAL:   return log4cpp::Priority::FATAL;
  }
  return log4cpp::Priority::FATAL;
}

} // anonymous namespace

// ########################################################################

namespace milogger {

LoggingConfig::LoggingConfig(const std::string& l4c_p)
{
  struct stat sb;
  if (stat(l4c_p.c_str(), &sb) == 0) {
    log4cpp::PropertyConfigurator::configure(l4c_p);
  } else {
    log4cpp::Appender *a = new log4cpp::OstreamAppender("rootAppender", &std::cout);
    log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("%d %p %c: %m%n");
    a->setLayout(layout);

    log4cpp::Category& root = log4cpp::Category::getRoot();
    root.setPriority(log4cpp::Priority::WARN);
    root.addAppender(a);
  }
}

LoggingConfig::~LoggingConfig()
{
  log4cpp::Category::shutdown();
}

} // namespace milogger

// ########################################################################

namespace milogger {
namespace detail {

struct CategoryPrivate {
  CategoryPrivate(log4cpp::Category& c)
    : mCategory(c) { }
  log4cpp::Category& mCategory;
};

// ------------------------------------------------------------------------

Category::Category(std::string c)
  : p(new CategoryPrivate(getCategoryL4C(c)))
{
}

// ------------------------------------------------------------------------

bool Category::isLoggingEnabled(PriorityLevel pl)
{
  return p->mCategory.isPriorityEnabled(levelToL4C(pl));
}

void Category::log(PriorityLevel pl, const std::string& message)
{
  p->mCategory.getStream(levelToL4C(pl)) << message;
}

} // namespace detail
} // namespace milogger
