#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "SynchronousQueue.hpp"
#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <atomic>


class Logger {
public:
	using LogRecord = std::string;
	using LogMessage = std::string;

	/***********************************************************
	 *  The level of the events to be tracked (or the level of
	 *  messages to be sent).
	 ***********************************************************/
	enum class Level {
		FATAL,
		ERROR,
		WARN,
		INFO,
		DEBUG
	};
	
	/***********************************************************
	 *  Get the instance of logger. It allows only a single 
	 *  instance to be created. If the parameter "level" is
	 *  different form logger's level, reset it.
	 ***********************************************************/
	static std::shared_ptr<Logger> getLogger(Level level = Level::DEBUG) {
		if (myLogger == nullptr)
			myLogger = std::shared_ptr<Logger>(new Logger(level));
		else if (myLogger->myLevel != level)
			myLogger->setLevel(level);
		return myLogger;
	}

	/***********************************************************
	 *  Destroy the logger. 
	 ***********************************************************/
	static void destroyLogger() {
		myLogger = nullptr;
	}
	
	/***********************************************************
	 *  Destructor. 
	 ***********************************************************/
	~Logger() {
		// Stop the logger and close the file stream.
		stop();
		if (logFileStream) {
			logFileStream->close();
			logFileStream = nullptr;
		}
	}

	/***********************************************************
	 *  Add log record to the log queue.
	 ***********************************************************/
	void write(Level level, const LogMessage& logMsg) {
		if (level > myLevel)
			return;

		// Create log record
		std::stringstream logRecordStream;
		logRecordStream << getCurrentTime()
			<< " [" << levelToString(level) << "] "
			<< logMsg;

		// Add the log record to log queue
		logQueue.put(logRecordStream.str());
	}
	
	/***********************************************************
	 *  Set the threshold for this logger. Logging messages which 
	 *  are less severe than this level will be ignored.
	 ***********************************************************/
	void setLevel(Level level) {
		myLevel = level;
	}

	/***********************************************************
	 *  Get the threshold for this logger.
	 ***********************************************************/
	Level getLevel() const {
		return myLevel;
	}
	
	/***********************************************************
	 *  Get the threshold for this logger as a string.
	 ***********************************************************/
	const std::string getLevelString() const {
		levelToString(myLevel);
	}

	/***********************************************************
	 *  Stop the logger.
	 ***********************************************************/
	void stop() {
		if (!needToStop) {
			needToStop = true;

			// Stop the log queue
			logQueue.stop();

			// Wait until all log records are written to file
			writeThread->join();
			writeThread = nullptr;
		}
	}

	/***********************************************************
	 *  If the logger was stopped, restart it.
	 ***********************************************************/
	void start() {
		if (needToStop) {
			needToStop = false;
			writeThread = std::make_shared<std::thread>([this] { runWriteThread(); } );
			logQueue.start();
		}
	}

	/***********************************************************
	 *  Set the log file path where the log records are send to.
	 ***********************************************************/
	bool setLogFilePath(const std::string& logFilePath) {
		std::lock_guard<std::mutex> lock(myMutex);
		if (myLogFilePath == logFilePath)
			return true;
		myLogFilePath = logFilePath;
		return setFileStream(logFilePath);
	}
	
	/***********************************************************
	 *  Get the log file path where the log records are send to.
	 ***********************************************************/
	const std::string getLogFilePath() const {
		std::lock_guard<std::mutex> lock(myMutex);
		return myLogFilePath;
	}
	
private:
	SynchronousQueue<LogRecord> logQueue;
	std::atomic<Level> myLevel;
	std::string myLogFilePath;
	std::shared_ptr<std::ofstream> logFileStream;
	std::shared_ptr<std::thread> writeThread;
	std::atomic<bool> needToStop;
	mutable std::mutex myMutex;
	static std::shared_ptr<Logger> myLogger;

	/***********************************************************
	 *  A private constructor. The parameter "level" specifies 
	 *  the lowest severity that will be dispatched to the log
	 *  file. The parameter "logFilePath" specifies the log file
	 *  path where the log records are send to.
	 ***********************************************************/
	Logger(Level level, const std::string& logFilePath = "acht_log.log") 
	: myLevel(level), logQueue(100), myLogFilePath(logFilePath), needToStop(false) {
		setFileStream(logFilePath);
		writeThread = std::make_shared<std::thread>([this] { runWriteThread(); } );
	}

	/***********************************************************
	 *  Set file stream with the log file path.
	 ***********************************************************/
	bool setFileStream(const std::string& logFilePath) {
		logFileStream = std::make_shared<std::ofstream>();
		logFileStream->open(logFilePath, std::ofstream::app);

		if (!logFileStream->is_open()) {
			std::cerr << "Failed to open log file: " << logFilePath << std::endl; 
			logFileStream = nullptr;
			return false;
		}

		return true;
	}

	/***********************************************************
	 *  Run the write thread until logger is stopped.
	 *  Write every log record it takes from the log queue, 
	 *  waiting if the task queue is empty.
	 ***********************************************************/
	void runWriteThread() {
		while (!needToStop) {
			std::string log;
			if (logQueue.take(log) && logFileStream) {
				*logFileStream << log << std::endl;
			}
		}
	}

	/***********************************************************
	 *  Convert a level to a string.
	 ***********************************************************/
	const std::string levelToString(Level level) const {
		switch(level) {
			case Level::FATAL:
				return "FATAL";
			case Level::ERROR:
				return "ERROR";
			case Level::WARN:
				return "WARN";
			case Level::INFO:
				return "INFO";
			case Level::DEBUG:
				return "DEBUG";
		}		
	}

	/***********************************************************
	 *  Get the current time as a string.
	 ***********************************************************/
	const std::string getCurrentTime() const {
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::stringstream tStream;
		tStream << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
		return tStream.str();
	}
};

std::shared_ptr<Logger> Logger::myLogger = nullptr;

/***********************************************************
*  Log messages to the log file.
***********************************************************/
#define LOG_FATAL(LOG_MESSAGE) Logger::getLogger()->write(Logger::Level::FATAL, LOG_MESSAGE);
#define LOG_ERROR(LOG_MESSAGE) Logger::getLogger()->write(Logger::Level::ERROR, LOG_MESSAGE);
#define LOG_WARN(LOG_MESSAGE) Logger::getLogger()->write(Logger::Level::WARN, LOG_MESSAGE);
#define LOG_INFO(LOG_MESSAGE) Logger::getLogger()->write(Logger::Level::INFO, LOG_MESSAGE);
#define LOG_DEBUG(LOG_MESSAGE) Logger::getLogger()->write(Logger::Level::DEBUG, LOG_MESSAGE);


#endif
