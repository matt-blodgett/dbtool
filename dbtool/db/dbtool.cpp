#include "dbtool.h"

#include <iostream>
#include <fstream>
#include <limits.h>

#include "dberrors.h"


#define DBC_LEN_STR 128 + 1
#define DBC_LEN_REM 254 + 1




void DBConnString::add_arg(std::string key, std::string val){
    connection_string += key + "=" + val + ";";
}
void DBConnString::set_cns(std::string cns){
    connection_string.clear();
    connection_string = cns;
}





void DBConnection::csv_out(std::string fpath){
    std::ofstream ofs;
    ofs.open(fpath.c_str());

//    for(size_t cidx = 0; cidx < m_columns.size(); cidx++){
//        ofs << m_columns.at(cidx).name << ",";
//    }

//    ofs << std::endl;

    DBRecordSet rows = fetch_all();
    for(size_t ridx = 0; ridx < rows.size(); ridx++){

        DBRecord row = rows.at(ridx);
        for(size_t i = 0; i < row.fields.size(); i++){
            ofs << row.fields.at(i).value << ",";
        }

        ofs << std::endl;
    }

    ofs.close();
}




DBConnection::DBConnection(){
    m_henv = nullptr;
    m_hdbc = nullptr;
    m_hstmt = nullptr;
    m_initialized = false;
    m_connected = false;
}
DBConnection::~DBConnection(){
    disconnect();
}

std::string DBConnection::to_string(SQLCHAR *uch){
    return std::string(reinterpret_cast<CHAR*>(uch));
}
SQLCHAR* DBConnection::to_uchar(std::string *str){
    return reinterpret_cast<SQLCHAR*>(const_cast<CHAR *>(str->c_str()));
}
std::string DBConnection::sql_from_file(std::string fpath){
    std::ifstream ifs;
    ifs.open(fpath.c_str());

    std::string sql;
    ifs.seekg(0, std::ios::end);
    sql.reserve(static_cast<SQLUINTEGER>(ifs.tellg()));
    ifs.seekg(0, std::ios::beg);
    sql.assign((std::istreambuf_iterator<CHAR>(ifs)),
                std::istreambuf_iterator<CHAR>());

    ifs.close();

    return sql;
}

