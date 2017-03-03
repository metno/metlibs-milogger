
#include "miLoggingStringRecord.h"

#include <boost/pool/pool_alloc.hpp>

namespace milogger {
namespace record {

StringRecord::StringRecord(Severity severity)
  : severity_(severity)
{
}

StringRecord::StringRecord()
  : severity_(INVALID)
{
}


typedef boost::fast_pool_allocator<StringRecord> StringRecord_pool_t;

StringRecord_pool_t poolStringRecord;

StringRecordPtr allocateStringRecord(Severity s)
{
  // TODO this is still allocating memory for the string "mText",
  // while it would only need to "clear" on an existing StringRecord
#if 0
  return std::allocate_shared<StringRecord, StringRecord_pool_t>(poolStringRecord, s);
#else
  // boost 1.58 fast_pool_allocator seems to have problems with
  // noncopyable types with constructor arguments
  StringRecordPtr sr = std::allocate_shared<StringRecord, StringRecord_pool_t>(poolStringRecord);
  sr->severity(s);
  return sr;
#endif
}

} // namespace record
} // namespace milogger
