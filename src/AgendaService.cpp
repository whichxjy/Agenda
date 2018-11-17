#include "AgendaService.hpp"
#include <functional>

AgendaService::AgendaService() {
	startAgenda();
	m_storage = Storage::getInstance();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
	if (m_storage == nullptr)
		return false;

	auto matchUsers = m_storage->queryUser([userName, password](const User& user)->bool{
		return (user.getName() == userName && user.getPassword() == password);
	});

	return !matchUsers.empty();
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
	if (m_storage == nullptr)
		return false;

	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (!sameNameUsers.empty()) {
		// Username already exists
		return false;
	}
	else {
		// Regist
		m_storage->createUser(User(userName, password, email, phone));
		return true;
	}
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
	if (m_storage == nullptr)
		return false;

	int deleteUserNum = m_storage->deleteUser([userName, password](const User& user)->bool{
		return (user.getName() == userName && user.getPassword() == password);
	});

	// If no user match, return false
	if (deleteUserNum == 0)
		return false;

	// // Delete meetings  that the user is in
	// m_storage->deleteMeeting([userName](const Meeting& meeting)->bool{
	// 	// Check if the user is sponsor of this meeting
	// 	if (meeting.getSponsor() == userName)
	// 		return true;

	// 	// Check if the user is a participator of this meeting
	// 	if (meeting.isParticipator(userName))
	// 		return true;

	// 	return false;
	// });

	deleteAllMeetings(userName);

	m_storage->updateMeeting([userName](const Meeting& meeting)->bool {
		return meeting.isParticipator(userName);
	},
		[userName](Meeting &meeting)->void {
			meeting.removeParticipator(userName);
	});

	m_storage->deleteMeeting([](const Meeting &meeting)->bool {
		return meeting.getParticipator().empty();
	});

	return true;
}

std::list<User> AgendaService::listAllUsers(void) const {
	return m_storage->queryUser([](const User& user){return true;});
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {
	if (m_storage == nullptr)
		return false;

	// Check if sponsor is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.empty()) {
		// Sponsor don't exists in the user list
		return false;
	}

	// Check if participators are in the user list
	for (auto participator_name : participator) {
		auto sameNameUsers = m_storage->queryUser([participator_name](const User& user)->bool {
			return user.getName() == participator_name;
		});
		if (sameNameUsers.empty()) {
			// Participator don't exists in the user list
			return false;
		}
	}

	if (Date(startDate) == Date() || Date(endDate) == Date())
		return false;

	// Check if the date is valid
	if (!Date::isValid(Date(startDate)) || !Date::isValid(Date(endDate)))
		return false;


	// Invaild date
	if (Date(startDate) >= Date(endDate))
		return true;

	// Find contradictory meetings
	auto contradictoryMeetings = m_storage->queryMeeting([userName, title, startDate, endDate] (const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getTitle() == title)
			return true;

		// Cheak overlap
		if ((meeting.getSponsor() == userName || meeting.isParticipator(userName))
			&& !(Date(endDate) <= meeting.getStartDate() || Date(startDate) >= meeting.getEndDate())) {
			// Overlap
			return true;
		}	
	 
		return false;
	});

	// If contradictory meeting exists, then fail to create meeting
	if (!contradictoryMeetings.empty()) {
		return false;
	}
	else {
		// create meeting
		m_storage->createMeeting(Meeting(userName, participator, Date(startDate), Date(endDate), title));
		return true;
	}

}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
	if (m_storage == nullptr)
		return false;

	// Check if user is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.empty()) {
		// User don't exists in the user list
		return false;
	}

	// Check if participator is in the user list
	sameNameUsers = m_storage->queryUser([participator](const User& user)->bool {
		return user.getName() == participator;
	});
	if (sameNameUsers.empty()) {
		// Participator don't exists in the user list
		return false;
	}

	// Check if the title exists, sponsor matchs and paticipator exists
	// If it exists, update it
	int updateMeetingNum = m_storage->updateMeeting([userName, title, participator]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getSponsor() == userName && meeting.getTitle() == title
			&& !meeting.isParticipator(participator) && userName != participator) {
			return true;
		}
		else
			return false;
	},
		[participator](Meeting & meeting)->void {
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

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator) {
	if (m_storage == nullptr)
		return false;

	// Check if sponsor is in the user list
	auto sameNameUsers = m_storage->queryUser([userName](const User& user)->bool {
		return user.getName() == userName;
	});
	if (sameNameUsers.empty()) {
		// User don't exists in the user list
		return false;
	}

	// // Check if participator is in the user list
	// sameNameUsers = m_storage->queryUser([participator](const User& user)->bool {
	// 	return user.getName() == participator;
	// });
	// if (sameNameUsers.empty()) {
	// 	// Sponsor don't exists in the user list
	// 	return false;
	// }

	// Check if the title exists, sponsor matchs and paticipator exists
	// If it exists, update it
	int updateMeetingNum = m_storage->updateMeeting([userName, title, participator]
		(const Meeting& meeting)->bool {
		// If title already exists
		if (meeting.getSponsor() == userName 
			&& meeting.getTitle() == title
			&& meeting.isParticipator(participator)) {
			return true;
		}
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

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
	if (m_storage == nullptr)
		return false;

	int updateMeetingNum = m_storage->updateMeeting([userName, title](const Meeting& meeting)->bool {
		return (meeting.getTitle() == title && meeting.isParticipator(userName));
	},
		[userName](Meeting & meeting)->void {
			meeting.removeParticipator(userName);
	});

	m_storage->deleteMeeting([](const Meeting &meeting)->bool {
		return meeting.getParticipator().empty();
	});

	if (updateMeetingNum == 0) {
		// No meeting match
		return false;
	}
	else {
		return true;
	}	
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const {
	return m_storage->queryMeeting([userName, title](const Meeting& meeting)->bool {
		return (meeting.getTitle() == title 
			&& (meeting.getSponsor() == userName || meeting.isParticipator(userName)));
	});
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
	if (!Date::isValid(Date(startDate)) || !Date::isValid(Date(endDate))
		|| Date(startDate) > Date(endDate) || Date(startDate) == Date() || Date(endDate) == Date()) 
		return std::list<Meeting>();

	return m_storage->queryMeeting([userName, startDate, endDate]
		(const Meeting& meeting)->bool {
			return ((meeting.getSponsor() == userName || meeting.isParticipator(userName))
				&& !(Date(endDate) < meeting.getStartDate() || Date(startDate) > meeting.getEndDate()));
	});
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return (meeting.getSponsor() == userName || meeting.isParticipator(userName));
	});
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return meeting.getSponsor() == userName;
	});
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
	return m_storage->queryMeeting([userName]
		(const Meeting& meeting)->bool {
			return meeting.isParticipator(userName);
	});
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
	if (m_storage == nullptr)
		return false;

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

bool AgendaService::deleteAllMeetings(const std::string &userName) {
	if (m_storage == nullptr)
		return false;

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

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage = nullptr;
}