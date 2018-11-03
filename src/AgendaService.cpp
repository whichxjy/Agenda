#include "AgendaService.hpp"
#include <functional>
/**
 * constructor
 */
AgendaService::AgendaService() {
	startAgenda();
}

/**
 * destructor
 */
AgendaService::~AgendaService() {
	quitAgenda();
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
		return (user.getName() == userName && user.getPassword() == password);
	});

	if (matchUsers.size() > 0) {
		// Match
		m_storage->createUser(*(matchUsers.begin()));
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
		m_storage->createUser(User(userName, password, email, phone));
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
		return (user.getName() == userName && user.getPassword() == password);
	});

	// If no user match, return false
	if (deleteUserNum == 0)
		return false;

	// Delete meetings  that the user is in
	m_storage->deleteMeeting([userName](const Meeting& meeting)->bool{
		// Check if the user is sponsor of this meeting
		if (meeting.getSponsor() == userName)
			return true;

		// Check if the user is a participator of this meeting
		if (meeting.isParticipator(userName))
			return true;

		return false;
	});

	return true;
}

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
	// if (m_storage == nullptr)
	// 	return std::list<User>();
	return m_storage->queryUser([](const User& user){return true;});
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

	// Check if sponsor is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.size() == 0) {
		// Sponsor don't exists in the user list
		return false;
	}

	// Check if participators are in the user list
	for (auto participator_name : participator) {
		auto sameNameUsers = m_storage->queryUser([participator_name](const User& user)->bool {
			return user.getName() == participator_name;
		});
		if (sameNameUsers.size() == 0) {
			// Participator don't exists in the user list
			return false;
		}
	}

	// Find the meetings that the user is sponsor or participator
	auto userExistMeetings = m_storage->queryMeeting([userName](const Meeting& meeting)->bool{
		// Check if the user is sponsor of this meeting
		if (meeting.getSponsor() == userName)
			return true;

		// Check if the user is a participator of this meeting
		if (meeting.isParticipator(userName))
			return true;

		return false;
	});

	// Find contradictory meetings
	auto contradictoryMeetings = m_storage->queryMeeting([userName, title, startDate, endDate, participator, userExistMeetings]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getTitle() == title)
			return true;

		// Invaild date
		if (startDate >= endDate)
			return true;

		// Cheak overlap
		if (userExistMeetings.size() > 0) {
			for (auto userExistMeeting : userExistMeetings) {
				// Overlap
				if (Date(endDate) > userExistMeeting.getStartDate() 
					&& Date(startDate) < userExistMeeting.getEndDate()) {
					return true;
				}	
			} 
		}

		return false;
	});

	// If contradictory meeting exists, then fail to create meeting
	if (contradictoryMeetings.size() > 0) {
		return false;
	}
	else {
		// create meeting
		m_storage->createMeeting(Meeting(userName, participator, Date(startDate), Date(endDate), title));
		return true;
	}

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
                            const std::string &participator) {
	if (m_storage == nullptr)
		return false;

	// Check if user is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.size() == 0) {
		// User don't exists in the user list
		return false;
	}

	// ??????????????????????????
	// Check if participator is in the user list
	sameNameUsers = m_storage->queryUser([participator](const User& user)->bool {
		return user.getName() == participator;
	});
	if (sameNameUsers.size() == 0) {
		// Participator don't exists in the user list
		return false;
	}

	// Check if the title exists, sponsor matchs and paticipator exists
	// If it exists, update it
	int updateMeetingNum = m_storage->updateMeeting([userName, title, participator]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getSponsor() == userName 
			&& meeting.getTitle() == title
			&& !meeting.isParticipator(participator))
			return true;
		else
			return false;
	},
		[participator](Meeting & meeting)->void{
			meeting.addParticipator(participator);
	});

	if (updateMeetingNum == 0) {
		// No meeting match
		return false;
	}
	else {
		return true;
	}	
}

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator) {
	if (m_storage == nullptr)
		return false;

	// Check if user is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.size() == 0) {
		// User don't exists in the user list
		return false;
	}

	// ??????????????????????????
	// Check if participator is in the user list
	sameNameUsers = m_storage->queryUser([participator](const User& user)->bool {
		return user.getName() == participator;
	});
	if (sameNameUsers.size() == 0) {
		// Participator don't exists in the user list
		return false;
	}

	// Check if the title exists, sponsor matchs and paticipator exists
	// If it exists, update it
	int updateMeetingNum = m_storage->updateMeeting([userName, title, participator]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getSponsor() == userName 
			&& meeting.getTitle() == title
			&& !meeting.isParticipator(participator))
			return true;
		else
			return false;
	},
		[participator](Meeting & meeting)->void{
			meeting.removeParticipator(participator);
	});

	if (updateMeetingNum == 0) {
		// No meeting match
		return false;
	}
	else {
		return true;
	}	
}

/**
 * quit from a meeting
 * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
	if (m_storage == nullptr)
		return false;

	int updateMeetingNum = m_storage->updateMeeting([userName, title](const Meeting& meeting)->bool {
		if (meeting.getTitle() == title && meeting.isParticipator(userName))
			return true;
		else
			return false;
	},
		[userName](Meeting & meeting)->void{
			meeting.removeParticipator(userName);
	});

	if (updateMeetingNum == 0) {
		// No meeting match
		return false;
	}
	else {
		return true;
	}	
}

/**
 * search a meeting by username and title
 * @param userName as a sponsor OR a participator
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const {
	return m_storage->queryMeeting([userName, title](const Meeting& meeting)->bool {
		return (meeting.getTitle() == title 
			&& (meeting.getSponsor() == userName || meeting.isParticipator(userName)));
	});
}

/**
 * search a meeting by username, time interval
 * @param userName as a sponsor OR a participator
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
	if (startDate > endDate)
		return std::list<Meeting>();

	return m_storage->queryMeeting([userName, startDate, endDate]
		(const Meeting& meeting)->bool {
			return ((meeting.getSponsor() == userName || meeting.isParticipator(userName))
				&& Date(endDate) >= meeting.getStartDate() && Date(startDate) <= meeting.getEndDate());
	});
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return (meeting.getSponsor() == userName || meeting.isParticipator(userName));
	});
}

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return meeting.getSponsor() == userName;
	});
}

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return meeting.isParticipator(userName);
	});
}

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
	int deleteMeetingNum = m_storage->deleteMeeting([userName, title]
		(const Meeting & meeting)->bool {
			return (meeting.getSponsor() == userName && meeting.getTitle() == title);
	});
	if (deleteMeetingNum == 0) {
		return false;
	}
	else {
		return true;
	}
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
	int deleteMeetingNum = m_storage->deleteMeeting([userName]
		(const Meeting & meeting)->bool {
			return meeting.getSponsor() == userName;
	});
	if (deleteMeetingNum == 0) {
		return false;
	}
	else {
		return true;
	}
}

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
	m_storage->sync();
	m_storage = nullptr;
}