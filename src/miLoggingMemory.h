#ifndef MILOGGER_LOGGING_MEMORY_HH
#define MILOGGER_LOGGING_MEMORY_HH 1

#include "miLoggingLogger.h"
#include "miLoggingSystem.h"

#include <map>
#include <vector>

namespace milogger {
namespace memory {

class MemorySystem : public system::System
{
public:
  struct Message
  {
    milogger::Severity severity;
    std::string tag;
    std::string message;
  };
  typedef std::vector<Message> messages_v;

public:
  MemorySystem();
  ~MemorySystem();

  void configure(const std::string&) override { clear(); }
  void addSink(const std::string&) override {}
  milogger::LoggerPtr getLogger(const std::string& tag) override;
  void shutdown() override { clear(); }

  void print(bool p) { print_ = p; }

  void push(milogger::Severity severity, const std::string& tag, const std::string& message);
  void clear();
  const messages_v& messages() const { return messages_; }

  static MemorySystem* instance() { return instance_; }

protected:
  static MemorySystem* instance_;

  bool print_;
  std::mutex messages_mutex_;
  messages_v messages_;
};

typedef std::shared_ptr<MemorySystem> MemorySystem_p;

} // namespace memory
} // namespace milogger

#endif // MILOGGER_LOGGING_SIMPLE_HH
