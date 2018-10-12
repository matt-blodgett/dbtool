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



struct DBPrivilege
{
    std::string catalog;
    std::string schema;
    std::string table;
    std::string column;
    std::string grantor;
    std::string grantee;
    std::string privilege;
    std::string isGrantable;
};


struct DBTable
{
    std::string catalog;
    std::string schema;
    std::string name;
    std::string type;
    std::string remarks;
};


struct DBDriver
{
    std::string name;
    std::string desc;
};


struct DBSource
{
    std::string name;
    std::string desc;
};


struct DBProcedure
{
    std::string catalog;
    std::string schema;
    std::string name;
    std::string numInParams;
    std::string numOutParams;
    std::string numResultSets;
    std::string remarks;
    SQLSMALLINT type;
};


struct DBField
{
    SQLUSMALLINT index;
    std::string value;
    SQLSMALLINT datatype;
};


struct DBRecord
{
    SQLUSMALLINT index;
    std::vector<DBField> fields;
};


struct DBColumn
{
    std::string catalog;
    std::string schema;
    std::string table;
    std::string name;
    SQLINTEGER index;
    SQLINTEGER size;
    std::string remarks;
    SQLSMALLINT datatypeCType;
    std::string datatypeName;
    SQLINTEGER bufferLength;
    SQLSMALLINT decimalDigits;
    SQLSMALLINT numPrecRadix;
    SQLSMALLINT nullableCVal;
    std::string defaultValue;
    SQLSMALLINT datatypeSQL;
    SQLSMALLINT datetimeSubtypeCode;
    SQLINTEGER charOctectLength;
    std::string isNullable;
    SQLSMALLINT procedureParamType;
    bool procedureColumn;
};


struct DBParam
{
    SQLSMALLINT iotype;
    SQLSMALLINT valtype;
    SQLSMALLINT sqltype;
    SQLUINTEGER colsize;
    SQLPOINTER paramval;
    SQLSMALLINT decimals;
    SQLSMALLINT buflen;
    SQLINTEGER strlen;
};


struct DBConnString
{
    std::string connection_string;
    void add_arg(std::string key, std::string val);
    void set_cns(std::string cns);
};


typedef std::vector<DBDriver> DBDriverList;
typedef std::vector<DBSource> DBSourceList;
typedef std::vector<DBProcedure> DBProcedureList;
typedef std::vector<DBPrivilege> DBPrivilegeList;

typedef std::vector<DBTable> DBTableSet;
typedef std::vector<DBColumn> DBColumnSet;

typedef std::vector<DBField> DBFieldSet;
typedef std::vector<DBRecord> DBRecordSet;


class DBConnection
{

public:
    explicit DBConnection();
    ~DBConnection();

public:
    static std::string to_string(SQLCHAR *uchar);
    static SQLCHAR* to_uchar(std::string *str);
    static std::string sql_from_file(const std::string &fpath);
    static void csv_write(const std::string &fpath, const DBRecordSet &rows);

public:
    DBDriverList get_drivers();
    DBSourceList get_sources();
    DBTableSet get_tables(std::string catalog, std::string schema, std::string table, std::string types);
    DBColumnSet get_columns(std::string catalog, std::string schema, std::string table, std::string column, bool procedure = false);
    DBProcedureList get_procedures(std::string catalog, std::string schema, std::string proc);
    DBPrivilegeList get_privileges(std::string catalog, std::string schema, std::string table, std::string column, bool getTablePrivileges);
    DBPrivilegeList get_column_privileges(std::string catalog, std::string schema, std::string table, std::string column);
    DBPrivilegeList get_table_privileges(std::string catalog, std::string schema, std::string table);

public:
    void initialize();
    void connect(const DBConnString &cns);
    void disconnect();
    void close();

    void prepare(const std::string &qry);
    void execute(const std::string &qry);
    void execute();

    void build_columns();
    DBColumnSet columns() const;
    DBRecord headers() const;

    void bind(const SQLUSMALLINT &index, DBParam param);

    DBRecordSet fetch_all();
    DBRecordSet fetch_many(const SQLINTEGER &limit);
    DBRecord fetch_one();
    DBRecord fetch();

    void end();

public:
    bool opened() const;
    bool connected() const;

    SQLINTEGER rowcount() const;
    SQLSMALLINT colcount() const;

    SQLHANDLE handle(const SQLSMALLINT &ht) const;

private:
    bool rc_error(const RETCODE &rc, const SQLSMALLINT &ht);

    bool m_opened;
    bool m_connected;
    DBConnString m_dbcns;
    DBColumnSet m_columns;
    SQLUSMALLINT m_row;

    SQLHENV m_henv;
    SQLHDBC m_hdbc;
    SQLHSTMT m_hstmt;
};


#endif











