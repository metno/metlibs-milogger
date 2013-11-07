
#include <cassert>
#include <iostream>

namespace milogger {
namespace detail {

static bool initialized = false;
static PriorityLevel PRIORITY = WARN;

// ------------------------------------------------------------------------

struct CategoryPrivate {
  CategoryPrivate(const std::string& c)
    : mCategory(c) { }
  std::string mCategory;
};

// ------------------------------------------------------------------------

Category::Category(std::string c)
  : p(new CategoryPrivate(c))
{
}

// ------------------------------------------------------------------------

bool Category::isLoggingEnabled(PriorityLevel pl)
{
  return pl >= PRIORITY;
}

// ------------------------------------------------------------------------

void Category::log(PriorityLevel pl, const std::string& message)
{ 
  // otherwise we do not know that std::cerr is initialized
  if (!initialized)
    return;

  std::cerr << p->mCategory << ' ';
  switch (pl) {
  case VERBOSE: std::cerr << "VERBOSE"; break;
  case DEBUG:   std::cerr << "DEBUG  "; break;
  case INFO :   std::cerr << "INFO   "; break;
  case WARN :   std::cerr << "WARN   "; break;
  case ERROR:   std::cerr << "ERROR  "; break;
  case FATAL:   std::cerr << "FATAL  "; break;
  }
  std::cerr << ": " << message << std::endl;
}

} // namespace detail
} // namespace milogger

// ########################################################################

namespace milogger {

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
