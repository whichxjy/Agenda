#include <iostream>
#include "Date.hpp"

using namespace std;

int main() {
	Date date1("1000-22-11/23:11");
	std::string date2 = Date::dateToString(date1);

	if (date2 == "0000-00-00/00:00")
		cout << "equal" << endl;

	return 0;
}