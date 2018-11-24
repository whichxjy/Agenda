// #ifndef LOGGER_HPP
// #define LOGGER_HPP

// #include <string>
// #include <memory>

// class Logger {

// private:
// 	Logger(const std::string& logFilePath = "agenda_log.log");
// 	bool setFileStream(const std::string& logFilePath);
// 	const std::string getCurrentTime() const;

// 	std::shared_ptr<std::ofstream> logFileStream;
// 	static std::shared_ptr<Logger> myLogger;

// public:
// 	static std::shared_ptr<Logger> getLogger();
// 	static void destroyLogger();
// 	~Logger();

//     void userLogIn(void);

//     void userRegister(void);

//     /**
//      * user logout
//      */
//     void userLogOut(void);

//     /**
//      * quit the Agenda
//      */
//     void quitAgenda(void);

//     /**
//      * delete a user from storage
//      */
//     void deleteUser(void);

//     /**
//      * list all users from storage
//      */
//     void listAllUsers(void);

//     /**
//      * user create a meeting with someone else
//      */
//     void createMeeting(void);

//     void addMeetingParticipator(void);

//     void removeMeetingParticipator(void);

//     void requestToQuitMeeting(void);

//     /**
//      * list all meetings from storage
//      */
//     void listAllMeetings(void);

//     /**
//      * list all meetings that this user sponsored
//      */
//     void listAllSponsorMeetings(void);

//     /**
//      * list all meetings that this user take part in
//      */
//     void listAllParticipateMeetings(void);

//     /**
//      * search meetings by title from storage
//      */
//     void queryMeetingByTitle(void);

//     /**
//      * search meetings by timeinterval from storage
//      */
//     void queryMeetingByTimeInterval(void);

//     /**
//      * delete meetings by title from storage
//      */
//     void deleteMeetingByTitle(void);

//     /**
//      * delete all meetings that this user sponsored
//      */
//     void deleteAllMeetings(void);

//     /**
//      * show the meetings in the screen
//      */
//     void printMeetings(std::list<Meeting> t_meetings);

// };

// #endif