DBDriverList DBConnection::get_drivers(){

    SQLCHAR uch_name[DBC_LEN_STR];
    SQLCHAR uch_desc[DBC_LEN_STR];

    SQLSMALLINT len_name;
    SQLSMALLINT len_desc;

    DBDriverList ret_drivers;
    while(SQLDriversA(
        m_henv, SQL_FETCH_NEXT,
        uch_name, DBC_LEN_STR, &len_name,
        uch_desc, DBC_LEN_STR, &len_desc) != SQL_NO_DATA)
    {
        DBDriver dbdrv;
        dbdrv.name = to_string(uch_name);
        dbdrv.desc = to_string(uch_desc);
        ret_drivers.push_back(dbdrv);
    }

    return ret_drivers;
}
DBSourceList DBConnection::get_sources(){

    SQLCHAR uch_name[DBC_LEN_STR];
    SQLCHAR uch_desc[DBC_LEN_STR];

    SQLSMALLINT len_name;
    SQLSMALLINT len_desc;

    DBSourceList ret_sources;
    while(SQLDataSourcesA(
        m_henv, SQL_FETCH_NEXT,
        uch_name, DBC_LEN_STR, &len_name,
        uch_desc, DBC_LEN_STR, &len_desc) != SQL_NO_DATA)
    {
        DBSource dbsrc;
        dbsrc.name = to_string(uch_name);
        dbsrc.desc = to_string(uch_desc);
        ret_sources.push_back(dbsrc);
    }

    return ret_sources;
}
DBTableSet DBConnection::get_tables(std::string catalog, std::string schema, std::string table, std::string types){
    // catalog = SQL_ALL_CATALOGS, schema = "", name = ""
    // schema = SQL_ALL_SCHEMAS, catalog = "", name = ""
    // tabletype = SQL_ALL_TABLE_TYPES, catalog = "", schema = "", name = ""
    // tabletype = "" or CSV ( 'TABLE', 'VIEW' or TABLE, VIEW)

    DBTableSet ret_tables;

    SQLCHAR *uch_searchCatalog = catalog.empty() ? nullptr : to_uchar(&catalog);
    SQLCHAR *uch_searchSchema = schema.empty() ? nullptr : to_uchar(&schema);
    SQLCHAR *uch_searchTable = table.empty() ? nullptr : to_uchar(&table);
    SQLCHAR *uch_searchType = types.empty() ? nullptr : to_uchar(&types);

    SQLSMALLINT len_searchCatalog = catalog.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchSchema = schema.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchTable = table.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchType = types.empty() ? 0 : SQL_NTS;

    RETCODE rc = SQLTablesA(m_hstmt,
        uch_searchCatalog, len_searchCatalog,
        uch_searchSchema, len_searchSchema,
        uch_searchTable, len_searchTable,
        uch_searchType, len_searchType);

    if(!rc_error(rc, SQL_HANDLE_STMT)){

        SQLCHAR uch_catalog[DBC_LEN_STR];
        SQLCHAR uch_schema[DBC_LEN_STR];
        SQLCHAR uch_name[DBC_LEN_STR];
        SQLCHAR uch_type[DBC_LEN_STR];
        SQLCHAR uch_remarks[DBC_LEN_REM];

        SQLINTEGER len_catalog;
        SQLINTEGER len_schema;
        SQLINTEGER len_name;
        SQLINTEGER len_type;
        SQLINTEGER len_remarks;

        SQLUSMALLINT cidx = 0;
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_catalog, DBC_LEN_STR, &len_catalog);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_schema, DBC_LEN_STR, &len_schema);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_name, DBC_LEN_STR, &len_name);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_type, DBC_LEN_STR, &len_type);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_remarks, DBC_LEN_REM, &len_remarks);

        while(SQLFetch(m_hstmt) == SQL_SUCCESS){

            DBTable dbtbl;

            dbtbl.catalog = to_string(uch_catalog);
            dbtbl.schema = to_string(uch_schema);
            dbtbl.name = to_string(uch_name);
            dbtbl.type = to_string(uch_type);
            dbtbl.remarks = to_string(uch_remarks);

            ret_tables.push_back(dbtbl);
        }
    }

    return ret_tables;
}
DBColumnSet DBConnection::get_columns(std::string catalog, std::string schema, std::string table, std::string column, bool procedure) {
    DBColumnSet ret_columns;

    SQLCHAR *uch_searchCatalog = catalog.empty() ? nullptr : to_uchar(&catalog);
    SQLCHAR *uch_searchSchema = schema.empty() ? nullptr : to_uchar(&schema);
    SQLCHAR *uch_searchTable = table.empty() ? nullptr : to_uchar(&table);
    SQLCHAR *uch_searchColumn = column.empty() ? nullptr : to_uchar(&column);

    SQLSMALLINT len_searchCatalog = catalog.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchSchema = schema.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchTable = table.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchColumn = column.empty() ? 0 : SQL_NTS;

    RETCODE rc;

    if(!procedure){
        rc = SQLColumnsA(m_hstmt,
            uch_searchCatalog, len_searchCatalog,
            uch_searchSchema, len_searchSchema,
            uch_searchTable, len_searchTable,
            uch_searchColumn, len_searchColumn);
    }
    else {
        rc = SQLProcedureColumnsA(m_hstmt,
            uch_searchCatalog, len_searchCatalog,
            uch_searchSchema, len_searchSchema,
            uch_searchTable, len_searchTable,
            uch_searchColumn, len_searchColumn);
    }

    if(!rc_error(rc, SQL_HANDLE_STMT)){

        SQLCHAR uch_catalog[DBC_LEN_STR];
        SQLCHAR uch_schema[DBC_LEN_STR];
        SQLCHAR uch_labelName[DBC_LEN_STR];
        SQLCHAR uch_columnName[DBC_LEN_STR];
        SQLSMALLINT val_columnType = -1;
        SQLSMALLINT val_dataType;
        SQLCHAR uch_typeName[DBC_LEN_STR];
        SQLINTEGER val_columnSize;
        SQLINTEGER val_bufferLength;
        SQLSMALLINT val_decimalDigits;
        SQLSMALLINT val_numPrecRadix;
        SQLSMALLINT val_nullable;
        SQLCHAR uch_remarks[DBC_LEN_REM];
        SQLCHAR uch_columnDefault[DBC_LEN_STR];
        SQLSMALLINT val_dataTypeSQL;
        SQLSMALLINT val_datetimeSubtypeCode;
        SQLINTEGER val_charOctetLength;
        SQLINTEGER val_ordinalPosition;
        SQLCHAR uch_isNullable[DBC_LEN_STR];

        SQLINTEGER len_catalog;
        SQLINTEGER len_schema;
        SQLINTEGER len_labelName;
        SQLINTEGER len_columnName;
        SQLINTEGER len_columnType;
        SQLINTEGER len_dataType;
        SQLINTEGER len_typeName;
        SQLINTEGER len_columnSize;
        SQLLEN len_bufferLength;
        SQLINTEGER len_decimalDigits;
        SQLINTEGER len_numPrecRadix;
        SQLINTEGER len_nullable;
        SQLINTEGER len_remarks;
        SQLINTEGER len_columnDefault;
        SQLINTEGER len_dataTypeSQL;
        SQLINTEGER len_datetimeSubtypeCode;
        SQLINTEGER len_charOctetLength;
        SQLINTEGER len_ordinalPosition;
        SQLINTEGER len_isNullable;

        SQLUSMALLINT cidx = 0;
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_catalog, DBC_LEN_STR, &len_catalog);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_schema, DBC_LEN_STR, &len_schema);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_labelName, DBC_LEN_STR, &len_labelName);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_columnName, DBC_LEN_STR, &len_columnName);
        if(procedure){SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_columnType, 0, &len_columnType);}
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_dataType, 0, &len_dataType);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_typeName, DBC_LEN_STR, &len_typeName);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SLONG, &val_columnSize, 0, &len_columnSize);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SLONG, &val_bufferLength, 0, &len_bufferLength);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_decimalDigits, 0, &len_decimalDigits);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_numPrecRadix, 0, &len_numPrecRadix);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_nullable, 0, &len_nullable);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_remarks, DBC_LEN_REM, &len_remarks);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_columnDefault, DBC_LEN_STR, &len_columnDefault);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_dataTypeSQL, 0, &len_dataTypeSQL);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SSHORT, &val_datetimeSubtypeCode, 0, &len_datetimeSubtypeCode);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SLONG, &val_charOctetLength, 0, &len_charOctetLength);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_SLONG, &val_ordinalPosition, 0, &len_ordinalPosition);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_isNullable, DBC_LEN_STR, &len_isNullable);

        while(SQLFetch(m_hstmt) == SQL_SUCCESS){

            DBColumn dbcol;
//std::string(err_msg, err_msg + (sizeof(err_msg) / sizeof(err_msg[0]))).c_str();

            dbcol.catalog = to_string(uch_catalog);
            dbcol.schema = to_string(uch_schema);
            dbcol.table = to_string(uch_labelName);
            dbcol.name = to_string(uch_columnName);
            dbcol.datatypeCType = val_dataType;
            dbcol.datatypeName = to_string(uch_typeName);
            dbcol.size = val_columnSize;
            dbcol.bufferLength = val_bufferLength;
            dbcol.decimalDigits = val_decimalDigits;
            dbcol.numPrecRadix = val_numPrecRadix;
            dbcol.nullableCVal = val_nullable;
            dbcol.remarks = to_string(uch_remarks);
            dbcol.defaultValue = to_string(uch_columnDefault);
            dbcol.datatypeSQL = val_dataTypeSQL;
            dbcol.datetimeSubtypeCode = val_datetimeSubtypeCode;
            dbcol.charOctectLength = val_charOctetLength;
            dbcol.index = val_ordinalPosition;
            dbcol.isNullable = to_string(uch_isNullable);
            dbcol.procedureParamType = val_columnType;
            dbcol.procedureColumn = procedure;

            ret_columns.push_back(dbcol);
        }
    }

    return ret_columns;
}
DBProcedureList DBConnection::get_procedures(std::string catalog, std::string schema, std::string proc){

    DBProcedureList ret_procedures;

    SQLCHAR *uch_searchCatalog = catalog.empty() ? nullptr : to_uchar(&catalog);
    SQLCHAR *uch_searchSchema = schema.empty() ? nullptr : to_uchar(&schema);
    SQLCHAR *uch_searchProc = proc.empty() ? nullptr : to_uchar(&proc);

    SQLSMALLINT len_searchCatalog = catalog.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchSchema = schema.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchProc = proc.empty() ? 0 : SQL_NTS;

    RETCODE rc = SQLProceduresA(m_hstmt,
        uch_searchCatalog, len_searchCatalog,
        uch_searchSchema, len_searchSchema,
        uch_searchProc, len_searchProc);

    if(!rc_error(rc, SQL_HANDLE_STMT)){

        SQLCHAR uch_catalog[DBC_LEN_STR];
        SQLCHAR uch_schema[DBC_LEN_STR];
        SQLCHAR uch_name[DBC_LEN_STR];
        SQLCHAR uch_inParams[DBC_LEN_STR];
        SQLCHAR uch_outParams[DBC_LEN_STR];
        SQLCHAR uch_results[DBC_LEN_STR];
        SQLCHAR uch_remarks[DBC_LEN_REM];
        SQLSMALLINT val_type;

        SQLINTEGER len_catalog;
        SQLINTEGER len_schema;
        SQLINTEGER len_name;
        SQLINTEGER len_inParams;
        SQLINTEGER len_outParams;
        SQLINTEGER len_results;
        SQLINTEGER len_remarks;
        SQLINTEGER len_type;

        SQLBindCol(m_hstmt, 1, SQL_C_CHAR, uch_catalog, DBC_LEN_STR, &len_catalog);
        SQLBindCol(m_hstmt, 2, SQL_C_CHAR, uch_schema, DBC_LEN_STR, &len_schema);
        SQLBindCol(m_hstmt, 3, SQL_C_CHAR, uch_name, DBC_LEN_STR, &len_name);
        SQLBindCol(m_hstmt, 4, SQL_C_CHAR, uch_inParams, DBC_LEN_STR, &len_inParams);
        SQLBindCol(m_hstmt, 5, SQL_C_CHAR, uch_outParams, DBC_LEN_STR, &len_outParams);
        SQLBindCol(m_hstmt, 6, SQL_C_CHAR, uch_results, DBC_LEN_STR, &len_results);
        SQLBindCol(m_hstmt, 7, SQL_C_CHAR, uch_remarks, DBC_LEN_REM, &len_remarks);
        SQLBindCol(m_hstmt, 8, SQL_C_SSHORT, &val_type, 0, &len_type);

        while(SQLFetch(m_hstmt) != SQL_NO_DATA){

//            std::cout << szRemarks << std::endl;
//            std::cout << cbRemarks << std::endl;
//            std::cout << std::endl;

            DBProcedure dbproc;

            dbproc.catalog = to_string(uch_catalog);
            dbproc.schema = to_string(uch_schema);
            dbproc.name = to_string(uch_name);
            dbproc.numInParams = to_string(uch_inParams);
            dbproc.numOutParams = to_string(uch_outParams);
            dbproc.numResultSets = to_string(uch_results);
            dbproc.remarks = to_string(uch_remarks);
            dbproc.type = val_type;

            ret_procedures.push_back(dbproc);
        }
    }

    return ret_procedures;
}
DBPrivilegeList DBConnection::get_privileges(std::string catalog, std::string schema, std::string table, std::string column, bool getTablePrivileges){

    DBPrivilegeList ret_privileges;

    SQLCHAR *uch_searchCatalog = catalog.empty() ? nullptr : to_uchar(&catalog);
    SQLCHAR *uch_searchSchema = schema.empty() ? nullptr : to_uchar(&schema);
    SQLCHAR *uch_searchTable = table.empty() ? nullptr : to_uchar(&table);
    SQLCHAR *uch_searchColumn = column.empty() ? nullptr : to_uchar(&column);

    SQLSMALLINT len_searchCatalog = catalog.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchSchema = schema.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchTable = table.empty() ? 0 : SQL_NTS;
    SQLSMALLINT len_searchColumn = column.empty() ? 0 : SQL_NTS;

    RETCODE rc;

    if(getTablePrivileges){
        rc = SQLTablePrivilegesA(m_hstmt,
            uch_searchCatalog, len_searchCatalog,
            uch_searchSchema, len_searchSchema,
            uch_searchTable, len_searchTable);
    }
    else {
        rc = SQLColumnPrivilegesA(m_hstmt,
            uch_searchCatalog, len_searchCatalog,
            uch_searchSchema, len_searchSchema,
            uch_searchTable, len_searchTable,
            uch_searchColumn, len_searchColumn);
    }

    if(!rc_error(rc, SQL_HANDLE_STMT)){

        SQLCHAR uch_catalog[DBC_LEN_STR];
        SQLCHAR uch_schema[DBC_LEN_STR];
        SQLCHAR uch_table[DBC_LEN_STR];
        SQLCHAR uch_column[DBC_LEN_STR];
        SQLCHAR uch_grantor[DBC_LEN_STR];
        SQLCHAR uch_grantee[DBC_LEN_STR];
        SQLCHAR uch_privilege[DBC_LEN_STR];
        SQLCHAR uch_isGrantable[DBC_LEN_STR];

        SQLINTEGER len_catalog;
        SQLINTEGER len_schema;
        SQLINTEGER len_table;
        SQLINTEGER len_column;
        SQLINTEGER len_grantor;
        SQLINTEGER len_grantee;
        SQLINTEGER len_privilege;
        SQLINTEGER len_isGrantable;

        SQLUSMALLINT cidx = 0;
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_catalog, DBC_LEN_STR, &len_catalog);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_schema, DBC_LEN_STR, &len_schema);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_table, DBC_LEN_STR, &len_table);
        if(!getTablePrivileges){SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_column, 0, &len_column);}
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_grantor, DBC_LEN_STR, &len_grantor);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_grantee, DBC_LEN_STR, &len_grantee);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_privilege, DBC_LEN_STR, &len_privilege);
        SQLBindCol(m_hstmt, ++cidx, SQL_C_CHAR, uch_isGrantable, DBC_LEN_STR, &len_isGrantable);

        while(SQLFetch(m_hstmt) == SQL_SUCCESS){

            DBPrivilege dbpriv;

            dbpriv.catalog = to_string(uch_catalog);
            dbpriv.schema = to_string(uch_schema);
            dbpriv.table = to_string(uch_table);
            dbpriv.column = getTablePrivileges ? "" : to_string(uch_column);
            dbpriv.grantor = to_string(uch_grantor);
            dbpriv.grantee = to_string(uch_grantee);
            dbpriv.privilege = to_string(uch_privilege);
            dbpriv.isGrantable = to_string(uch_isGrantable);

            ret_privileges.push_back(dbpriv);
        }
    }

    return ret_privileges;
}
DBPrivilegeList DBConnection::get_column_privileges(std::string catalog, std::string schema, std::string table, std::string column){
    return get_privileges(catalog, schema, table, column, false);
}
DBPrivilegeList DBConnection::get_table_privileges(std::string catalog, std::string schema, std::string table){
    return get_privileges(catalog, schema, table, "", true);
}

