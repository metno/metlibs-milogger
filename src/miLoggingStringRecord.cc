
#include "miLoggingStringRecord.h"

#ifndef MILOGGER_DO_NOT_USE_POOL_ALLOCATOR
#include <boost/pool/pool_alloc.hpp>
#endif

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


#ifndef MILOGGER_DO_NOT_USE_POOL_ALLOCATOR
typedef boost::fast_pool_allocator<StringRecord> StringRecord_pool_t;
static StringRecord_pool_t poolStringRecord;
#endif

StringRecordPtr allocateStringRecord(Severity s)
{
  // TODO this is still allocating memory for the string "mText",
  // while it would only need to "clear" on an existing StringRecord
#ifdef MILOGGER_DO_NOT_USE_POOL_ALLOCATOR
  return std::make_shared<StringRecord>(s);
#elif 0
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
