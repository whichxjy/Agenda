#include "AgendaUI.hpp"

using std::cin;
using std::cout;
using std::endl;

AgendaUI::AgendaUI() {
    startAgenda();
}

void AgendaUI::OperationLoop(void) {
    do
    {
        std::string operation = getOperation();
    } while (executeOperation(operation));

}

/**
* constructor
*/
void AgendaUI::startAgenda(void) {
    cout << "-------------------------Agenda------------------------------" << endl
         << "Action :" << endl
         << "l   - log in Agenda by user name and password" << endl
         << "r   - register an Agenda account" << endl
         << "q   - quit Agenda" << endl
         << "-------------------------------------------------------------" << endl;       
}
/**
* catch user's operation
* @return the operation
*/
std::string AgendaUI::getOperation() {
    if (m_userName == "")
        cout << "\nAgenda : ~$";
    else
        cout << "\nAgenda@" << m_userName << " : #";
    std::string operation;
    cin >> operation;
    return operation;
}

/**
* execute the operation
* @return if the operationloop continue
*/
bool AgendaUI::executeOperation(std::string t_operation);

/**
* user Login
*/
void AgendaUI::userLogIn(void) {
    cout << "\n[log in] [user name] [password]" << endl;
         << "[log in] ";
    std::string name, password;
    cin >> name >> password;
    if (m_agendaService.userLogIn(name, password)) {
        cout << "[log in] succeed!" << endl;
        m_userName = name;
        m_userPassword = password;
    }
    else {
        cout << "[error] log in fail!" << endl;
    }
}

/**
* user regist
*/
void AgendaUI::userRegister(void) {
    cout << "\n[register] [user name] [password] [email] [phone]" << endl;
         << "[register] ";
    std::string name, password, email, phone;
    cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name, password, email, phone)) {
        cout << "[register] succeed!\n" << endl;
             << "-------------------------Agenda------------------------------" << endl
             << "Action :" << endl
             << "o   - log out Agenda" << endl
             << "dc  - delete Agenda account" << endl
             << "lu  - list all Agenda user" << endl
             << "cm  - create a meeting" << endl
             << "la  - list all meetings" << endl
             << "las - list all sponsor meetings" << endl
             << "lap - list all participate meetings" << endl
             << "qm  - query meeting by title" << endl
             << "qt  - query meeting by time interval" << endl
             << "dm  - delete meeting by title" << endl
             << "da  - delete all meeting" << endl
             << "-------------------------------------------------------------" << endl; 
    }
    else {
        cout << "[error] register fail!" << endl;
    }
}

/**
* user logout
*/
void AgendaUI::userLogOut(void) {

}

/**
* quit the Agenda
*/
void AgendaUI::quitAgenda(void);

/**
* delete a user from storage
*/
void AgendaUI::deleteUser(void) {
    cout << "[delete Agenda account] ";
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        cout << "succeed!" << endl;
    }
    else {
        cout << "delete Agenda account fail" << endl;
    }
}

/**
* list all users from storage
*/
void AgendaUI::listAllUsers(void) {
    std::list<User> users = m_agendaService.listAllUsers();
    cout << "[list all users]\n" << endl
         << "name    "
}

/**
* user create a meeting with someone else
*/
void AgendaUI::createMeeting(void);

/**
* list all meetings from storage
*/
void AgendaUI::listAllMeetings(void);

/**
* list all meetings that this user sponsored
*/
void AgendaUI::listAllSponsorMeetings(void);

/**
* list all meetings that this user take part in
*/
void AgendaUI::listAllParticipateMeetings(void);

/**
* search meetings by title from storage
*/
void AgendaUI::queryMeetingByTitle(void);

/**
* search meetings by timeinterval from storage
*/
void AgendaUI::queryMeetingByTimeInterval(void);

/**
* delete meetings by title from storage
*/
void AgendaUI::deleteMeetingByTitle(void);

/**
* delete all meetings that this user sponsored
*/
void AgendaUI::deleteAllMeetings(void);

/**
* show the meetings in the screen
*/
void AgendaUI::printMeetings(std::list<Meeting> t_meetings);