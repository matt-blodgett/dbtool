#pragma once

#ifndef DBERRORS_H
#define DBERRORS_H


#include <exception>
#include <map>


class DBException : public std::exception {

public:
    explicit DBException();

    const char *what() const noexcept {
        return "DBException: call msg() for a printable string.";
    }

    void set(long errnum, std::string sqlstate, std::string sqlmessage);
    std::string msg();

    static DBException from_state(std::string sqlstate);
    static std::string get_reason(std::string sqlstate);

    static const std::map<std::string, std::string> SQLStateMessages;
    static const std::map<std::string, std::string> SQLStateClasses;

private:
    long m_errnum;
    std::string m_sqlstate;
    std::string m_sqlmessage;
    time_t m_timestamp;

};

class InternalError : public DBException {};
class DatabaseError : public DBException {};
class DataError : public DatabaseError {};
class IntegrityError : public DatabaseError {};
class InvalidStateError : public DatabaseError {};
class SyntaxError : public DBException {};



#endif
