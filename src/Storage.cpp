#include "Storage.hpp"
#include "Path.hpp"
#include <iostream>
#include <fstream>


std::shared_ptr<Storage> Storage::m_instance = nullptr;

/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr)
		m_instance = std::shared_ptr<Storage>(new Storage());
		// m_instance = std::make_shared<Storage>();
	return m_instance;
}

/**
*   default constructor
*/
Storage::Storage() : m_dirty(false) {
	readFromFile();
}

/**
*   destructor
*/
Storage::~Storage() {
	sync();
	m_instance = nullptr;
}

/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
	std::ifstream user_input(Path::userPath), meeting_input(Path::meetingPath);

	if (!user_input.is_open() || !meeting_input.is_open())
		return false;

	m_userList.clear();
	m_meetingList.clear();

	std::string line;

	// Read from user file
	while (std::getline(user_input, line)) {
		std::string user_info[4];
		int item_begin = 0, item_end = -1;
		for (int i = 0; i < 4; ++i) {
			item_begin = line.find('\"', item_end + 1);
			item_end = line.find('\"', item_begin + 1);
			user_info[i] = line.substr(item_begin + 1, item_end - item_begin - 1);
		}
		// Create user
		m_userList.emplace_back(user_info[0], user_info[1], user_info[2], user_info[3]);
	}
	user_input.close();

	// Read from meeting file
	while (std::getline(meeting_input, line)) {
		std::string meeting_info[5];
		int item_begin = 0, item_end = -1;
		for (int i = 0; i < 5; ++i) {
			item_begin = line.find('\"', item_end + 1);
			item_end = line.find('\"', item_begin + 1);
			meeting_info[i] = line.substr(item_begin + 1, item_end - item_begin - 1);
		}

		// Get participator
		std::vector<std::string> participators;
		std::string participator_info(meeting_info[1]);
		item_begin = 0;
		item_end = -1;
		while (true) {
			item_begin = item_end + 1;
			item_end = participator_info.find("&", item_begin + 1);
			if (item_end != std::string::npos) {
				// not the last participator
				participators.emplace_back(
					participator_info.substr(item_begin, item_end - item_begin));
			}
			else {
				// the last participator
				participators.emplace_back(
					participator_info.substr(item_begin));
				break;
			}
		}

		// Create meeting
		m_meetingList.emplace_back(meeting_info[0], participators, Date(meeting_info[2]),
			Date(meeting_info[3]), meeting_info[4]);
	}
	meeting_input.close();

	return true;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
	std::ofstream user_output(Path::userPath), meeting_output(Path::meetingPath);

	if (!user_output.is_open() || !meeting_output.is_open())
		return false;

	// Write to user file
	for (const auto &user : m_userList) {
		user_output << "\"" << user.getName() << "\","
					<< "\"" << user.getPassword() << "\","
					<< "\"" << user.getEmail() << "\","
					<< "\"" << user.getPhone() << "\"" << std::endl;
	}
	user_output.close();

	// Write to meeting file
	for (const auto &meeting : m_meetingList) {
		meeting_output << "\"" << meeting.getSponsor() << "\",\"";

		// write participator with '&'
		auto participators = meeting.getParticipator();
		if (participators.empty()) {
			// No paticipator
			meeting_output << "\","; 
		}
		else {
			// Have paticipator
			meeting_output << *(participators.begin());
			for (auto iter = participators.begin() + 1; iter != participators.end(); ++iter) {
				meeting_output << "&" << *iter;
			}
			meeting_output << "\",";
		}

		meeting_output << "\"" << Date::dateToString(meeting.getStartDate()) << "\","
					   << "\"" << Date::dateToString(meeting.getEndDate()) << "\","
					   << "\"" << meeting.getTitle() << "\"" << std::endl;
	}
	meeting_output.close();

	return true;
}

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
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
	if (num > 0)
		m_dirty = true;
	return num;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int num = 0;
	auto iter = m_userList.begin();
	while (iter != m_userList.end()) {
		if (filter(*iter)) {
			m_userList.erase(iter);
			++num;
		}
		else {
			++iter;
		}
	}
	if (num > 0)
		m_dirty = true;
	return num;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.emplace_back(t_meeting);
	m_dirty = true;
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
			f_meetings.push_back(meeting);
	}
	return f_meetings;
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
	if (num > 0)
		m_dirty = true;
	return num;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int num = 0;
	auto iter = m_meetingList.begin();
	while (iter != m_meetingList.end()) {
		if (filter(*iter)) {
			m_meetingList.erase(iter);
			++num;
		}
		else {
			++iter;
		}
	}
	if (num > 0)
		m_dirty = true;
	return num;
}

/**
* sync with the file
*/
bool Storage::sync(void) {
	if (m_dirty && writeToFile()) {
		m_dirty = false;
		return true;
	}
	return false;
}