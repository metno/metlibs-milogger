/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
    MA  02110-1301, USA
*/
//#define NLOG4CPP

#ifndef LOGHANDLER_CPP_
#define LOGHANDLER_CPP_

/**
 * @addtogroup common 
 * @{
 * @addtogroup logHandler
 * @{
 */
/** @file
 * Common class used for handling logs in WDB applications.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// PROJECT INCLUDES
//

#ifndef NLOG4CPP
// SYSTEM INCLUDES
#include <log4cpp/Appender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/PatternLayout.hh>
#endif

/** COMMON_LOG is used to define the access point to the logging framework
 */
#ifndef COMMON_LOG
#ifndef NLOG4CPP
#define COMMON_LOG log4cpp::Category
#else
#define COMMON_LOG milogger::LogHandler
#endif
#endif

#ifndef MI_LOG
#define MI_LOG log4cpp::Category
#endif


#ifndef NLOG4CPP
namespace milogger {

/** 
 * The WdbLogHandler class encapsulates the setting up and tearing down of the 
 * loggingframework used in WDB system applications.
 * 
 * The WdbLogHandler class is a singleton. There is no need for multiple 
 * instantiations of the class, as the log4cpp framework allows the logging 
 * of different categories to different files/outputs, if required.
 * 
 * The WdbLogHandler instance is automatically created when getInstance() is 
 * called, but must be explicitly deleted (and the pointer set to 0) when the
 * instance is killed. 
 */
class LogHandler
{
public:

	/**
	 * Default destructor
	 */
	virtual ~LogHandler();
	/**
	 * Set the level of logging to be used in the WdbLogHandler. The level of
	 * logging controls the output level of the WdbLogHandler, anything below
	 * the set level is ignored.
	 * @param[in]	logLevel		The level of logging to be used.
	 */
	void setLogLevel( int logLevel );
	/**
	 * Set the object identifier of the object the WdbLogHandler is operating
	 * for. This information is printed in the log output.
	 * @param[in]	on			The name of the object.
	 */
    void setObjectName( const std::string & on );
	/**
	 * Set the object number of the object the WdbLogHandler is operating
	 * for. This information is printed in the log output.
	 * @param[in]	on			The numerical id of the object.
	 */
    void setObjectNumber( int on );
    // To init the static object
    static LogHandler * initLogHandler(const std::string& propertiesfilename);
    static LogHandler * initLogHandler( int loglevel, const std::string & fileName);
    static LogHandler * getInstance(void)
		{
		  return _loghandler;
		}

protected:
    /**
    	 * Default constructor.
    */
    LogHandler( int loglevel, const std::string & fileName );
    /**
       * Default constructor.
    */
    LogHandler(const std::string& propertiesfilename);
	/**
	 * Default copy constructor. Protected so that it cannot be called directly. 
	 */
	LogHandler(const LogHandler & );
	/**
	 * Default copy constructor. Protected so that it cannot be called directly. 
	 */
	LogHandler & operator= (const LogHandler & );

private:
	/**
	 * Name of object being logged for.
	 */
	std::string objectName_;
	/**
	 * Number id of object being logged.
	 */	
	int objectNumber_;
	/**
	 * Process id of the process being logged.  
	 */
    int pid_;
	/**
	 * The PatternLayout to be used by the logger
	 */
    log4cpp::PatternLayout & layout_;
	/**
	 * Method that updates the layout with new information. Should be called 
	 * each time any of the variables used in the layout, objectName_, 
	 * objectNumber_ or pid_ changes. 
	 */
    void updateLayout();

    static LogHandler * _loghandler;
	
};

} // namespace wdb
#else

#include <iostream>
#include <stdio.h>
#include <string>

namespace milogger {

/*
class LogHandlerStream
{
public:
  void flush() { };
  friend LogHandlerStream& operator<<(LogHandlerStream& stream, char* text);
};

LogHandlerStream& operator<<(LogHandlerStream& stream, char* text)
{
      //std::cout << text;
      return stream;
}
*/

/**
 * NOTE:
 * Could implement different LogHandlerStream-classes for handling different
 * types of streams (ala log4cpp); info, waring, debug etc.
 */
class LogHandlerPrinter
{
public:
  LogHandlerPrinter() {
    ///_infoStream = new LogHandlerStreamInfo;
  }
  ~LogHandlerPrinter() {
    ///delete _infoStream;
  }
  static inline std::ostream& debugStream() { return std::cout; };
  static inline std::ostream& infoStream() { return std::cout; };
  static inline std::ostream& warnStream() { return std::cout; };
  static inline std::ostream& errorStream() { return std::cout; };

private:
  ///LogHandlerStreamInfo* _infoStream;
};

/**
 * A "dummy"-logger, written to avoid log4cpp dependencies, and for this reason ONLY.
 * This is NOT a complete replacement for log4cpp!
 */
class LogHandler
{
public:
  /**
   * Default constructor.
   */
  LogHandler( int loglevel, const std::string& fileName );

  /**
   * Default destructor
   */
  virtual ~LogHandler();
  /**
   * Set the level of logging to be used in the LogHandler. The level of
   * logging controls the output level of the LogHandler, anything below
   * the set level is ignored.
   * @param[in] logLevel    The level of logging to be used.
   */
  void setLogLevel( int logLevel );
  /**
   * Dummy function.
   */
  void setObjectName( const std::string & on );
  /**
   * Dummy function.
   */
  void setObjectNumber( int on );

  static LogHandler* initLogHandler( int loglevel, const std::string & fileName);
  static LogHandler* initLogHandler(const std::string & propertiesfilename);

  static LogHandler * getInstance(void)
  {
    return _loghandler;
  }
  static LogHandlerPrinter & getInstance(const std::string& name)
  {
    return *_loghandlerPrinter;
  }

private:
  enum LOGLEVEL {
    DEBUG, INFO, WARN, ERROR, FATAL
  };

  LOGLEVEL priority;

  /**
   * Default copy constructor. Protected so that it cannot be called directly.
   */
  LogHandler(const LogHandler & );
  /**
   * Default copy constructor. Protected so that it cannot be called directly.
   */
  LogHandler & operator= (const LogHandler & );

  static LogHandler * _loghandler;
  static LogHandlerPrinter * _loghandlerPrinter;

};

} // namespace wdb
#endif


/**
 * @}
 *
 * @}
 */

#endif /* WDBLOGHANDLER_CPP_ */