void DBConnection::initialize(){

    if(!m_initialized){

        RETCODE rc;

        rc = SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &m_henv);
        if(rc_error(rc, SQL_HANDLE_ENV)){return;}

        SQLPOINTER odbc_version = reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3);
        rc = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, odbc_version, 0);
        if(rc_error(rc, SQL_HANDLE_ENV)){return;}

        rc = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
        if(rc_error(rc, SQL_HANDLE_DBC)){return;}

        SQLUINTEGER timeout_login = 60;
        rc = SQLSetConnectAttr(m_hdbc, SQL_ATTR_LOGIN_TIMEOUT, &timeout_login, 0);
        if(rc_error(rc, SQL_HANDLE_DBC)){return;}

        SQLUINTEGER timeout_connection = 0;
        rc = SQLSetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_TIMEOUT, &timeout_connection, 0);
        if(rc_error(rc, SQL_HANDLE_DBC)){return;}

        SQLPOINTER trace_opt = reinterpret_cast<SQLPOINTER>(SQL_OPT_TRACE_OFF);
        rc = SQLSetConnectAttr(m_hdbc, SQL_ATTR_TRACE, trace_opt, 0);
        if(rc_error(rc, SQL_HANDLE_DBC)){return;}

        m_initialized = true;
    }
}
void DBConnection::connect(DBConnString dbcns){

    if(!m_connected){

        std::string cns = dbcns.connection_string;

        SQLSMALLINT len_cns_in = static_cast<SQLSMALLINT>(cns.size());
        SQLCHAR *uch_cns_in = DBConnection::to_uchar(&cns);
        SQLCHAR uch_cns_out[DBC_LEN_REM];

        SQLSMALLINT buf_cns_out;
        SQLUSMALLINT cflag = SQL_DRIVER_NOPROMPT;
        HWND hwnd_desktop = GetDesktopWindow();

        RETCODE rc;
        rc = SQLDriverConnectA(m_hdbc,
            hwnd_desktop,
            uch_cns_in, len_cns_in,
            uch_cns_out, DBC_LEN_REM,
            &buf_cns_out, cflag);
        if(rc_error(rc, SQL_HANDLE_DBC)){return;}

        rc = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
        if(rc_error(rc, SQL_HANDLE_STMT)){return;}

        SQLPOINTER no_scan = reinterpret_cast<SQLPOINTER>(SQL_NOSCAN_OFF);
        rc = SQLSetStmtAttr(m_hstmt, SQL_ATTR_NOSCAN, no_scan, 0);
        if(rc_error(rc, SQL_HANDLE_STMT)){return;}

        SQLUINTEGER timeout_query = 0;
        rc = SQLSetStmtAttr(m_hstmt, SQL_ATTR_QUERY_TIMEOUT, &timeout_query, 0);
        if(rc_error(rc, SQL_HANDLE_STMT)){return;}

        SQLULEN max_len = 0;
        rc = SQLSetStmtAttr(m_hstmt, SQL_ATTR_MAX_LENGTH, &max_len, 0);
        if(rc_error(rc, SQL_HANDLE_STMT)){return;}

        m_connected = true;
    }
}
void DBConnection::disconnect(){

    if(m_connected){

        RETCODE rc;

        if(m_hstmt){
            rc = SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
            if(rc_error(rc, SQL_HANDLE_STMT)){return;}

             m_hstmt = nullptr;
        }

        if(m_hdbc){
            rc = SQLDisconnect(m_hdbc);
            if(rc_error(rc, SQL_HANDLE_DBC)){return;}

            rc = SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
            if(rc_error(rc, SQL_HANDLE_DBC)){return;}

            m_hdbc = nullptr;
        }

        if(m_henv){
            rc = SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
            if(rc_error(rc, SQL_HANDLE_ENV)){return;}

            m_henv = nullptr;
        }

        m_initialized = false;
        m_connected = false;
    }
}

