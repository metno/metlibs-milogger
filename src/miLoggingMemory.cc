
#include "miLoggingMemory.h"

#include "miLoggingStringRecord.h"
#include "miLoggingUtils.h"

#include <iostream>

namespace milogger {
namespace memory {

class MemoryLogger : public Logger
{
public:
  MemoryLogger(const std::string& tag)
      : threshold_(milogger::INFO)
      , tag_(tag)
  {
  }

  ~MemoryLogger() {}

  bool isEnabledFor(milogger::Severity severity) override { return (severity >= threshold_); }
  milogger::RecordPtr createRecord(milogger::Severity severity) override;
  void submitRecord(milogger::RecordPtr record) override;

private:
  const milogger::Severity threshold_;
  const std::string tag_;
};

// ########################################################################

milogger::RecordPtr MemoryLogger::createRecord(milogger::Severity severity)
{
  return isEnabledFor(severity) ? milogger::record::allocateStringRecord(severity) : milogger::RecordPtr();
}

void MemoryLogger::submitRecord(milogger::RecordPtr record)
{
  milogger::record::StringRecordPtr sr = std::static_pointer_cast<milogger::record::StringRecord>(record);
  MemorySystem::instance()->push(sr->severity(), tag_, sr->text());
}

// ########################################################################

MemorySystem::MemorySystem()
    : print_(false)
{
  instance_ = this;
}

MemorySystem::~MemorySystem()
{
  instance_ = nullptr;
}

MemorySystem* MemorySystem::instance_;

milogger::LoggerPtr MemorySystem::getLogger(const std::string& tag)
{
  return std::make_shared<MemoryLogger>(tag);
}

void MemorySystem::push(milogger::Severity severity, const std::string& tag, const std::string& message)
{
  std::lock_guard<std::mutex> guard(messages_mutex_);
  messages_.push_back({severity, tag, message});
  if (print_)
    std::cerr << milogger::textFromSeverity(severity) << ' ' << tag << ' ' << message << std::endl;
}

void MemorySystem::clear()
{
  std::lock_guard<std::mutex> guard(messages_mutex_);
  messages_.clear();
}

} // namespace memory
} // namespace milogger
