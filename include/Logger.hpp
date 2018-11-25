#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <memory>
#include <vector>

enum State {
    SUCCESS,
    FAILURE
};

class Logger {

private:
	Logger(const std::string& logFilePath = "agenda_log.log");
	bool setFileStream(const std::string& logFilePath);
	const std::string getCurrentTime() const;

	std::shared_ptr<std::ofstream> logFileStream;
	static std::shared_ptr<Logger> myLogger;

public:
	static std::shared_ptr<Logger> getLogger();

	static void destroyLogger();
	~Logger();

    void userLogIn(State t_state, const std::string& t_username);

    void userRegister(State t_state, const std::string& t_username, const std::string& t_email, 
        const std::string& t_phone);

    void userLogOut(const std::string& t_username);

    void startAgenda(void);

    void quitAgenda(void);

    void deleteUser(State t_state, const std::string& t_username);

    void listAllUsers(const std::string& t_username);

    void createMeeting(State t_state, const std::string& t_username, const std::string& t_title, 
            const std::string& t_startTime, const std::string& t_endTime, 
            std::vector<std::string> participators);

    void addMeetingParticipator(State t_state, const std::string& t_username, const std::string& t_title, 
            const std::string& t_participator);

    void removeMeetingParticipator(State t_state, const std::string& t_username, const std::string& t_title, 
            const std::string& t_participator);

    void requestToQuitMeeting(State t_state, const std::string& t_username, const std::string& t_title);

    void listAllMeetings(const std::string& t_username);

    void listAllSponsorMeetings(const std::string& t_username);

    void listAllParticipateMeetings(const std::string& t_username);

    void queryMeetingByTitle(const std::string& t_username, const std::string& t_title);

    void queryMeetingByTimeInterval(const std::string& t_username, const std::string& t_startTime, 
        const std::string& t_endTime);

    void deleteMeetingByTitle(State t_state, const std::string& t_username, const std::string& t_title);

    void deleteAllMeetings(State t_state, const std::string& t_username);

    void readFromFile(State t_state, const std::string& t_userPath, const std::string& t_meetingPath);

    void writeToFile(State t_state, const std::string& t_userPath, const std::string& t_meetingPath);

    void terminateProgram();

};

#endif

