
#ifndef MILOGGER_LOGGING_NONE_HH
#define MILOGGER_LOGGING_NONE_HH 1

#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

namespace milogger {
namespace none {

class NoLogger : public ::milogger::Logger {
public:
  bool isEnabledFor(Severity severity) override;
  RecordPtr createRecord(Severity severity) override;
  void submitRecord(RecordPtr record) override;
};

// ########################################################################

class NoSystem : public ::milogger::system::System {
public:
  LoggerPtr getLogger(const std::string& tag) override;

private:
  static std::mutex sMutex;
  static std::shared_ptr<NoLogger> sLogger;
};

} // namespace none
} // namespace milogger

#endif // MILOGGER_LOGGING_NONE_HH
