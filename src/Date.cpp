#include "Date.hpp"
#include <sstream>
#include <cctype>
#include <iomanip>

Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(const std::string &dateString) {
	*this = stringToDate(dateString);
}

int Date::getYear(void) const {
	return m_year;	
}

void Date::setYear(const int t_year) {
	m_year = t_year;	
}

int Date::getMonth(void) const {
	return m_month;	
}

void Date::setMonth(const int t_month) {
	m_month = t_month;	
}

int Date::getDay(void) const {
	return m_day;	
}

void Date::setDay(const int t_day) {
	m_day = t_day;	
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;	
}

int Date::getMinute(void) const {
	return m_minute;	
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;	
}

bool Date::isValid(const Date &t_date) {
	int year = t_date.getYear(), month = t_date.getMonth(), day = t_date.getDay(),
		hour = t_date.getHour(), minute = t_date.getMinute();
	
	if (year < 1000 || year > 9999 || month < 1 || month > 12
	    || day < 1 || hour < 0 || hour > 23 || minute < 0 || minute > 59)
		return false;
	
	if ((month == 1 || month == 3 || month == 5 || month == 7
	     || month == 8 || month == 10 || month == 12) && day > 31) {
		return false;	
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
		return false;	
	}
	else if (month == 2) {
		//check whether this year is leap year
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
			//leap year
			if (day > 29)
				return false;
		}
		else {
			//not leap year
			if (day > 28)
				return false;
		}
	}	
	return true;
}
	
Date Date::stringToDate(const std::string &t_dateString) {
	// check the format of string
	if (t_dateString.size() != 16 || t_dateString[4] != '-'
	    || t_dateString[7] != '-' || t_dateString[10] != '/'
	    || t_dateString[13] != ':' || !isdigit(t_dateString[0])
	    || !isdigit(t_dateString[1]) || !isdigit(t_dateString[2])
	    || !isdigit(t_dateString[3]) || !isdigit(t_dateString[5])
	    || !isdigit(t_dateString[6]) || !isdigit(t_dateString[8])
	    || !isdigit(t_dateString[9]) || !isdigit(t_dateString[11])
	    || !isdigit(t_dateString[12]) || !isdigit(t_dateString[14])
	    || !isdigit(t_dateString[15])) {
		// Not correct
		return Date();
	}
	else {
		// Correct
		Date date;
		date.setYear(std::stoi(t_dateString.substr(0, 4)));
		date.setMonth(std::stoi(t_dateString.substr(5, 2)));
		date.setDay(std::stoi(t_dateString.substr(8, 2)));
		date.setHour(std::stoi(t_dateString.substr(11, 2)));
		date.setMinute(std::stoi(t_dateString.substr(14, 2)));
		return date;
	}
}

std::string Date::dateToString(const Date &t_date) {
	if (!isValid(t_date))
		return std::string("0000-00-00/00:00");
	std::ostringstream ostr;
	ostr << std::setfill('0') << std::setw(4) << t_date.getYear() << "-"
		<< std::setw(2) << t_date.getMonth() << "-"
		<< std::setw(2) << t_date.getDay() << "/"
		<< std::setw(2) << t_date.getHour() << ":"
		<< std::setw(2) << t_date.getMinute();
	return ostr.str();
}

Date& Date::operator=(const Date &t_date) {
	m_year = t_date.getYear();
	m_month = t_date.getMonth();
	m_day = t_date.getDay();
	m_hour = t_date.getHour();
	m_minute = t_date.getMinute();
	return *this;
}

bool Date::operator==(const Date &t_date) const {
	return (m_year == t_date.getYear() && m_month == t_date.getMonth()
			&& m_day == t_date.getDay() && m_hour == t_date.getHour()
			&& m_minute == t_date.getMinute());
}

bool Date::operator>(const Date &t_date) const {
	return (m_year > t_date.getYear()
			|| (m_year == t_date.getYear() && m_month > t_date.getMonth())
			|| (m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day > t_date.getDay())
			|| (m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() 
				&& m_hour > t_date.getHour())
			|| (m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() 
				&& m_hour == t_date.getHour() && m_minute > t_date.getMinute())
			);
}

bool Date::operator<(const Date &t_date) const {
	return !(*this >= t_date);
}

bool Date::operator>=(const Date &t_date) const {
	return (*this > t_date || *this == t_date);
}

bool Date::operator<=(const Date &t_date) const {
	return !(*this > t_date);
}
