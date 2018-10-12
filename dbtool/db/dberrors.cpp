#include "dberrors.h"
#include <chrono>
#include <sstream>


DBException::DBException() {
    timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

std::string DBException::msg(){
    std::stringstream ss;
    ss << "\n--------------------\n";
    ss << "SQLERROR: " << errnum << "\n";
    ss << "SQLSTATE: " << sqlstate << "\n";
    ss << "REASON: " << Errs.at(sqlstate) << "\n";
    ss << "MESSAGE: \n" << sqlmessage;
    ss << "\n--------------------\n";
    return ss.str();
}

