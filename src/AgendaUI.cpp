#include "AgendaUI.hpp"
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;

AgendaUI::AgendaUI() {
    startAgenda();
}

void AgendaUI::OperationLoop(void) {
    std::string operation;
    while (true) {
        cout << "\n"
             << "------------------------------------Agenda------------------------------------" << endl
             << "Action :" << endl
             << "l   - log in Agenda by user name and password" << endl
             << "r   - register an Agenda account" << endl
             << "q   - quit Agenda" << endl
             << "------------------------------------------------------------------------------" << endl;
        cout << "\nAgenda : ~$ ";
        cin >> operation;
        if (operation == "l") {
            userLogIn();
        }
        else if (operation == "r") {
            userRegister();
        }
        else if (operation == "q") {
            quitAgenda();
            break;
        }
        else {
            cout << "*** Commmand not found! ***" << endl;
        }
    }
}

/**
* constructor
*/
void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
}
/**
* catch user's operation
* @return the operation
*/
std::string AgendaUI::getOperation() {
    cout << "\n"
         << "------------------------------------Agenda------------------------------------" << endl
         << "Action :" << endl
         << "o    - log out Agenda" << endl
         << "dc   - delete Agenda account" << endl
         << "lu   - list all Agenda user" << endl
         << "cm   - create a meeting" << endl
         << "amp  - add meeting participator" << endl
         << "rmp  - remove meeting participator" << endl
         << "rqm  - request to quit meeting" << endl
         << "la   - list all meetings" << endl
         << "las  - list all sponsor meetings" << endl
         << "lap  - list all participate meetings" << endl
         << "qm   - query meeting by title" << endl
         << "qt   - query meeting by time interval" << endl
         << "dm   - delete meeting by title" << endl
         << "da   - delete all meeting" << endl
         << "------------------------------------------------------------------------------" << endl
         << "\nAgenda@" << m_userName << " : ~# ";
    std::string operation;
    cin >> operation;
    return operation;

}

/**
* execute the operation
* @return if the operationloop continue
*/
bool AgendaUI::executeOperation(std::string t_operation) {
    if (t_operation == "o") {
        userLogOut();
        return false;
    }
    else if (t_operation == "dc") {
        deleteUser();
        return false;
    }
    else if (t_operation == "lu") {
        listAllUsers();
    }
    else if (t_operation == "cm") {
        createMeeting();
    }
    else if (t_operation == "amp") {
        addMeetingParticipator();
    }
    else if (t_operation == "rmp") {
        removeMeetingParticipator();
    }
    else if (t_operation == "rqm") {
        requestToQuitMeeting();
    }
    else if (t_operation == "la") {
        listAllMeetings();
    }
    else if (t_operation == "las") {
        listAllSponsorMeetings();
    }
    else if (t_operation == "lap") {
        listAllParticipateMeetings();
    }
    else if (t_operation == "qm") {
        queryMeetingByTitle();
    }
    else if (t_operation == "qt") {
        queryMeetingByTimeInterval();
    }
    else if (t_operation == "dm") {
        deleteMeetingByTitle();
    }
    else if (t_operation == "da") {
        deleteAllMeetings();
    }
    else {
        cout << "*** Commmand not found ***" << endl;
    }
    return true;
}

/**
* user Login
*/
void AgendaUI::userLogIn(void) {
    cout << "\n[log in] [user name] [password]" << endl
         << "[log in] ";
    std::string name, password, operation;
    cin >> name >> password;
    if (m_agendaService.userLogIn(name, password)) {
        cout << "[log in] succeed!" << endl;
        m_userName = name;
        m_userPassword = password;
        while (executeOperation(getOperation()));
    }
    else {
        cout << "[log in] Password error or user doesn't exist" << endl;
    }
}

/**
* user regist
*/
void AgendaUI::userRegister(void) {
    cout << "\n[register] [user name] [password] [email] [phone]" << endl
         << "[register] ";
    std::string name, password, email, phone;
    cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name, password, email, phone)) {
        cout << "[register] succeed!" << endl;
    }
    else {
        cout << "[register] This username has been registered!" << endl;
    }
}

/**
* user logout
*/
void AgendaUI::userLogOut(void) {
    cout << "\n"
         << "*************************" << endl
         << "*                       *" << endl
         << "*        Log Out        *" << endl
         << "*                       *" << endl
         << "*************************" << endl;
}

