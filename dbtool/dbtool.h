#pragma once

#if defined(DBTOOL_LIB)
#   define DBTOOLSHARED_EXPORT __declspec(dllexport)
#else
#   define DBTOOLSHARED_EXPORT __declspec(dllimport)
#endif


#ifndef DBCONNECTION_H
#define DBCONNECTION_H


#include <windows.h>
#include <sqlext.h>
#include <string>
#include <vector>


struct DBColumn
{
    std::wstring name;
    SQLUSMALLINT index;
    SQLSMALLINT datatype;
    SQLSMALLINT decimals;
    SQLSMALLINT nullable;
    SQLULEN datalen;

    void load(SQLHSTMT *hstmt, SQLUSMALLINT column);
};


class DBConnection
{

public:
    DBConnection();

public:
    void initialize();
    void connect(std::string drv, std::string sys, std::string usr, std::string pwd);
    void disconnect();

    void prepare(std::string qry);
    void execute();
    void fetch();

    void db_error(SQLSMALLINT handletype);

    std::vector<std::string> outparams;

public:
    void getmaniinfo(SQLINTEGER pronum);

public:
    SQLSMALLINT column_count() const;

public:
    std::wstring sql_getinfo(const SQLUSMALLINT info);
    void write_db_info(std::string filepath = std::string());

private:
    bool m_initialized;
    bool m_connected;

private:
    SQLHENV m_henv;
    SQLHDBC m_hdbc;
    SQLHSTMT m_hstmt;
};


#endif
