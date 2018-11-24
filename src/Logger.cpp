// #include "Logger.hpp"
// #include <fstream>

// #include <chrono>
// #include <ctime>
// #include <iomanip>
// #include <sstream>

// std::shared_ptr<Logger> Logger::myLogger = nullptr;

// Logger::Logger(const std::string& logFilePath) {
// 	setFileStream(logFilePath);
// }

// bool Logger::setFileStream(const std::string& logFilePath) {
// 	logFileStream = std::make_shared<std::ofstream>();
// 	logFileStream->open(logFilePath, std::ofstream::app);

// 	if (!logFileStream->is_open()) {
// 		// std::cerr << "Failed to open log file: " << logFilePath << std::endl; 
// 		logFileStream = nullptr;
// 		return false;
// 	}

// 	return true;
// }

// const std::string Logger::getCurrentTime() const {
// 	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
// 	std::stringstream tStream;
// 	tStream << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
// 	return tStream.str();
// }

// std::shared_ptr<Logger> Logger::getLogger() {
// 	if (myLogger == nullptr)
// 		myLogger = std::shared_ptr<Logger>(new Logger());
// 	return myLogger;
// }

// void Logger::destroyLogger() {
// 	myLogger = nullptr;
// }

// Logger::~Logger() {
// 	logFileStream->close();
// 	myLogger = nullptr;
// }

