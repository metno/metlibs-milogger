
#ifndef MILOGGER_LOGGING_SIMPLE_HH
#define MILOGGER_LOGGING_SIMPLE_HH 1

#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

#include <map>
#include <vector>

namespace milogger {
namespace simple {

class OstreamLogger : public ::milogger::Logger {
public:
  OstreamLogger(Severity threshold, const std::string& tag)
    : threshold_(threshold), tag_(tag) { }

  ~OstreamLogger() { }

  bool isEnabledFor(Severity severity) override;
  RecordPtr createRecord(Severity severity) override;
  void submitRecord(RecordPtr record) override;

private:
  const Severity threshold_;
  const std::string tag_;
};

// ########################################################################

class SimpleSystem : public system::System {
public:
  SimpleSystem();
  ~SimpleSystem();

  void configure(const std::string&);
  void addSink(const std::string& sinkoptions);
  LoggerPtr getLogger(const std::string& tag);
  void shutdown();

  void write(const std::string& formatted);

  static SimpleSystem* instance()
    { return instance_; }

  void setThreshold(Severity threshold);

protected:
  typedef std::shared_ptr<std::ostream> ostream_ptr;
  typedef std::vector<ostream_ptr> sinks_v;

protected:
  typedef std::map<std::string, Severity> tag_threshold_m;

  static SimpleSystem* instance_;

  Severity threshold_;
  tag_threshold_m tag_thresholds_;
  std::mutex sinks_mutex_;
  sinks_v sinks_;
};

typedef std::shared_ptr<SimpleSystem> SimpleSystemPtr;

} // namespace simple
} // namespace milogger

#endif // MILOGGER_LOGGING_SIMPLE_HH
