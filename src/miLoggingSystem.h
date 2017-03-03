
#ifndef MILOGGER_LOGGING_SYSTEM_HH
#define MILOGGER_LOGGING_SYSTEM_HH 1

#include "miLoggingLogger.h"

#include <mutex>

namespace milogger {
namespace system {

typedef std::recursive_mutex mutex_t;
typedef std::lock_guard<mutex_t> lock_guard_t;

extern mutex_t tags_mutex;

void registerTag(LoggerTag* loggertag);
void unregisterTag(LoggerTag* loggertag);
void resetAllTags();


class System {
public:
  System();
  virtual ~System();

  virtual void configure(const std::string&);
  virtual void addSink(const std::string& sinkoptions);
  virtual LoggerPtr getLogger(const std::string& tag) = 0;
  virtual void shutdown();
};

typedef std::shared_ptr<System> SystemPtr;


extern void selectSystem(SystemPtr system);
extern SystemPtr selectedSystem();

} // namespace system
} // namespace milogger

#endif // MILOGGER_LOGGING_SYSTEM_HH
