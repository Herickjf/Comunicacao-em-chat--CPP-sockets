#ifndef __LIBTSLOG_HPP__
#define __LIBTSLOG_HPP__

#include <iostream>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <ctime>
#include <csignal>
#include "ticketLock.hpp"
#include "colors.hpp"

using namespace std;

// Instância global do TicketLock
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
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_INFO_COLOR << "[INFO]" << COLOR_RESET << " - " 
         << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_WARN(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_WARN_COLOR << "[WARN]" << COLOR_RESET << " - " 
         << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_DEBUG(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_DEBUG_COLOR << "[DEBUG]" << COLOR_RESET << " - " 
         << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_SUCCESS(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_SUCCESS_COLOR << "[SUCCESS]" << COLOR_RESET << " - " 
         << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

void LOG_ERROR(string message){
    ticket.Lock();
    // INICIO - Seção threadSafe
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_ERROR_COLOR << "[ERROR]" << COLOR_RESET << " - " 
         << message << endl;
    // FIM -  Seção threadSafe
    ticket.Unlock();
}

// Versões com formatação adicional
void LOG_INFO(string message, string context){
    ticket.Lock();
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_INFO_COLOR << "[INFO]" << COLOR_RESET << " - "
         << "[" << context << "] - " 
         << message << endl;
    ticket.Unlock();
}

void LOG_WARN(string message, string context){
    ticket.Lock();
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_WARN_COLOR << "[WARN]" << COLOR_RESET << " - "
         << "[" << context << "] - " 
         << message << endl;
    ticket.Unlock();
}

void LOG_DEBUG(string message, string context){
    ticket.Lock();
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_DEBUG_COLOR << "[DEBUG]" << COLOR_RESET << " - "
         << "[" << context << "] - " 
         << message << endl;
    ticket.Unlock();
}

void LOG_SUCCESS(string message, string context){
    ticket.Lock();
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_SUCCESS_COLOR << "[SUCCESS]" << COLOR_RESET << " - "
         << "[" << context << "] - " 
         << message << endl;
    ticket.Unlock();
}

void LOG_ERROR(string message, string context){
    ticket.Lock();
    cout << COLOR_WHITE STYLE_DIM << getTime() << COLOR_RESET << " - " 
         << LOG_ERROR_COLOR << "[ERROR]" << COLOR_RESET << " - "
         << "[" << context << "] - " 
         << message << endl;
    ticket.Unlock();
}

#endif