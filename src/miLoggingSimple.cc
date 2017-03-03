
#include "miLoggingSimple.h"

#include "miLoggingStringRecord.h"
#include "miLoggingUtils.h"

#include <memory>
#include <mutex>
#include <boost/date_time/posix_time/posix_time.hpp>

// thread info
#include <sys/types.h>
#include <sys/syscall.h>

#include <fstream>
#include <iostream>
#include <thread>

namespace milogger {
namespace simple {

namespace /*anonymous*/ {

void no_delete(std::ostream*) {}

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

  { using namespace boost::posix_time;
    ptime now = microsec_clock::universal_time();
    fmt << to_iso_extended_string(now);
  }

#if 0
  { // only on linux
    pid_t tid = syscall(__NR_gettid);
    fmt << " [" << tid << "]";
  }
#endif

#if 0
  fmt << " {" << std::this_thread::get_id() << "}";
#endif

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
