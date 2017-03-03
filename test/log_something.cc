
#include <thread>
#include <vector>

#define MILOGGER_CATEGORY "something"
#include "miLogging.h"

#include <gtest/gtest.h>

namespace {

void fun_function(int i=0)
{
  METLIBS_LOG_TIME(LOGVAL(i));
  METLIBS_LOG_DEBUG("this is a bug");
  METLIBS_LOG_INFO("this is very informational");
  METLIBS_LOG_ERROR("this is actually ok");
}

void info_function()
{
  const ::milogger::scopes::LogEnterExitTime MILOGGER_scope(MILOGGER_logger, ::milogger::INFO, __PRETTY_FUNCTION__);
  METLIBS_LOG_ERROR("this is my message");
}

void call_info()
{
  for (int i=0; i<100; ++i)
    info_function();
}

} // namespace

TEST(MiLogging, ManyThreads)
{
  milogger::LoggingConfig log4cpp("-.!!=-:");

  fun_function(4);
  info_function();

  const int NTHREADS = 100;
  std::vector<std::thread*> threads;;
  for (int i=0; i<NTHREADS; ++i) {
    threads.push_back(new std::thread(call_info));
  }
  for (int i=0; i<NTHREADS; ++i) {
    threads[i]->join();
    delete threads[i];
  }
}

#if 0
TEST(MiLogging, NoCopies)
{
  milogger::LoggingConfig log4cpp("-.!!=-:");
  milogger::LoggingConfig lc_copy = lc; // copy constructor deleted
}
#endif
