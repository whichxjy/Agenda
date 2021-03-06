#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::shared_ptr<Logger> Logger::myLogger = nullptr;

Logger::Logger(const std::string& logFilePath) {
	setFileStream(logFilePath);
}

bool Logger::setFileStream(const std::string& logFilePath) {
	logFileStream = std::make_shared<std::ofstream>();
	logFileStream->open(logFilePath, std::ofstream::app);

	if (!logFileStream->is_open()) {
		logFileStream = nullptr;
		return false;
	}

	return true;
}

const std::string Logger::getCurrentTime() const {
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream tStream;
	tStream << std::put_time(std::localtime(&time), "%Y-%m-%d %X");
	return tStream.str();
}

std::shared_ptr<Logger> Logger::getLogger() {
	if (myLogger == nullptr)
		myLogger = std::shared_ptr<Logger>(new Logger);
	return myLogger;
}

void Logger::destroyLogger() {
	myLogger = nullptr;
}

Logger::~Logger() {
	logFileStream->close();
	myLogger = nullptr;
}

void Logger::userLogIn(State t_state, const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[log in] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << std::endl;
}

void Logger::userRegister(State t_state, const std::string& t_username, const std::string& t_email, 
		const std::string& t_phone) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[register] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << " | "
		<< "email: " << t_email << " | "
		<< "phone: " << t_phone << std::endl; 
}

void Logger::userLogOut(const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[log out] "
		<< "username: " << t_username << std::endl;
}

void Logger::startAgenda(void) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[Start Agenda] " << std::endl;
}

void Logger::quitAgenda(void) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[Quit Agenda] " << std::endl;
}

void Logger::deleteUser(State t_state, const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[delete Agenda account] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << std::endl;
}

void Logger::listAllUsers(const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[list all users] "
		<< "username: " << t_username << std::endl;
}

void Logger::createMeeting(State t_state, const std::string& t_username, const std::string& t_title, 
            const std::string& t_startTime, const std::string& t_endTime, 
            std::vector<std::string> participators) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[create meeting] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ") << std::endl
		<< "\t\t\t\t" << "username: " << t_username << std::endl
		<< "\t\t\t\t" << "title: " << t_title << std::endl
		<< "\t\t\t\t" << "Start Time: " << t_startTime << std::endl
		<< "\t\t\t\t" << "End Time: " << t_endTime << std::endl
		<< "\t\t\t\t" << "Participators: " << *(participators.begin());
        for (auto iter = participators.begin() + 1; iter != participators.end(); ++iter) {
            *logFileStream << "," << *iter;
        }
        *logFileStream << std::endl; 
}

void Logger::addMeetingParticipator(State t_state, const std::string& t_username, const std::string& t_title, 
        const std::string& t_participator) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[add participator] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << " | "
		<< "title: " << t_title << " | "
		<< "participator: " << t_participator << std::endl;
}

void Logger::removeMeetingParticipator(State t_state, const std::string& t_username, const std::string& t_title, 
        const std::string& t_participator) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[remove participator] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << " | "
		<< "title: " << t_title << " | "
		<< "participator: " << t_participator << std::endl;
}

void Logger::requestToQuitMeeting(State t_state, const std::string& t_username, const std::string& t_title) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[quit meeting] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "username: " << t_username << " | "
		<< "title: " << t_title << std::endl;
}

void Logger::listAllMeetings(const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[list all meetings] "
		<< "username: " << t_username << std::endl;
}

void Logger::listAllSponsorMeetings(const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[list all sponsor meetings] "
		<< "username: " << t_username << std::endl;
}

void Logger::listAllParticipateMeetings(const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[list all participator meetings] "
		<< "username: " << t_username << std::endl;
}

void Logger::queryMeetingByTitle(const std::string& t_username, const std::string& t_title) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[query meeting] "
		<< "username: " << t_username << " | "
		<< "title: " << t_title << std::endl;
}

void Logger::queryMeetingByTimeInterval(const std::string& t_username, const std::string& t_startTime, 
		const std::string& t_endTime) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[query meeting] "
		<< "username: " << t_username << " | "
		<< "Start Time: " << t_startTime << " | "
		<< "Endl Time: " << t_endTime << std::endl;
}

void Logger::deleteMeetingByTitle(State t_state, const std::string& t_username, const std::string& t_title) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[delete meeting] "
		<< "username: " << t_username << " | "
		<< "title: " << t_title << std::endl;
}

void Logger::deleteAllMeetings(State t_state, const std::string& t_username) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[delete meeting] "
		<< "username: " << t_username << std::endl;
}

void Logger::readFromFile(State t_state, const std::string& t_userPath, const std::string& t_meetingPath) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[read from file] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "user path: " << t_userPath << " | "
		<< "meeting path: " << t_meetingPath << std::endl;
}

void Logger::writeToFile(State t_state, const std::string& t_userPath, const std::string& t_meetingPath) {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[write to file] "
		<< ((t_state == SUCCESS) ? "[success] " : "[failure] ")
		<< "user path: " << t_userPath << " | "
		<< "meeting path: " << t_meetingPath << std::endl;
}

void Logger::terminateProgram() {
	*logFileStream << "[" << getCurrentTime() << "] "
		<< "[*** TERMINATE PROGRAM ***] " << std::endl; 
}