void DBConnection::prepare(std::string qry){

    SQLCHAR *uch_qry = DBConnection::to_uchar(&qry);
    SQLINTEGER len_qry = static_cast<SQLINTEGER>(qry.size());

    RETCODE rc = SQLPrepareA(m_hstmt, uch_qry, len_qry);
    if(rc_error(rc, SQL_HANDLE_STMT)){return;}
}
void DBConnection::execute(std::string qry){

    SQLCHAR *uch_qry = DBConnection::to_uchar(&qry);
    RETCODE rc = SQLExecDirectA(m_hstmt, uch_qry, SQL_NTS);
    if(rc_error(rc, SQL_HANDLE_STMT)){return;}
}
void DBConnection::execute(){

    RETCODE rc = SQLExecute(m_hstmt);
    if(rc_error(rc, SQL_HANDLE_STMT)){return;}
}

void DBConnection::bind_param(SQLUSMALLINT index, DBParam param){

    RETCODE rc = SQLBindParameter(
        m_hstmt, index,
        param.iotype,
        param.valtype,
        param.sqltype,
        param.colsize,
        param.decimals,
        param.paramval,
        param.buflen,
        &param.strlen);
    if(rc_error(rc, SQL_HANDLE_STMT)){return;}
}

DBRecordSet DBConnection::fetch_all(){
    return fetch_many(-1);
}
DBRecordSet DBConnection::fetch_many(SQLINTEGER limit){

    DBRecordSet rows;

    SQLINTEGER count = limit < 0 ? LONG_MAX : limit;
    for(SQLINTEGER i = 0; i < count; i++){
        DBRecord row = fetch();

        if(row.index < 0){
            break;
        }

        rows.push_back(row);
    }

    return rows;
}
DBRecord DBConnection::fetch_one(){
    return fetch();
}
DBRecord DBConnection::fetch(){

    DBRecord row;
    row.index = -1;

    if(SQLFetch(m_hstmt) == SQL_NO_DATA){return row;}

    row.index = 1;
    for(SQLUSMALLINT col = 1; col < colcount() + 1; col++){
        SQLCHAR uch_data[DBC_LEN_STR];
        SQLINTEGER len_data;

        SQLGetData(m_hstmt, col, SQL_C_CHAR, uch_data, sizeof(uch_data), &len_data);

        DBField field;
        field.index = col;
        field.datatype = SQL_C_CHAR;
        field.value = std::string(uch_data, uch_data + (sizeof(uch_data) / sizeof uch_data[0])).c_str();

        row.fields.push_back(field);
    }

    return row;
}

