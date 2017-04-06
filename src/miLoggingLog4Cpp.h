
#ifndef MILOGGER_LOGGING_LOG4CPP_HH
#define MILOGGER_LOGGING_LOG4CPP_HH 1

#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

namespace milogger {
namespace l4c {

class L4CSystem : public system::System {
public:
  void configure(const std::string&) override;
  void addSink(const std::string& sinkoptions) override;
  LoggerPtr getLogger(const std::string& tag) override;
  void shutdown() override;
};

} // namespace l4c
} // namespace milogger

#endif // MILOGGER_LOGGING_LOG4CPP_HH
