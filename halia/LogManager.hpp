#pragma once
#include <stdio.h>
#include <stdarg.h>

namespace Halia
{
	namespace LogMsgType
	{
		//! Defines the different types of logging possible
		enum LogMsgType
		{
			ERR = 0,
			WARN = 1,
			MSG = 2,
			DBG = 3,
		};
	};

	//! A class to encapsulate logging data to various output sources.
	class LogManager
	{
	public:
		//! Constructor
		LogManager( );

		//! Destructor
		~LogManager( );

		//! Open a file to have log data written to
		void Open( const char* path = "halia.log" );

		//! Close the open output log file
		void Close( );

		//! Write a formated string to the log output streams
		void Write( LogMsgType::LogMsgType msgtype, char* format, ... );

	private:
		//! The underlying reference to the logging file
		FILE* mLogFile;
	};

	//! Singleton Instance of the Log Manager
	extern LogManager* LMInstance;
};
//! Various defines to make accessing the log manager easier
#define HLOGGER Halia::LMInstance
#define HLOGERR(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::ERR, msg, __VA_ARGS__ )
#define HLOGWARN(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::WARN, msg, __VA_ARGS__ )
#define HLOGMSG(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::MSG, msg, __VA_ARGS__ )
#define HLOGDBG(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::DBG, msg, __VA_ARGS__ )