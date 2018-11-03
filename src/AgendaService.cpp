#include "AgendaService.hpp"
#include <functional>
/**
 * constructor
 */
AgendaService::AgendaService() {

}

/**
 * destructor
 */
AgendaService::~AgendaService() {

}

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
	if (m_storage == nullptr)
		return false;

	auto matchUsers = m_storage->queryUser([userName, password](const User& user)->bool{
		return (user.getName() == userName && user.getPassword() == password)
	});

	if (matchUsers.size() > 0) {
		// Match
		m_storage->createUser(*(matchUsers.begin()));
		m_storage->sync();
		return true;
	}
	else {
		// No user match
		return false;
	}
}

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
	if (m_storage == nullptr)
		return false;

	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.size() > 0) {
		// Username already exists
		return false;
	}
	else {
		// Regist
		m_storage->createUser(User(username, password, email, phone));
		m_storage->sync();
		return true;
	}
}

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
	if (m_storage == nullptr)
		return false;

	int deleteUserNum = m_storage->deleteUser([userName, password](const User& user)->bool{
		return (user.getName() == userName && user.getPassword() == password)
	});

	// If no user match, return false
	if (deleteUserNum == 0)
		return false;

	// User exists
	int deleteMeetingNum = m_storage->deleteMeeting([userName](const Meeting& meeting)->bool{
		// Check if the user is sponsor of this meeting
		if (meeting.getSponsor() == username)
			return true;

		// Check if the user is a participator of this meeting
		for (auto participator : meeting.getParticipator()) {
			if (participator == username)
				return true;
		}

		return false;
	});

	m_storage->sync();
	return true;
}

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
	if (m_storage == nullptr)
		return false;

	return queryUser([](const User& user){return true;});
}

/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endData the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {
	if (m_storage == nullptr)
		return false;

	auto contradictoryMeetings = m_storage->queryMeeting([userName, title, startDate, endDate, participator]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getTitle() == title)
			return true;

		if (meeting.isParticipator(userName) && )

	});





}

/**
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator);

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator);

/**
 * quit from a meeting
 * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title);

/**
 * search a meeting by username and title
 * @param userName as a sponsor OR a participator
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const;
/**
 * search a meeting by username, time interval
 * @param userName as a sponsor OR a participator
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const;

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const;

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const;

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const;

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title);

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName);

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
	m_storage = nullptr;
}