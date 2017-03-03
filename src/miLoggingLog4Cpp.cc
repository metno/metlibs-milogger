
#include "miLoggingLog4Cpp.h"

#include "miLoggingStringRecord.h"

#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>

#include <memory>

#include <sys/stat.h>
#include <unistd.h>

#define MILOGGER_FLUSH 1

namespace milogger {
namespace l4c {

namespace /* anonymous*/ {

#ifdef MILOGGER_FLUSH
class FlushingOstreamAppender : public log4cpp::OstreamAppender {
public:
  FlushingOstreamAppender(const std::string& name, std::ostream* stream)
    : OstreamAppender(name, stream) { }

protected:
  virtual void _append(const log4cpp::LoggingEvent& event)
    { log4cpp::OstreamAppender::_append(event); if (_stream) (*_stream) << std::flush; }

};

log4cpp::Appender* makeConsoleAppender()
{
  log4cpp::Appender *a = new FlushingOstreamAppender("console", &std::clog);
  log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
  layout->setConversionPattern("%d %p %c: %m%n");
  a->setLayout(layout);
  return a;
}

void fixConsoleLoggers()
{
  typedef std::vector<log4cpp::Category*> categories_t;
  categories_t* allCat = log4cpp::Category::getCurrentCategories();
  if (not allCat)
    return;

  for (categories_t::iterator itC=allCat->begin(); itC!=allCat->end(); ++itC) {
    // (*itC)->setAdditivity(false);
    log4cpp::AppenderSet allApp = (*itC)->getAllAppenders();
    for (log4cpp::AppenderSet::iterator itA=allApp.begin(); itA!=allApp.end(); ++itA) {
      log4cpp::OstreamAppender* oApp = dynamic_cast<log4cpp::OstreamAppender*>(*itA);
      if (!oApp)
        continue;
      (*itC)->removeAppender(oApp);
      (*itC)->addAppender(makeConsoleAppender());
    }
  }
}
#else // !MILOGGER_FLUSH
log4cpp::Appender* makeConsoleAppender()
{
  return new log4cpp::OstreamAppender("console", &std::clog);
}
#endif // !MILOGGER_FLUSH

const char DEFAULT_PATTERN[] = "%d %p %c: %m%n";

void addRootAppender(log4cpp::Appender* a)
{
  std::unique_ptr<log4cpp::PatternLayout> layout(new log4cpp::PatternLayout());
  layout->setConversionPattern(DEFAULT_PATTERN);
  a->setLayout(layout.release());

  log4cpp::Category& root = log4cpp::Category::getRoot();
  root.addAppender(a);
}

// ########################################################################

log4cpp::Priority::PriorityLevel severity2L4C(Severity s)
{
  switch (s) {
  case ::milogger::VERBOSE: return log4cpp::Priority::DEBUG;
  case ::milogger::DEBUG:   return log4cpp::Priority::DEBUG;
  case ::milogger::INFO :   return log4cpp::Priority::INFO;
  case ::milogger::WARN :   return log4cpp::Priority::WARN;
  case ::milogger::ERROR:   return log4cpp::Priority::ERROR;
  case ::milogger::FATAL:
  case ::milogger::INVALID: break;
  }
  return log4cpp::Priority::FATAL;
}

#if 0
Severity severity4L4C(log4cpp::Priority::Value pv)
{
  if (pv >= log4cpp::Priority::DEBUG)
    return ::milogger::DEBUG;
  else if (pv >= log4cpp::Priority::INFO)
    return ::milogger::INFO;
  else if (pv >= log4cpp::Priority::WARN)
    return ::milogger::WARN;
  else if (pv >= log4cpp::Priority::ERROR)
    return ::milogger::ERROR;
  else
    return ::milogger::FATAL;
}
#endif

log4cpp::Category& getCategoryL4C(const std::string& name)
{
  if (name.empty())
    return log4cpp::Category::getRoot();
  else
    return log4cpp::Category::getInstance(name);
}

} // anonymous namespace

// ########################################################################

class L4CLogger : public ::milogger::Logger {
public:
  L4CLogger(const std::string& tag);
  ~L4CLogger() { }

  bool isEnabledFor(Severity severity) override;
  RecordPtr createRecord(Severity severity) override;
  void submitRecord(RecordPtr record) override;

private:
  log4cpp::Category& mCategory;
};

L4CLogger::L4CLogger(const std::string& tag)
  : mCategory(getCategoryL4C(tag))
{ }

bool L4CLogger::isEnabledFor(Severity severity)
{
  return mCategory.isPriorityEnabled(severity2L4C(severity));
}

RecordPtr L4CLogger::createRecord(Severity severity)
{
  if (mCategory.isPriorityEnabled(severity2L4C(severity))) {
    return record::allocateStringRecord(severity);
  } else {
    return RecordPtr();
  }
}

void L4CLogger::submitRecord(RecordPtr record)
{
  record::StringRecordPtr sr = std::static_pointer_cast<record::StringRecord>(record);
  mCategory.log(severity2L4C(sr->severity()), sr->text());
}

// ########################################################################

void L4CSystem::configure(const std::string& l4c_p)
{
  struct stat sb;
  if (stat(l4c_p.c_str(), &sb) == 0) {
    log4cpp::PropertyConfigurator::configure(l4c_p);
#ifdef MILOGGER_FLUSH
    fixConsoleLoggers();
#endif // MILOGGER_FLUSH
  } else {
    log4cpp::Category& root = log4cpp::Category::getRoot();
    root.setPriority(log4cpp::Priority::WARN);

    addRootAppender(makeConsoleAppender());
  }
}

void L4CSystem::addSink(const std::string& sinkoptions)
{
  static int counter = 0;
  std::ostringstream name;
  name << "sink_" << (++counter);

  addRootAppender(new log4cpp::FileAppender(name.str(), sinkoptions, false));
}

void L4CSystem::shutdown()
{
  log4cpp::Category::shutdown();
}

LoggerPtr L4CSystem::getLogger(const std::string& tag)
{
  return std::make_shared<L4CLogger>(tag);
}

} // namespace l4c
} // namespace milogger
