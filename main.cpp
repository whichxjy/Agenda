#include <iostream>
#include "AgendaService.hpp"

using namespace std;

int main() {
	AgendaService A;
	cout << A.userLogIn("xian", "123") << endl;
	cout << A.userRegister("xian", "123","5@qq.com", "135") << endl;
	cout << A.userLogIn("xian", "123") << endl;

	A.quitAgenda();
	return 0;
}