
#ifndef METLIBS_MILOGGER_UTILS_H
#define METLIBS_MILOGGER_UTILS_H 1

#include "miLoggingLogger.h"

#include <iosfwd>
#include <map>
#include <string>

namespace milogger {

const char* textFromSeverity(Severity s);
Severity severityFromText(const std::string& text, size_t b_level, size_t e_level);

typedef std::map<std::string, Severity> tag_threshold_m;

void parseTagThresholds(Severity& root_threshold, tag_threshold_m& tag_thresholds, std::istream& config);
Severity findTagThreshold(const Severity& root_threshold, const tag_threshold_m& tag_thresholds, const std::string& tag);

} // namespace milogger

#endif // METLIBS_MILOGGER_UTILS_H
