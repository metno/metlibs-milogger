
#include "miLoggingSimple.h"

#include "miLoggingStringRecord.h"
#include "miLoggingUtils.h"

#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

namespace milogger {
namespace simple {

namespace /*anonymous*/ {

void no_delete(std::ostream*) {}

std::size_t get_thread_id() noexcept
{
  static std::atomic<std::size_t> id_counter(0);
  thread_local std::size_t this_thread_id = id_counter;
  id_counter += 1;
  return this_thread_id;
}

} /*anonymous namespace*/

// ########################################################################

bool OstreamLogger::isEnabledFor(Severity severity)
{
  return (severity >= threshold_);
}

RecordPtr OstreamLogger::createRecord(Severity severity)
{
  if (severity >= threshold_)
    return record::allocateStringRecord(severity);
  else
    return RecordPtr();
}

void OstreamLogger::submitRecord(RecordPtr record)
{
  record::StringRecordPtr sr = std::static_pointer_cast<record::StringRecord>(record);

  std::ostringstream fmt;

  {
    const auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    static const auto start = now;
    const auto diff = now - start;
    fmt << std::setw(10) << diff.count(); // 10 digits should be enough for about 3 months
  }

  fmt << " {" << std::setw(3) << get_thread_id() << "}";

  const char* sev = textFromSeverity(sr->severity());
  fmt << ' ' << sev << ' ' << tag_ << ": " << sr->text() << std::endl;

  SimpleSystem::instance()->write(fmt.str());
}

// ########################################################################

SimpleSystem* SimpleSystem::instance_;

SimpleSystem::SimpleSystem()
  : threshold_(WARN)
{
  instance_ = this;
}

SimpleSystem::~SimpleSystem()
{
  instance_ = 0;
}

void SimpleSystem::configure(const std::string& file)
{
  { std::lock_guard<std::mutex> guard(sinks_mutex_);
    if (sinks_.empty())
      sinks_.push_back(ostream_ptr(&std::clog, no_delete));
  }

  std::ifstream config(file);
  parseTagThresholds(threshold_, tag_thresholds_, config);
}

void SimpleSystem::addSink(const std::string& sinkoptions)
{
  std::shared_ptr<std::ofstream> file = std::make_shared<std::ofstream>();
  file->open(sinkoptions.c_str());
  if (file->is_open()) {
    std::lock_guard<std::mutex> guard(sinks_mutex_);
    sinks_.push_back(file);
  }
}

LoggerPtr SimpleSystem::getLogger(const std::string& tag)
{
  Severity threshold = findTagThreshold(threshold_, tag_thresholds_, tag);
  return std::make_shared<simple::OstreamLogger>(threshold, tag);
}

void SimpleSystem::shutdown()
{
  std::lock_guard<std::mutex> guard(sinks_mutex_);
  sinks_.clear();
}

void SimpleSystem::setThreshold(Severity threshold)
{
  threshold_ = threshold;
  milogger::system::resetAllTags();
}

void SimpleSystem::write(const std::string& formatted)
{
  const size_t nchars = formatted.size();
  if (nchars == 0 || sinks_.empty())
    return;

  std::lock_guard<std::mutex> guard(sinks_mutex_);
  const char* chars = formatted.c_str();
  for(sinks_v::iterator it = sinks_.begin(); it != sinks_.end(); ++it) {
    (*it)->write(chars, nchars);
    (*it)->flush();
  }
}

} // namespace simple
} // namespace milogger
