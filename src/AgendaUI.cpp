#include "AgendaUI.hpp"
#include "AgendaError.hpp"

#include <iomanip>

using std::cin;
using std::cout;
using std::endl;

void sig_handler(int sig) {
    if (sig == SIGINT) {
        Logger::getLogger()->terminateProgram();
        Storage::getInstance()->sync();
        cout << "\n"
             << "***********************************" << endl
             << "*                                 *" << endl
             << "*        TERMINATE PROGRAM        *" << endl
             << "*                                 *" << endl
             << "***********************************\n" << endl;
    }
    exit(sig);
}

AgendaUI::AgendaUI() {
    logger = Logger::getLogger();
    startAgenda();
}

void AgendaUI::OperationLoop(void) {
    signal(SIGINT, sig_handler);
    std::string operation;
    while (true) {
        cout << "\n"
             << "------------------------------------Agenda------------------------------------" << endl
             << "Action :" << endl
             << "l   - log in Agenda by user name and password" << endl
             << "r   - register an Agenda account" << endl
             << "q   - quit Agenda" << endl
             << "------------------------------------------------------------------------------" << endl;
        cout << "\nAgenda :~$ ";
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
    logger->startAgenda();
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
         << "\nAgenda@" << m_userName << " :~# ";
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
        logger->userLogIn(SUCCESS, name);
        while (executeOperation(getOperation()));
    }
    else {
        cout << "[log in] Password error or user doesn't exist" << endl;
        logger->userLogIn(FAILURE, name);
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
        logger->userRegister(SUCCESS, name, email, phone);
    }
    else {
        cout << "[register] This username has been registered!" << endl;
        logger->userRegister(FAILURE, name, email, phone);
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
         << "*************************\n" << endl;
    logger->userLogOut(m_userName);
}

/**
* quit the Agenda
*/
void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
    cout << "\n"
         << "*****************************" << endl
         << "*                           *" << endl
         << "*        Quit Agenda        *" << endl
         << "*                           *" << endl
         << "*****************************\n" << endl;
    logger->quitAgenda();
}

/**
* delete a user from storage
*/
void AgendaUI::deleteUser(void) {
    cout << "\n[delete Agenda account] ";
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        cout << "succeed!" << endl;
        logger->deleteUser(SUCCESS, m_userName);
    }
    else {
        cout << "delete Agenda account fail" << endl;
        logger->deleteUser(FAILURE, m_userName);
    }
}

/**
* list all users from storage
*/
void AgendaUI::listAllUsers(void) {
    cout << "\n[list all users]\n" << endl
         << std::left << std::setw(20) << "name" 
         << std::setw(25) << "email" << std::setw(20) << "phone" << endl;
    std::list<User> users = m_agendaService.listAllUsers();
    for (auto user : users) {
        cout << std::left << std::setw(20) << user.getName() 
        << std::setw(25) << user.getEmail() << std::setw(20) << user.getPhone() << endl;
    }
    logger->listAllUsers(m_userName);
}

/**
* user create a meeting with someone else
*/
void AgendaUI::createMeeting(void) {
    cout << "\n[create meeting] [the number of participators]" << endl
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
             << "[create meeting] ";
        cin >> participator;
        participators.push_back(participator);
    }
    
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mmdd/hh:mm)]" << endl
         << "[create meeting] ";
    std::string title, startTime, endTime;
    cin >> title >> startTime >> endTime;
    try {
        m_agendaService.createMeeting(m_userName, title, startTime, endTime, participators);
        cout << "[create meeting] succeed!" << endl;
        logger->createMeeting(SUCCESS, m_userName, title, startTime, endTime, participators);
    }
    catch (const AgendaError& e) {
        cout << "[create meeting] " << e.what() << endl;
        logger->createMeeting(FAILURE, m_userName, title, startTime, endTime, participators);
    }
}

