#ifndef AGENDAERROR_HPP
#define AGENDAERROR_HPP

#include <stdexcept>
#include <string>

class AgendaError : public std::logic_error {
public:
	explicit AgendaError(const std::string &msg) : std::logic_error(msg) { }
	~AgendaError() = default;
};

#endif