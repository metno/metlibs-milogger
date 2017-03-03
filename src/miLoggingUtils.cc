
#include "miLoggingUtils.h"

#include <iostream>
#include <string>

namespace /*anonymous*/ {

const char l4c_cat[] = "log4cpp.category.";
const size_t n_l4c_cat = sizeof(l4c_cat)-1;

const char l4c_root_cat[] = "log4cpp.rootCategory=";
const size_t n_l4c_root_cat = sizeof(l4c_root_cat)-1;

size_t find_comma(const std::string& text, size_t begin)
{
  size_t comma = text.find(",", begin);
  if (comma != std::string::npos)
    return comma;
  else
    return text.size();
}

} /*anonymous namespace*/

namespace milogger {

Severity severityFromText(const std::string& text, size_t b_level, size_t e_level)
{
  size_t n_level = e_level - b_level;
  if (text.compare(b_level, n_level, "FATAL") == 0)
    return milogger::FATAL;
  else if (text.compare(b_level, n_level, "ERROR") == 0)
    return milogger::ERROR;
  else if (text.compare(b_level, n_level, "WARN") == 0)
    return milogger::WARN;
  else if (text.compare(b_level, n_level, "INFO") == 0)
    return milogger::INFO;
  else if (text.compare(b_level, n_level, "DEBUG") == 0)
    return milogger::DEBUG;
  else if (text.compare(b_level, n_level, "VERBOSE") == 0)
    return milogger::VERBOSE;
  else
    return milogger::INVALID;
}

const char* textFromSeverity(Severity s)
{
  switch (s) {
  case VERBOSE: return "VERBOSE";
  case DEBUG:   return "DEBUG  ";
  case INFO :   return "INFO   ";
  case WARN :   return "WARN   ";
  case ERROR:   return "ERROR  ";
  case FATAL:   return "FATAL  ";
  case INVALID: break;
  }
  return "INVALID";
}

void parseTagThresholds(Severity& root_threshold, tag_threshold_m& tag_thresholds, std::istream& config)
{
  tag_thresholds.clear();

  if (!config.good())
    return;

  std::string line;
  while (std::getline(config, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    if (line.compare(0, n_l4c_cat, l4c_cat) == 0) {
      size_t eq = line.find("=", n_l4c_cat);
      if (eq != std::string::npos) {
        size_t b_level = eq+1;
        size_t e_level = find_comma(line, b_level);
        Severity t = severityFromText(line, b_level, e_level);
        if (t != milogger::INVALID) {
          const std::string cat = line.substr(n_l4c_cat, eq-n_l4c_cat);
          tag_thresholds.insert(std::make_pair(cat, t));
        }
      }
    } else if (line.compare(0, n_l4c_root_cat, l4c_root_cat) == 0) {
      size_t b_level = n_l4c_root_cat;
      size_t e_level = find_comma(line, b_level);
      Severity t = severityFromText(line, b_level, e_level);
      if (t != milogger::INVALID) {
        root_threshold = t;
      }
    }
  }
}

Severity findTagThreshold(const Severity& root_threshold, const tag_threshold_m& tag_thresholds, const std::string& tag)
{
  tag_threshold_m::const_iterator tt = tag_thresholds.find(tag);
  if (tt != tag_thresholds.end())
    return tt->second;

  size_t end = std::string::npos;
  while (true) {
    size_t dot = tag.find_last_of(".", end);
    if (dot == end || dot == std::string::npos)
      return root_threshold;

    const std::string head = tag.substr(0, dot);
    tag_threshold_m::const_iterator tt = tag_thresholds.find(head);
    if (tt != tag_thresholds.end())
      return tt->second;
    end = dot;
  }
}

} // namespace milogger
