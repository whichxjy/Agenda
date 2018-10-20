#include "Storage.hpp"


/**
*   default constructor
*/
Storage::Storage() {

}

/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr)
		m_instance = make_shared<Storage>();
	return m_instance;
}


/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> f_users;
	for (auto user : m_userList) {
		if (filter(user))
			f_users.push_back(user);
	}
	return f_users;
}

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
             std::function<void(User &)> switcher) {
	int num = 0;
	for (auto &user : m_userList) {
		if (filter(user)){
			switcher(user);
			++num;
		}
	}
	return num;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int num = 0;
	for (auto iter = m_userList.begin(); iter != m_userList.end(); ++iter) {
		if (filter(*iter)) {
			m_userList.erase(iter);
			++num;
		}
	}
	return num;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.push_back(t_meeting);
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(
  std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting> f_meetings;
	for (auto meeting : m_meetingList) {
		if (filter(meeting))
			f_meetings.push_back(user);
	}
	retrun f_meetings;
}

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                std::function<void(Meeting &)> switcher) {
	int num = 0;
	for (auto &meeting : m_meetingList) {
		if (filter(meeting)){
			switcher(meeting);
			++num;
		}
	}
	return num;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int num = 0;
	for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ++iter) {
		if (filter(*iter)) {
			m_meetingList.erase(iter);
			++num;
		}
	}
	return num;
}

/**
* sync with the file
*/
bool Storage::sync(void) {

}