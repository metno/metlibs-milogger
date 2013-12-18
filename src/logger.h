
#ifndef _metno_logger_h
#define _metno_logger_h

#include <iostream>

#define LOG4CXX_DEBUG(logger, message) {}

#define LOG4CXX_INFO(logger, message)  { std::cout << "INFO:" << message << std::endl;  }
#define LOG4CXX_WARN(logger, message)  { std::cerr << "WARN:" << message << std::endl;  }
#define LOG4CXX_ERROR(logger, message) { std::cerr << "ERROR:" << message << std::endl; }
#define LOG4CXX_FATAL(logger, message) { std::cerr << "FATAL:" << message << std::endl; }


#endif
