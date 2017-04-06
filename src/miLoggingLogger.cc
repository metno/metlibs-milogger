
#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

#ifndef MILOGGER_DO_NOT_USE_STD_ATOMIC
#include <atomic>
#endif

namespace milogger {

Record::Record()
{
}

Logger::Logger()
{
}

LoggerPtr LoggerTag::logger()
{
#ifndef MILOGGER_DO_NOT_USE_STD_ATOMIC
  if (std::atomic_load(&mLogger) == 0) {
    system::lock_guard_t guard(system::tags_mutex);
    if (std::atomic_load(&mLogger) == 0) {
      LoggerPtr logger(getLoggerByTag(mTag, mTagExtra));
      system::registerTag(this);
      std::atomic_store(&mLogger, logger);
    }
  }
#else
  system::lock_guard_t guard(system::tags_mutex);
  if (!mLogger) {
    LoggerPtr logger(getLoggerByTag(mTag, mTagExtra));
    system::registerTag(this);
    mLogger = logger;
  }
#endif
  return mLogger;
}

void LoggerTag::reset()
{
#ifndef MILOGGER_DO_NOT_USE_STD_ATOMIC
  if (std::atomic_load(&mLogger) != 0) {
    system::lock_guard_t guard(system::tags_mutex);
    if (std::atomic_load(&mLogger) != 0) {
      LoggerPtr logger;
      system::unregisterTag(this);
      std::atomic_store(&mLogger, logger);
    }
  }
#else
  system::lock_guard_t guard(system::tags_mutex);
  if (mLogger) {
    system::unregisterTag(this);
    mLogger = LoggerPtr();
  }
#endif
}

} // namespace milogger
