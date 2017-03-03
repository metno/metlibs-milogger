
#ifndef MILOGGER_LOGGING_STRINGRECORD_HH
#define MILOGGER_LOGGING_STRINGRECORD_HH 1

#include "miLoggingLogger.h"

#include <sstream>

namespace milogger {
namespace record {

class StringRecord;

typedef std::shared_ptr<StringRecord> StringRecordPtr;

extern StringRecordPtr allocateStringRecord(Severity s);


class StringRecord : public Record, private nocopies {
public:
  StringRecord(Severity severity);
  StringRecord();

  Severity severity() const
    { return severity_; }

  std::ostream& stream() override
    { return stream_; }

  std::string text()
    { return stream_.str(); }

private:
  void severity(Severity s)
    { severity_ = s; }
  friend StringRecordPtr allocateStringRecord(Severity s);

private:
  std::ostringstream stream_;
  Severity severity_;
};

} // namespace record
} // namespace milogger

#endif // MILOGGER_LOGGING_STRINGRECORD_HH
