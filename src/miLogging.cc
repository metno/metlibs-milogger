
#define MILOGGER_CATEGORY "miLogger.miLogging" // get rid of warning
#include "miLogging.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iomanip>

namespace /* anonymous*/ {

} // namespace anonymous

// ########################################################################

namespace milogger {
namespace detail {

static const std::string LEAVE("leave");

LogScope::LogScope(CategoryRef cat, const char* fun)
    : mCategory(cat)
    , mFunction(fun)
    , mExitMessage(LEAVE)
{
  MILOGGER_DETAIL_LOG_CLM(mCategory, DEBUG, "++ enter " << mFunction);
}

LogScope::~LogScope()
{
  MILOGGER_DETAIL_LOG_CLM(mCategory, DEBUG, "++ " << mExitMessage << ' ' << mFunction);
}

// ========================================================================

LogTime::LogTime(CategoryRef cat, const char* fun)
  : LogScope(cat, fun)
{
    gettimeofday(&pre, 0);
}

LogTime::~LogTime()
{
  struct timeval post;
  gettimeofday(&post, 0);
  const double s = (((double)post.tv_sec*1000000.0 + (double)post.tv_usec)-((double)pre.tv_sec*1000000.0 + (double)pre.tv_usec))/1000000.0;
  std::ostringstream exitmsg;
  exitmsg << "spent " << std::setprecision(6) << std::setw(10) << s << "s in";
  mExitMessage = exitmsg.str();
}

// ========================================================================

std::string getCategoryName(const std::string& c, const std::string& f)
{
  if (c.empty())
    return "";
  else if (f.empty())
    return c;
  else
    return (c + "." + f);
}

} // namespace detail
} // namespace milogger

#if !defined(NO_LOG4CPP) || defined(HAVE_LIBLOG4CPP)
#include "miLogging_log4cpp.cc"
#else
#include "miLogging_iostream.cc"
#endif
