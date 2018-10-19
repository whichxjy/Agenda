#include "Date.hpp"
Date::Date() : m_year(1999), m_month(1), m_day(1), m_hour(0), m_minute(0) {
}

Date::Date() : m_year(t_year), m_month(t_month), m_day(t_day),
				m_hour(t_hour), m_minute(t_minute) {	
}

Date::Date(std::string dateString) {

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

void setDay(const int t_day) {
	m_day = t_day;	
}

int getHour(void) const {
	return m_day;	
}

void setHour(const int t_hour) {
	m_hour = t_hour;	
}

int getMinute(void) const {
	return m_hour;	
}

void setMinute(const int t_minute) {
	m_minute = t_minute;	
}

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
bool Date::isValid(const Date t_date) {
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
	

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  static Date stringToDate(const std::string t_dateString);

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
  static std::string dateToString(Date t_date);

  /**
  *  @brief overload the assign operator
  */
  Date &operator=(const Date &t_date);

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool operator==(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool operator>(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool operator<(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool operator>=(const Date &t_date) const;

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool operator<=(const Date &t_date) const;


