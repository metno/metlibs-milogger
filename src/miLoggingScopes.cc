
#include "miLoggingScopes.h"

#include <iomanip>

#include <sys/time.h>

namespace /*anonymous*/ {

double seconds(const struct timeval& t)
{
  return t.tv_sec + t.tv_usec / 1e6;
}

double seconds_now()
{
  struct timeval now;
  gettimeofday(&now, 0);
  return seconds(now);
}

} /*anonymous namespace*/

// ########################################################################

namespace milogger {
namespace scopes {

LogEnterExitScope::LogEnterExitScope(LoggerTag& logger, Severity severity, const char* scopename)
  : mLogger(logger)
  , mSeverity(severity)
  , mScopeName(scopename)
  , mTime(-1)
{
  if (RecordPtr r = mLogger.createRecord(mSeverity)) {
    r->stream() << "++ enter " << mScopeName;
    mLogger.submitRecord(r);
  } else {
    mScopeName = 0;
  }
}

LogEnterExitScope::~LogEnterExitScope()
{
  if (!mScopeName)
    return;
  if (RecordPtr r = mLogger.createRecord(mSeverity)) {
    r->stream() << "++ ";
    if (mTime >= 0) {
      r->stream() << "spent " << std::setprecision(6) << std::setw(10) << mTime << "s in ";
    } else {
      r->stream() << "leave ";
    }
    r->stream() << mScopeName;
    mLogger.submitRecord(r);
  }
}

LogEnterExitTime::LogEnterExitTime(LoggerTag& logger, Severity severity, const char* scopename)
  : LogEnterExitScope(logger, severity, scopename)
{
  if (mScopeName)
    mTime = seconds_now();
}

LogEnterExitTime::~LogEnterExitTime()
{
  if (mScopeName)
    mTime = std::max(0.0, seconds_now() - mTime);
}

} // namespace scopes
} // namespace milogger
