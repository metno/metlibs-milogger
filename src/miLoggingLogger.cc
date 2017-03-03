
#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

namespace milogger {

Record::Record()
{
}

Logger::Logger()
{
}

LoggerPtr LoggerTag::logger()
{
  if (std::atomic_load(&mLogger) == 0) {
    system::lock_guard_t guard(system::tags_mutex);
    if (std::atomic_load(&mLogger) == 0) {
      LoggerPtr logger(getLoggerByTag(mTag, mTagExtra));
      system::registerTag(this);
      std::atomic_store(&mLogger, logger);
    }
  }
  return mLogger;
}

void LoggerTag::reset()
{
  if (std::atomic_load(&mLogger) != 0) {
    system::lock_guard_t guard(system::tags_mutex);
    if (std::atomic_load(&mLogger) != 0) {
      LoggerPtr logger;
      system::unregisterTag(this);
      std::atomic_store(&mLogger, logger);
    }
  }
}

} // namespace milogger