void DBConnection::end(){
    RETCODE rc = SQLCloseCursor(m_hstmt);
    if(rc_error(rc, SQL_HANDLE_STMT)){return;}
}

bool DBConnection::initialized() const{
    return m_initialized;
}
bool DBConnection::connected() const{
    return m_connected;
}

SQLINTEGER DBConnection::rowcount() const{
    SQLINTEGER row_count;
    SQLRowCount(m_hstmt, &row_count);
    return row_count;
}
SQLSMALLINT DBConnection::colcount() const{
    SQLSMALLINT col_count;
    SQLNumResultCols(m_hstmt, &col_count);
    return col_count;
}

DBException DBConnection::error_class(std::string sqlstate){

    std::string cls_code = sqlstate.substr(0, 2);

    if(cls_code == "22"){
        return DataError();
    }
    else if(cls_code == "23"){
        return IntegrityError();
    }
    else if(cls_code == "24" || cls_code == "25" || cls_code == "26"){
        return InvalidStateError();
    }
    else if(cls_code == "42"){
        return SyntaxError();
    }

    return DatabaseError();
}

bool DBConnection::rc_error(RETCODE rc, SQLSMALLINT ht){

    bool error = !(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO || rc == SQL_NO_DATA);

    if(error){

        if(rc == SQL_INVALID_HANDLE){
            throw InvalidStateError();
        }

        SQLHANDLE hnd = nullptr;

        if(ht == SQL_HANDLE_STMT){
            hnd = m_hstmt;
        }
        else if(ht == SQL_HANDLE_ENV){
            hnd = m_henv;
        }
        else if(ht == SQL_HANDLE_DBC){
            hnd = m_hdbc;
        }
        else {
            throw InternalError();
        }

        SQLCHAR err_state[SQL_SQLSTATE_SIZE + 1];
        SQLCHAR err_msg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER err_num;
        SQLSMALLINT len_text = SQL_NTS;

        SQLGetDiagRecA(ht, hnd, 1,
            err_state, &err_num, err_msg,
            SQL_MAX_MESSAGE_LENGTH, &len_text);

        std::string sqlstate = std::string(err_state, err_state + (sizeof(err_state) / sizeof(err_state[0]))).c_str();
        std::string sqlmessage = std::string(err_msg, err_msg + (sizeof(err_msg) / sizeof(err_msg[0]))).c_str();

        DBException dberr = error_class(sqlstate);
        dberr.errnum = err_num;
        dberr.sqlstate = sqlstate;
        dberr.sqlmessage = sqlmessage;
        throw dberr;
    }

    return error;
}