void AgendaUI::addMeetingParticipator(void) {
    cout << "\n[add participator] [meeting title] [participator username]" << endl
         << "[add participator] ";
    std::string title, participator;
    cin >> title >> participator;
    try {
        m_agendaService.addMeetingParticipator(m_userName, title, participator);
        cout << "[add participator] succeed!" << endl;
        logger->addMeetingParticipator(SUCCESS, m_userName, title, participator);
    }
    catch (const AgendaError& e) {
        cout << "[add participator] " << e.what() << endl;
        logger->addMeetingParticipator(FAILURE, m_userName, title, participator);
    }
}

void AgendaUI::removeMeetingParticipator(void) {
    cout << "\n[remove participator] [meeting title] [participator username]" << endl
         << "[remove participator] ";
    std::string title, participator;
    cin >> title >> participator;
    try {
        m_agendaService.removeMeetingParticipator(m_userName, title, participator);
        cout << "[remove participator] succeed!" << endl;
        logger->removeMeetingParticipator(SUCCESS, m_userName, title, participator);
    }
    catch (const AgendaError& e) {
        cout << "[remove participator] " << e.what() << endl;
        logger->removeMeetingParticipator(FAILURE, m_userName, title, participator);
    }
}

void AgendaUI::requestToQuitMeeting(void) {
    cout << "\n[quit meeting] [meeting title]" << endl
         << "[quit meeting] ";
    std::string title;
    cin >> title;
    try {
        m_agendaService.quitMeeting(m_userName, title);
        cout << "[quit meeting] succeed!" << endl;
        logger->requestToQuitMeeting(SUCCESS, m_userName, title);
    }
    catch (const AgendaError& e) {
        cout << "[quit meeting] " << e.what() << endl;
        logger->requestToQuitMeeting(FAILURE, m_userName, title);
    }
}

/**
* list all meetings from storage
*/
void AgendaUI::listAllMeetings(void) {
    cout << "\n[list all meetings]\n" << endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
    logger->listAllMeetings(m_userName);
}

/**
* list all meetings that this user sponsored
*/
void AgendaUI::listAllSponsorMeetings(void) {
    cout << "\n[list all sponsor meetings]\n" << endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
    logger->listAllSponsorMeetings(m_userName);
}

/**
* list all meetings that this user take part in
*/
void AgendaUI::listAllParticipateMeetings(void) {
    cout << "\n[list all participator meetings]\n" << endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
    logger->listAllParticipateMeetings(m_userName);
}

/**
* search meetings by title from storage
*/
void AgendaUI::queryMeetingByTitle(void) {
    cout << "\n[query meeting] [title]" << endl
         << "[query meeting] ";
    std::string title;
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
    logger->queryMeetingByTitle(m_userName, title);
}

/**
* search meetings by timeinterval from storage
*/
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << "\n[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
         << "[query meeting] ";
    std::string startTime, endTime;
    cin >> startTime >> endTime;
    cout << "[query meeting]\n";
    printMeetings(m_agendaService.meetingQuery(m_userName, startTime, endTime));
    logger->queryMeetingByTimeInterval(m_userName, startTime, endTime);
}

/**
* delete meetings by title from storage
*/
void AgendaUI::deleteMeetingByTitle(void) {
    cout << "\n[delete meeting] [title]" << endl
         << "[delete meeting] ";
    std::string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting] succeed!" << endl;
        logger->deleteMeetingByTitle(SUCCESS, m_userName, title);
    }
    else {
        cout << "[delete meeting] delete meeting fail!" << endl;
        logger->deleteMeetingByTitle(FAILURE, m_userName, title);
    }
}

/**
* delete all meetings that this user sponsored
*/
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "\n[delete all meeting] succeed!" << endl;
        logger->deleteAllMeetings(SUCCESS, m_userName);
    }
    else {
        cout << "\n[delete all meeting] delete all meeting fail!" << endl;
        logger->deleteAllMeetings(FAILURE, m_userName);
    }
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