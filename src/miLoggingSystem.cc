
#include "miLoggingLogger.h"

#if defined(HAVE_LOG4CPP)
#include "miLoggingLog4Cpp.h"
#endif
#include "miLoggingNone.h"
#include "miLoggingSimple.h"
#include "miLoggingSystem.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace milogger {
namespace system {

namespace /*anonymous*/ {

SystemPtr selectSystemFromEnvVar()
{
  std::string use;
  if (const char* use_c = getenv("MILOGGER_USE"))
    use = use_c;

#if defined(HAVE_LOG4CPP)
  if (use.empty() || use == "LOG4CPP" || use == "log4cpp") {
    return std::make_shared<milogger::l4c::L4CSystem>();
  }
#endif

  if (use == "NONE" || use == "none") {
    return std::make_shared<milogger::none::NoSystem>();
  }

  return std::make_shared<milogger::simple::SimpleSystem>();
}

SystemPtr _selectedSystem;

std::string joinTagNames(const std::string& tag, const char* tag_extra)
{
  if (tag.empty() || !tag_extra)
    return tag;
  else
    return (tag + "." + tag_extra);
}

} /*anonymous namespace*/

// ########################################################################

System::System()
{
}

System::~System()
{
}

void System::configure(const std::string&)
{
}

void System::addSink(const std::string&)
{
}

void System::shutdown()
{
}

// ========================================================================

std::recursive_mutex tags_mutex;
std::vector<LoggerTag*> loggerTags;

void registerTag(LoggerTag* loggertag)
{
  milogger::system::lock_guard_t guard(tags_mutex);
  loggerTags.push_back(loggertag);
}

void unregisterTag(LoggerTag* loggertag)
{
  milogger::system::lock_guard_t guard(tags_mutex);
  loggerTags.erase(std::remove(loggerTags.begin(), loggerTags.end(), loggertag),
      loggerTags.end());
}

void resetAllTags()
{
  milogger::system::lock_guard_t guard(tags_mutex);
  std::vector<LoggerTag*> lt;
  std::swap(lt, loggerTags); // avoid long erase in unregisterTag

  for (LoggerTag* lt1 : loggerTags)
    lt1->reset();
}

void selectSystem(system::SystemPtr system)
{
  resetAllTags();
  _selectedSystem = system;
}

system::SystemPtr selectedSystem()
{
  if (!_selectedSystem)
    selectSystem(selectSystemFromEnvVar());
  return _selectedSystem;
}

} // namespace system

// ########################################################################

using namespace milogger::system;

LoggerPtr getLoggerByTag(const char* tag, const char* tag_extra)
{
  const std::string t = joinTagNames(tag, tag_extra);
  return selectedSystem()->getLogger(t);
}

LoggingConfig::LoggingConfig(const std::string& s)
{
  selectedSystem()->configure(s);
}

void LoggingConfig::addConfig(const std::string& c)
{
  selectedSystem()->configure(c);
}

void LoggingConfig::addSink(const std::string& sinkoptions)
{
  selectedSystem()->addSink(sinkoptions);
}

LoggingConfig::~LoggingConfig()
{
  resetAllTags();
  selectedSystem()->shutdown();
  _selectedSystem = system::SystemPtr();
}

} // namespace milogger
