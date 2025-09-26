#ifndef __LIBTSLOG_HPP__
#define __LIBTSLOG_HPP__

#include <iostream>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <ctime>
#include "ticketLock.hpp"

using namespace std;
TicketLock ticket;

string getTimeStr(){
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    tm* local_time = localtime(&time);
    
    stringstream ss;
    ss  << setfill('0') << "["
        << setw(2) << local_time->tm_hour << ":"
        << setw(2) << local_time->tm_min << ":"
        << setw(2) << local_time->tm_sec << "]";

    return ss.str();
}      

string getTime() {
    return getTimeStr();
}

void LOG_INFO(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << getTime() << " - [INFO] - " << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_WARN(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << getTime() << " - [WARN] - " << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_DEBUG(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << getTime() << " - [DEBUG] - " << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_SUCCESS(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << getTime() << " - [SUCCESS] - " << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_ERROR(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << getTime() << " - [ERROR] - " << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

#endif