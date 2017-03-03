
#include "miLoggingNone.h"

#include <memory>
#include <mutex>

namespace milogger {
namespace none {

bool NoLogger::isEnabledFor(Severity)
{
  return false;
}

RecordPtr NoLogger::createRecord(Severity)
{
  return RecordPtr();
}

void NoLogger::submitRecord(RecordPtr)
{
}

// ########################################################################

std::mutex NoSystem::sMutex;
std::shared_ptr<NoLogger> NoSystem::sLogger;

LoggerPtr NoSystem::getLogger(const std::string&)
{
  std::lock_guard<std::mutex> guard(sMutex);
  if (!sLogger)
    sLogger = std::make_shared<NoLogger>();
  return sLogger;
}

} // namespace none
} // namespace milogger