/**
* quit the Agenda
*/
void AgendaUI::quitAgenda(void) {
    cout << "\n"
         << "*****************************" << endl
         << "*                           *" << endl
         << "*        Quit Agenda        *" << endl
         << "*                           *" << endl
         << "*****************************\n" << endl;
}

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
    cout << "[list all users]\n" << endl
         << std::left << std::setw(20) << "name" 
         << std::setw(25) << "email" << std::setw(20) << "phone" << endl;
    std::list<User> users = m_agendaService.listAllUsers();
    for (auto user : users) {
        cout << std::left << std::setw(20) << user.getName() 
        << std::setw(25) << user.getEmail() << std::setw(20) << user.getPhone() << endl;
    }
}

/**
* user create a meeting with someone else
*/
void AgendaUI::createMeeting(void) {
    cout << "[create meeting] [the number of participators]" << endl
         << "[create meeting] ";
    int numOfParticipators;
    while (true) {
        cin >> numOfParticipators;
        if (numOfParticipators >= 0)
            break;
        else
            cout << "[create meeting] [please enter a natural number]" << endl;
    }

    std::vector<std::string> participators;
    std::string participator;
    for (int i = 1; i <= numOfParticipators; ++i) {
        cout << "[create meeting] [please enter the participator " << i << " ]"<< endl
             << "[create meeting] " << endl;
        cin >> participator;
        participators.push_back(participator);
    }
    
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mmdd/hh:mm)]" << endl;
    std::string title, startTime, endTime;
    cin >> title >> startTime >> endTime;
    if (m_agendaService.createMeeting(m_userName, title, startTime, endTime, participators))
        cout << "[create meeting] succeed!" << endl;
    else
        cout << "[create meeting] error!" << endl;
}

void AgendaUI::addMeetingParticipator(void) {
    cout << "[add participator] [meeting title] [participator username]" << endl
         << "[add participator] ";
    std::string title, participator;
    cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator))
        cout << "[add participator] succeed!" << endl;
    else
        cout << "[add participator] error!" << endl;

}

void AgendaUI::removeMeetingParticipator(void) {
    cout << "[remove participator] [meeting title] [participator username]" << endl
         << "[meeting title] ";
    std::string title, participator;
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator))
        cout << "[remove participator] succeed!" << endl;
    else
        cout << "[remove participator] error!" << endl;
}

void AgendaUI::requestToQuitMeeting(void) {
    cout << "[quit meeting] [meeting title]" << endl
         << "[quit meeting] ";
    std::string title;
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title))
        cout << "[quit meeting] succeed!" << endl;
    else
        cout << "[quit meeting] error!" << endl;
}

/**
* list all meetings from storage
*/
void AgendaUI::listAllMeetings(void) {
    cout << "[list all meetings]" << endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}

/**
* list all meetings that this user sponsored
*/
void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meetings]" << endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

/**
* list all meetings that this user take part in
*/
void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all participator meetings]" << endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

/**
* search meetings by title from storage
*/
void AgendaUI::queryMeetingByTitle(void) {
    cout << "[query meeting] [title]:"
         << "[query meeting] ";
    std::string title;
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

/**
* search meetings by timeinterval from storage
*/
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
         << "[query meeting] ";
    std::string startTime, endTime;
    cin >> startTime >> endTime;
    printMeetings(m_agendaService.meetingQuery(m_userName, startTime, endTime));
}

/**
* delete meetings by title from storage
*/
void AgendaUI::deleteMeetingByTitle(void) {
    cout << "[delete meeting] [title]" << endl
         << "[delete meeting] ";
    std::string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title))
        cout << "[delete meeting] succeed!" << endl;
    else
        cout << "[delete meeting] delete meeting fail!" << endl;
}

/**
* delete all meetings that this user sponsored
*/
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName))
        cout << "[delete all meeting] succeed!" << endl;
    else
        cout << "[delete all meeting] delete all meeting fail!" << endl;
}

/**
* show the meetings in the screen
*/
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    cout << std::left << std::setw(18) << "title" 
         << std::setw(20) << "sponsor" 
         << std::setw(20) << "start time"
         << std::setw(20) << "end time"
         << "participators" << endl;
    for (auto meeting : t_meetings) {
        cout << std::left << std::setw(18) << meeting.getTitle()
             << std::setw(20) << meeting.getSponsor()
             << std::setw(20) << Date::dateToString(meeting.getStartDate())
             << std::setw(20) << Date::dateToString(meeting.getEndDate());
        auto participators = meeting.getParticipator();
        if (!participators.empty()) {
            cout << *(participators.begin());
            for (auto iter = participators.begin() + 1; iter != participators.end(); ++iter) {
                cout << "," << *iter;
            }
        }
        cout << endl;
    }
}