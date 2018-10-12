#include "dberrors.h"

#include <sstream>
#include <chrono>


DBException::DBException() {
    m_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void DBException::set(long errnum, std::string sqlstate, std::string sqlmessage){
    m_errnum = errnum;
    m_sqlstate = sqlstate;
    m_sqlmessage = sqlmessage;
}
std::string DBException::msg(){
    std::stringstream ss;
    ss << "\n--------------------\n";
    ss << "SQLERROR: " << m_errnum << "\n";
    ss << "SQLSTATE: " << m_sqlstate << "\n";
    ss << "REASON: " << get_reason(m_sqlstate) << "\n";
    ss << "MESSAGE: \n" << m_sqlmessage;
    ss << "\n--------------------\n";
    return ss.str();
}

DBException DBException::from_state(std::string sqlstate){

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
std::string DBException::get_reason(std::string sqlstate){

    const std::map<std::string, std::string>::const_iterator iter = SQLStateMessages.find(sqlstate);

    if(iter != SQLStateMessages.end()){
       return iter->second;
    }

    return "SQLSTATE UNKOWN";
}

const std::map<std::string, std::string> DBException::SQLStateMessages = {
    {"01000", "General warning"},
    {"01001", "Cursor operation conflict"},
    {"01002", "Disconnect error"},
    {"01003", "NULL value eliminated in set function"},
    {"01004", "String data, right-truncated"},
    {"01006", "Privilege not revoked"},
    {"01007", "Privilege not granted"},
    {"01S00", "Invalid connection string attribute"},
    {"01S01", "Error in row"},
    {"01S02", "Option value changed"},
    {"01S06", "Attempt to fetch before the result set returned the first rowset"},
    {"01S07", "Fractional truncation"},
    {"01S08", "Error saving File DSN"},
    {"01S09", "Invalid keyword"},
    {"07001", "Wrong number of parameters"},
    {"07002", "COUNT field incorrect"},
    {"07005", "Prepared statement not a cursor-specification"},
    {"07006", "Restricted data type attribute violation"},
    {"07009", "Invalid descriptor index"},
    {"07S01", "Invalid use of default parameter"},
    {"08001", "Client unable to establish connection"},
    {"08002", "Connection name in use"},
    {"08003", "Connection not open"},
    {"08004", "Server rejected the connection"},
    {"08007", "Connection failure during transaction"},
    {"08S01", "Communication link failure"},
    {"21S01", "Insert value list does not match column list"},
    {"21S02", "Degree of derived table does not match column list"},
    {"22001", "String data, right-truncated"},
    {"22002", "Indicator variable required but not supplied"},
    {"22003", "Numeric value out of range"},
    {"22007", "Invalid datetime format"},
    {"22008", "Datetime field overflow"},
    {"22012", "Division by zero"},
    {"22015", "Interval field overflow"},
    {"22018", "Invalid character value for cast specification"},
    {"22019", "Invalid escape character"},
    {"22025", "Invalid escape sequence"},
    {"22026", "String data, length mismatch"},
    {"23000", "Integrity constraint violation"},
    {"24000", "Invalid cursor state"},
    {"25000", "Invalid transaction state"},
    {"25S01", "Transaction state"},
    {"25S02", "Transaction is still active"},
    {"25S03", "Transaction is rolled back"},
    {"28000", "Invalid authorization specification"},
    {"34000", "Invalid cursor name"},
    {"3C000", "Duplicate cursor name"},
    {"3D000", "Invalid catalog name"},
    {"3F000", "Invalid schema name"},
    {"40001", "Serialization failure"},
    {"40002", "Integrity constraint violation"},
    {"40003", "Statement completion unknown"},
    {"42000", "Syntax error or access violation"},
    {"42S01", "Base table or view already exists"},
    {"42S02", "Base table or view not found"},
    {"42S11", "Index already exists"},
    {"42S12", "Index not found"},
    {"42S21", "Column already exists"},
    {"42S22", "Column not found"},
    {"44000", "WITH CHECK OPTION violation"},
    {"HY000", "General error"},
    {"HY001", "Memory allocation error"},
    {"HY003", "Invalid application buffer type"},
    {"HY004", "Invalid SQL data type"},
    {"HY007", "Associated statement is not prepared"},
    {"HY008", "Operation canceled"},
    {"HY009", "Invalid use of null pointer"},
    {"HY010", "Function sequence error"},
    {"HY011", "Attribute cannot be set now"},
    {"HY012", "Invalid transaction operation code"},
    {"HY013", "Memory management error"},
    {"HY014", "Limit on the number of handles exceeded"},
    {"HY015", "No cursor name available"},
    {"HY016", "Cannot modify an implementation row descriptor"},
    {"HY017", "Invalid use of an automatically allocated descriptor handle"},
    {"HY018", "Server declined cancel request"},
    {"HY019", "Non-character and non-binary data sent in pieces"},
    {"HY020", "Attempt to concatenate a null value"},
    {"HY021", "Inconsistent descriptor information"},
    {"HY024", "Invalid attribute value"},
    {"HY090", "Invalid string or buffer length"},
    {"HY091", "Invalid descriptor field identifier"},
    {"HY092", "Invalid attribute/option identifier"},
    {"HY095", "Function type out of range"},
    {"HY096", "Invalid information type"},
    {"HY097", "Column type out of range"},
    {"HY098", "Scope type out of range"},
    {"HY099", "Nullable type out of range"},
    {"HY100", "Uniqueness option type out of range"},
    {"HY101", "Accuracy option type out of range"},
    {"HY103", "Invalid retrieval code"},
    {"HY104", "Invalid precision or scale value"},
    {"HY105", "Invalid parameter type"},
    {"HY106", "Fetch type out of range"},
    {"HY107", "Row value out of range"},
    {"HY109", "Invalid cursor position"},
    {"HY110", "Invalid driver completion"},
    {"HY111", "Invalid bookmark value"},
    {"HYC00", "Optional feature not implemented"},
    {"HYT00", "Timeout expired"},
    {"HYT01", "Connection timeout expired"},
    {"IM001", "Driver does not support this function"},
    {"IM002", "Data source name not found and no default driver specified"},
    {"IM003", "Specified driver could not be loaded"},
    {"IM004", "Driver's SQLAllocHandle on SQL_HANDLE_ENV failed"},
    {"IM005", "Driver's SQLAllocHandle on SQL_HANDLE_DBC failed"},
    {"IM006", "Driver's SQLSetConnectAttr failed"},
    {"IM007", "No data source or driver specified; dialog prohibited"},
    {"IM008", "Dialog failed"},
    {"IM009", "Unable to load translation DLL"},
    {"IM010", "Data source name too long"},
    {"IM011", "Driver name too long"},
    {"IM012", "DRIVER keyword syntax error"},
    {"IM013", "Trace file error"},
    {"IM014", "Invalid name of File DSN"},
    {"IM015", "Corrupt file data source"},
};
const std::map<std::string, std::string> DBException::SQLStateClasses = {
    {"00", "Unqualified Successful Completion"},
    {"01", "Warning"},
    {"02", "No Data"},
    {"07", "Dynamic SQL Error"},
    {"08", "Connection Exception"},
    {"09", "Triggered Action Exception"},
    {"0A", "Feature Not Supported"},
    {"0E", "Invalid Schema Name List Specification"},
    {"0F", "Invalid Token"},
    {"0K", "Resignal When Handler Not Active"},
    {"0N", "SQL/XML Mapping Error"},
    {"0W", "Prohibited Statement Encountered During Trigger"},
    {"0Z", "Diagnostics Exception"},
    {"10", "XQuery Error"},
    {"20", "Case Not Found for Case Statement"},
    {"21", "Cardinality Violation"},
    {"22", "Data Exception"},
    {"23", "Constraint Violation"},
    {"24", "Invalid Cursor State"},
    {"25", "Invalid Transaction State"},
    {"26", "Invalid SQL Statement Identifier"},
    {"27", "Triggered Data Change Violation"},
    {"28", "Invalid Authorization Specification"},
    {"2D", "Invalid Transaction Termination"},
    {"2E", "Invalid Connection Name"},
    {"2F", "SQL Function Exception"},
    {"33", "Invalid SQL Descriptor Name"},
    {"34", "Invalid Cursor Name"},
    {"35", "Invalid Condition Number"},
    {"36", "Cursor Sensitivity Exception"},
    {"38", "External Function Exception"},
    {"39", "External Function Call Exception"},
    {"3B", "Savepoint Exception"},
    {"3C", "Ambiguous Cursor Name"},
    {"3F", "Invalid Schema Name"},
    {"42", "Syntax Error or Access Rule Violation"},
    {"44", "WITH CHECK OPTION Violation"},
    {"46", "Java Errors"},
    {"51", "Invalid Application State"},
    {"53", "Invalid Operand or Inconsistent Specification"},
    {"54", "SQL or Product Limit Exceeded"},
    {"55", "Object Not in Prerequisite State"},
    {"56", "Miscellaneous SQL or Product Error"},
    {"57", "Resource Not Available or Operator Intervention"},
    {"58", "System Error"},
};
