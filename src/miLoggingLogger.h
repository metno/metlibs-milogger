
#ifndef MILOGGER_LOGGING_LOGGER_HH
#define MILOGGER_LOGGING_LOGGER_HH 1

#include <memory>
#include <ostream>
#include <string>

namespace milogger {

class nocopies {
protected:
  constexpr nocopies() = default;
  ~nocopies() = default;

  nocopies(const nocopies&) = delete;
  nocopies& operator=(const nocopies&) = delete;
};

enum Severity { INVALID, VERBOSE, DEBUG, INFO, WARN, ERROR, FATAL };

// ------------------------------------------------------------------------

class Record {
public:
  Record();
  virtual ~Record() { }
  virtual std::ostream& stream() = 0;
};

typedef std::shared_ptr<Record> RecordPtr;

// ------------------------------------------------------------------------

class Logger : nocopies {
protected:
  Logger();

public:
  virtual ~Logger() { }

  virtual bool isEnabledFor(Severity severity)
    { return static_cast<bool>(createRecord(severity)); }
  virtual RecordPtr createRecord(Severity severity) = 0;
  virtual void submitRecord(RecordPtr record) = 0;
};

typedef std::shared_ptr<Logger> LoggerPtr;

LoggerPtr getLoggerByTag(const char* tag, const char* tag_extra);

// ------------------------------------------------------------------------

class LoggerTag {
public:
  LoggerTag(const char* tag, const char* tag_extra=0)
    : mTag(tag), mTagExtra(tag_extra) { }

  ~LoggerTag()
    { reset(); }

  bool isEnabledFor(Severity severity)
    { return logger()->isEnabledFor(severity); }

  RecordPtr createRecord(Severity severity)
    { return logger()->createRecord(severity); }

  void submitRecord(RecordPtr record)
    { logger()->submitRecord(record); }

  LoggerPtr logger();
  void reset();

private:
  const char *mTag, *mTagExtra;
  LoggerPtr mLogger;
};

// ------------------------------------------------------------------------

class LoggingConfig : nocopies {
public:
  LoggingConfig(const std::string& configuration = std::string());
  ~LoggingConfig();

  void addConfig(const std::string& configuration = std::string());
  void addSink(const std::string& sinkoptions);
};

} // namespace milogger

#endif // MILOGGER_LOGGING_LOGGER_HH
