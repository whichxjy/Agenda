#include "Date.hpp"
Date::Date() : m_year(1999), m_month(1), m_day(1), m_hour(0), m_minute(0) {
}

Date::Date() : m_year(t_year), m_month(t_month), m_day(t_day),
				m_hour(t_hour), m_minute(t_minute) {	
}