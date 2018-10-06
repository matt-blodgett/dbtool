#include "dbtool.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


#define DBC_MSG_BUFF (1 << 12)


#define CHARBUFF 1

#define LEN_PRO 12
#define LEN_OTN 5
#define LEN_ORG 7 + CHARBUFF
#define LEN_LVD 10
#define LEN_LVT 6
#define LEN_DTN 5
#define LEN_DST 7 + CHARBUFF
#define LEN_ARD 10
#define LEN_ART 6
#define LEN_EDA 10
#define LEN_ETA 6
#define LEN_STS 3 + CHARBUFF
#define LEN_SYS 3 + CHARBUFF


#define LEN_DATA 30
#define BUF_CONNSTR 255


// cp -Path "C:\dev\dbtool\build-dbtool-Desktop_Qt_5_11_1_MinGW_32bit-Debug\debug\dbtool.dll" -Destination "C:\dev\dbtool\build-dbtest-Desktop_Qt_5_11_1_MinGW_32bit-Debug\debug\dbtool.dll"


static const std::map<SQLSMALLINT, std::wstring> InfoTypes = {
    {SQL_ACCESSIBLE_PROCEDURES, L"SQL_ACCESSIBLE_PROCEDURES"},
    {SQL_ACCESSIBLE_TABLES, L"SQL_ACCESSIBLE_TABLES"},
    {SQL_ACTIVE_CONNECTIONS, L"SQL_ACTIVE_CONNECTIONS"},
    {SQL_ACTIVE_ENVIRONMENTS, L"SQL_ACTIVE_ENVIRONMENTS"},
    {SQL_ACTIVE_STATEMENTS, L"SQL_ACTIVE_STATEMENTS"},
    {SQL_ALTER_DOMAIN, L"SQL_ALTER_DOMAIN"},
    {SQL_ALTER_TABLE, L"SQL_ALTER_TABLE"},
    {SQL_AT_ADD_COLUMN, L"SQL_AT_ADD_COLUMN"},
    {SQL_AT_ADD_COLUMN_DEFAULT, L"SQL_AT_ADD_COLUMN_DEFAULT"},
    {SQL_AT_ADD_COLUMN_SINGLE, L"SQL_AT_ADD_COLUMN_SINGLE"},
    {SQL_AT_ADD_CONSTRAINT, L"SQL_AT_ADD_CONSTRAINT"},
    {SQL_AT_ADD_TABLE_CONSTRAINT, L"SQL_AT_ADD_TABLE_CONSTRAINT"},
    {SQL_AT_CONSTRAINT_NAME_DEFINITION, L"SQL_AT_CONSTRAINT_NAME_DEFINITION"},
    {SQL_AT_DROP_COLUMN_RESTRICT, L"SQL_AT_DROP_COLUMN_RESTRICT"},
    {SQL_SQL_CONFORMANCE, L"SQL_SQL_CONFORMANCE"},
    {SQL_SC_SQL92_ENTRY, L"SQL_SC_SQL92_ENTRY"},
    {SQL_DATETIME_LITERALS, L"SQL_DATETIME_LITERALS"},
    {SQL_ASYNC_MODE, L"SQL_ASYNC_MODE"},
    {SQL_AM_STATEMENT, L"SQL_AM_STATEMENT"},
    {SQL_BATCH_ROW_COUNT, L"SQL_BATCH_ROW_COUNT"},
    {SQL_BRC_EXPLICIT, L"SQL_BRC_EXPLICIT"},
    {SQL_BATCH_SUPPORT, L"SQL_BATCH_SUPPORT"},
    {SQL_BS_ROW_COUNT_EXPLICIT, L"SQL_BS_ROW_COUNT_EXPLICIT"},
    {SQL_BS_ROW_COUNT_PROC, L"SQL_BS_ROW_COUNT_PROC"},
    {SQL_BS_SELECT_EXPLICIT, L"SQL_BS_SELECT_EXPLICIT"},
    {SQL_BS_SELECT_PROC, L"SQL_BS_SELECT_PROC"},
    {SQL_BOOKMARK_PERSISTENCE, L"SQL_BOOKMARK_PERSISTENCE"},
    {SQL_BP_DELETE, L"SQL_BP_DELETE"},
    {SQL_BP_SCROLL, L"SQL_BP_SCROLL"},
    {SQL_BP_UPDATE, L"SQL_BP_UPDATE"},
    {SQL_CATALOG_LOCATION, L"SQL_CATALOG_LOCATION"},
    {SQL_CL_START, L"SQL_CL_START"},
    {SQL_CATALOG_NAME, L"SQL_CATALOG_NAME"},
    {SQL_CATALOG_NAME_SEPARATOR, L"SQL_CATALOG_NAME_SEPARATOR"},
    {SQL_CATALOG_TERM, L"SQL_CATALOG_TERM"},
    {SQL_CATALOG_USAGE, L"SQL_CATALOG_USAGE"},
    {SQL_CU_DML_STATEMENTS, L"SQL_CU_DML_STATEMENTS"},
    {SQL_CU_PROCEDURE_INVOCATION, L"SQL_CU_PROCEDURE_INVOCATION"},
    {SQL_CU_TABLE_DEFINITION, L"SQL_CU_TABLE_DEFINITION"},
    {SQL_COLLATION_SEQ, L"SQL_COLLATION_SEQ"},
    {SQL_COLUMN_ALIAS, L"SQL_COLUMN_ALIAS"},
    {SQL_CONCAT_NULL_BEHAVIOR, L"SQL_CONCAT_NULL_BEHAVIOR"},
    {SQL_CB_NULL, L"SQL_CB_NULL"},
    {SQL_CONVERT_BIGINT, L"SQL_CONVERT_BIGINT"},
    {SQL_BIGINT, L"SQL_BIGINT"},
    {SQL_DECIMAL, L"SQL_DECIMAL"},
    {SQL_CONVERT_DECIMAL, L"SQL_CONVERT_DECIMAL"},
    {SQL_CONVERT_BINARY, L"SQL_CONVERT_BINARY"},
    {SQL_CVT_CHAR, L"SQL_CVT_CHAR"},
    {SQL_CVT_NUMERIC, L"SQL_CVT_NUMERIC"},
    {SQL_CVT_DECIMAL, L"SQL_CVT_DECIMAL"},
    {SQL_CVT_INTEGER, L"SQL_CVT_INTEGER"},
    {SQL_CVT_SMALLINT, L"SQL_CVT_SMALLINT"},
    {SQL_CVT_VARCHAR, L"SQL_CVT_VARCHAR"},
    {SQL_CVT_BINARY, L"SQL_CVT_BINARY"},
    {SQL_CVT_VARBINARY, L"SQL_CVT_VARBINARY"},
    {SQL_CVT_TINYINT, L"SQL_CVT_TINYINT"},
    {SQL_CVT_LONGVARBINARY, L"SQL_CVT_LONGVARBINARY"},
    {SQL_CVT_WCHAR, L"SQL_CVT_WCHAR"},
    {SQL_CVT_WVARCHAR, L"SQL_CVT_WVARCHAR"},
    {SQL_CONVERT_BIT, L"SQL_CONVERT_BIT"},
    {SQL_CVT_FLOAT, L"SQL_CVT_FLOAT"},
    {SQL_CVT_REAL, L"SQL_CVT_REAL"},
    {SQL_CVT_BIT, L"SQL_CVT_BIT"},
    {SQL_CONVERT_CHAR, L"SQL_CONVERT_CHAR"},
    {SQL_CVT_LONGVARCHAR, L"SQL_CVT_LONGVARCHAR"},
    {SQL_CVT_TIMESTAMP, L"SQL_CVT_TIMESTAMP"},
    {SQL_CVT_WLONGVARCHAR, L"SQL_CVT_WLONGVARCHAR"},
    {SQL_CONVERT_DATE, L"SQL_CONVERT_DATE"},
    {SQL_TYPE_DATE, L"SQL_TYPE_DATE"},
    {SQL_TYPE_TIMESTAMP, L"SQL_TYPE_TIMESTAMP"},
    {SQL_CONVERT_TIMESTAMP, L"SQL_CONVERT_TIMESTAMP"},
    {SQL_CONVERT_DOUBLE, L"SQL_CONVERT_DOUBLE"},
    {SQL_DOUBLE, L"SQL_DOUBLE"},
    {SQL_FLOAT, L"SQL_FLOAT"},
    {SQL_CONVERT_FLOAT, L"SQL_CONVERT_FLOAT"},
    {SQL_CONVERT_FUNCTIONS, L"SQL_CONVERT_FUNCTIONS"},
    {SQL_FN_CVT_CONVERT, L"SQL_FN_CVT_CONVERT"},
    {SQL_FN_CVT_CAST, L"SQL_FN_CVT_CAST"},
    {SQL_CONVERT_INTEGER, L"SQL_CONVERT_INTEGER"},
    {SQL_CONVERT_INTERVAL_YEAR_MONTH, L"SQL_CONVERT_INTERVAL_YEAR_MONTH"},
    {SQL_CONVERT_INTERVAL_DAY_TIME, L"SQL_CONVERT_INTERVAL_DAY_TIME"},
    {SQL_CONVERT_LONGVARBINARY, L"SQL_CONVERT_LONGVARBINARY"},
    {SQL_CONVERT_LONGVARCHAR, L"SQL_CONVERT_LONGVARCHAR"},
    {SQL_CONVERT_NUMERIC, L"SQL_CONVERT_NUMERIC"},
    {SQL_CONVERT_REAL, L"SQL_CONVERT_REAL"},
    {SQL_CONVERT_SMALLINT, L"SQL_CONVERT_SMALLINT"},
    {SQL_CONVERT_TIME, L"SQL_CONVERT_TIME"},
    {SQL_TYPE_TIME, L"SQL_TYPE_TIME"},
    {SQL_CONVERT_TINYINT, L"SQL_CONVERT_TINYINT"},
    {SQL_CONVERT_VARBINARY, L"SQL_CONVERT_VARBINARY"},
    {SQL_CONVERT_VARCHAR, L"SQL_CONVERT_VARCHAR"},
    {SQL_CONVERT_WCHAR, L"SQL_CONVERT_WCHAR"},
    {SQL_CONVERT_WLONGVARCHAR, L"SQL_CONVERT_WLONGVARCHAR"},
    {SQL_CONVERT_WVARCHAR, L"SQL_CONVERT_WVARCHAR"},
    {SQL_CORRELATION_NAME, L"SQL_CORRELATION_NAME"},
    {SQL_CN_ANY, L"SQL_CN_ANY"},
    {SQL_CREATE_ASSERTION, L"SQL_CREATE_ASSERTION"},
    {SQL_CREATE_CHARACTER_SET, L"SQL_CREATE_CHARACTER_SET"},
    {SQL_CREATE_COLLATION, L"SQL_CREATE_COLLATION"},
    {SQL_CREATE_DOMAIN, L"SQL_CREATE_DOMAIN"},
    {SQL_CREATE_SCHEMA, L"SQL_CREATE_SCHEMA"},
    {SQL_CS_AUTHORIZATION, L"SQL_CS_AUTHORIZATION"},
    {SQL_CS_CREATE_SCHEMA, L"SQL_CS_CREATE_SCHEMA"},
    {SQL_CREATE_TABLE, L"SQL_CREATE_TABLE"},
    {SQL_CT_CREATE_TABLE, L"SQL_CT_CREATE_TABLE"},
    {SQL_CREATE_TRANSLATION, L"SQL_CREATE_TRANSLATION"},
    {SQL_CREATE_VIEW, L"SQL_CREATE_VIEW"},
    {SQL_CV_CHECK_OPTION, L"SQL_CV_CHECK_OPTION"},
    {SQL_CV_CREATE_VIEW, L"SQL_CV_CREATE_VIEW"},
    {SQL_CURSOR_COMMIT_BEHAVIOR, L"SQL_CURSOR_COMMIT_BEHAVIOR"},
    {SQL_CB_CLOSE, L"SQL_CB_CLOSE"},
    {SQL_CURSOR_ROLLBACK_BEHAVIOR, L"SQL_CURSOR_ROLLBACK_BEHAVIOR"},
    {SQL_CURSOR_SENSITIVITY, L"SQL_CURSOR_SENSITIVITY"},
    {SQL_SENSITIVE, L"SQL_SENSITIVE"},
    {SQL_DATA_SOURCE_NAME, L"SQL_DATA_SOURCE_NAME"},
    {SQL_DATA_SOURCE_READ_ONLY, L"SQL_DATA_SOURCE_READ_ONLY"},
    {SQL_ATTR_ACCESS_MODE, L"SQL_ATTR_ACCESS_MODE"},
    {SQL_DATABASE_NAME, L"SQL_DATABASE_NAME"},
    {SQL_DBMS_NAME, L"SQL_DBMS_NAME"},
    {SQL_DBMS_VER, L"SQL_DBMS_VER"},
    {SQL_DEFAULT_TXN_ISOLATION, L"SQL_DEFAULT_TXN_ISOLATION"},
    {SQL_TXN_READ_COMMITTED, L"SQL_TXN_READ_COMMITTED"},
    {SQL_DESCRIBE_PARAMETER, L"SQL_DESCRIBE_PARAMETER"},
    {SQL_DRIVER_NAME, L"SQL_DRIVER_NAME"},
    {SQL_DRIVER_ODBC_VER, L"SQL_DRIVER_ODBC_VER"},
    {SQL_DRIVER_VER, L"SQL_DRIVER_VER"},
    {SQL_DROP_ASSERTION, L"SQL_DROP_ASSERTION"},
    {SQL_DROP_CHARACTER_SET, L"SQL_DROP_CHARACTER_SET"},
    {SQL_DROP_COLLATION, L"SQL_DROP_COLLATION"},
    {SQL_DROP_DOMAIN, L"SQL_DROP_DOMAIN"},
    {SQL_DROP_SCHEMA, L"SQL_DROP_SCHEMA"},
    {SQL_DROP_TABLE, L"SQL_DROP_TABLE"},
    {SQL_DT_DROP_TABLE, L"SQL_DT_DROP_TABLE"},
    {SQL_DROP_TRANSLATION, L"SQL_DROP_TRANSLATION"},
    {SQL_DROP_VIEW, L"SQL_DROP_VIEW"},
    {SQL_DV_DROP_VIEW, L"SQL_DV_DROP_VIEW"},
    {SQL_DYNAMIC_CURSOR_ATTRIBUTES1, L"SQL_DYNAMIC_CURSOR_ATTRIBUTES1"},
    {SQL_CA1_ABSOLUTE, L"SQL_CA1_ABSOLUTE"},
    {SQL_CA1_BULK_ADD, L"SQL_CA1_BULK_ADD"},
    {SQL_CA1_LOCK_NO_CHANGE, L"SQL_CA1_LOCK_NO_CHANGE"},
    {SQL_CA1_NEXT, L"SQL_CA1_NEXT"},
    {SQL_CA1_POS_DELETE, L"SQL_CA1_POS_DELETE"},
    {SQL_CA1_POS_POSITION, L"SQL_CA1_POS_POSITION"},
    {SQL_CA1_POS_REFRESH, L"SQL_CA1_POS_REFRESH"},
    {SQL_CA1_POS_UPDATE, L"SQL_CA1_POS_UPDATE"},
    {SQL_CA1_POSITIONED_UPDATE, L"SQL_CA1_POSITIONED_UPDATE"},
    {SQL_CA1_POSITIONED_DELETE, L"SQL_CA1_POSITIONED_DELETE"},
    {SQL_CA1_RELATIVE, L"SQL_CA1_RELATIVE"},
    {SQL_CA1_SELECT_FOR_UPDATE, L"SQL_CA1_SELECT_FOR_UPDATE"},
    {SQL_DYNAMIC_CURSOR_ATTRIBUTES2, L"SQL_DYNAMIC_CURSOR_ATTRIBUTES2"},
    {SQL_CA2_LOCK_CONCURRENCY, L"SQL_CA2_LOCK_CONCURRENCY"},
    {SQL_CA2_MAX_ROWS_CATALOG, L"SQL_CA2_MAX_ROWS_CATALOG"},
    {SQL_CA2_MAX_ROWS_DELETE, L"SQL_CA2_MAX_ROWS_DELETE"},
    {SQL_CA2_MAX_ROWS_INSERT, L"SQL_CA2_MAX_ROWS_INSERT"},
    {SQL_CA2_MAX_ROWS_SELECT, L"SQL_CA2_MAX_ROWS_SELECT"},
    {SQL_CA2_MAX_ROWS_UPDATE, L"SQL_CA2_MAX_ROWS_UPDATE"},
    {SQL_CA2_OPT_ROWVER_CONCURRENCY, L"SQL_CA2_OPT_ROWVER_CONCURRENCY"},
    {SQL_CA2_OPT_VALUES_CONCURRENCY, L"SQL_CA2_OPT_VALUES_CONCURRENCY"},
    {SQL_CA2_READ_ONLY_CONCURRENCY, L"SQL_CA2_READ_ONLY_CONCURRENCY"},
    {SQL_CA2_SENSITIVITY_ADDITIONS, L"SQL_CA2_SENSITIVITY_ADDITIONS"},
    {SQL_CA2_SENSITIVITY_UPDATES, L"SQL_CA2_SENSITIVITY_UPDATES"},
    {SQL_CA2_SIMULATE_UNIQUE, L"SQL_CA2_SIMULATE_UNIQUE"},
    {SQL_EXPRESSIONS_IN_ORDERBY, L"SQL_EXPRESSIONS_IN_ORDERBY"},
    {SQL_FETCH_DIRECTION, L"SQL_FETCH_DIRECTION"},
    {SQL_FD_FETCH_ABSOLUTE, L"SQL_FD_FETCH_ABSOLUTE"},
    {SQL_FD_FETCH_BOOKMARK, L"SQL_FD_FETCH_BOOKMARK"},
    {SQL_FD_FETCH_FIRST, L"SQL_FD_FETCH_FIRST"},
    {SQL_FD_FETCH_LAST, L"SQL_FD_FETCH_LAST"},
    {SQL_FD_FETCH_NEXT, L"SQL_FD_FETCH_NEXT"},
    {SQL_FD_FETCH_PRIOR, L"SQL_FD_FETCH_PRIOR"},
    {SQL_FD_FETCH_RELATIVE, L"SQL_FD_FETCH_RELATIVE"},
    {SQL_FILE_USAGE, L"SQL_FILE_USAGE"},
    {SQL_FILE_NOT_SUPPORTED, L"SQL_FILE_NOT_SUPPORTED"},
    {SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1, L"SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1"},
    {SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2, L"SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2"},
    {SQL_GETDATA_EXTENSIONS, L"SQL_GETDATA_EXTENSIONS"},
    {SQL_GD_BLOCK, L"SQL_GD_BLOCK"},
    {SQL_GROUP_BY, L"SQL_GROUP_BY"},
    {SQL_GB_GROUP_BY_CONTAINS_SELECT, L"SQL_GB_GROUP_BY_CONTAINS_SELECT"},
    {SQL_IDENTIFIER_CASE, L"SQL_IDENTIFIER_CASE"},
    {SQL_IC_MIXED, L"SQL_IC_MIXED"},
    {SQL_IC_SENSITIVE, L"SQL_IC_SENSITIVE"},
    {SQL_IDENTIFIER_QUOTE_CHAR, L"SQL_IDENTIFIER_QUOTE_CHAR"},
    {SQL_INDEX_KEYWORDS, L"SQL_INDEX_KEYWORDS"},
    {SQL_IK_ASC, L"SQL_IK_ASC"},
    {SQL_IK_DESC, L"SQL_IK_DESC"},
    {SQL_INFO_SCHEMA_VIEWS, L"SQL_INFO_SCHEMA_VIEWS"},
    {SQL_INTEGRITY, L"SQL_INTEGRITY"},
    {SQL_KEYSET_CURSOR_ATTRIBUTES1, L"SQL_KEYSET_CURSOR_ATTRIBUTES1"},
    {SQL_CA1_BOOKMARK, L"SQL_CA1_BOOKMARK"},
    {SQL_CA1_BULK_DELETE_BY_BOOKMARK, L"SQL_CA1_BULK_DELETE_BY_BOOKMARK"},
    {SQL_CA1_BULK_FETCH_BY_BOOKMARK, L"SQL_CA1_BULK_FETCH_BY_BOOKMARK"},
    {SQL_CA1_BULK_UPDATE_BY_BOOKMARK, L"SQL_CA1_BULK_UPDATE_BY_BOOKMARK"},
    {SQL_KEYSET_CURSOR_ATTRIBUTES2, L"SQL_KEYSET_CURSOR_ATTRIBUTES2"},
    {SQL_CA2_CRC_EXACT, L"SQL_CA2_CRC_EXACT"},
    {SQL_KEYWORDS, L"SQL_KEYWORDS"},
    {SQL_LIKE_ESCAPE_CLAUSE, L"SQL_LIKE_ESCAPE_CLAUSE"},
    {SQL_LOCK_TYPES, L"SQL_LOCK_TYPES"},
    {SQL_LCK_NO_CHANGE, L"SQL_LCK_NO_CHANGE"},
    {SQL_MAX_ASYNC_CONCURRENT_STATEMENTS, L"SQL_MAX_ASYNC_CONCURRENT_STATEMENTS"},
    {SQL_MAX_BINARY_LITERAL_LEN, L"SQL_MAX_BINARY_LITERAL_LEN"},
    {SQL_MAX_CATALOG_NAME_LEN, L"SQL_MAX_CATALOG_NAME_LEN"},
    {SQL_MAX_CHAR_LITERAL_LEN, L"SQL_MAX_CHAR_LITERAL_LEN"},
    {SQL_MAX_COLUMN_NAME_LEN, L"SQL_MAX_COLUMN_NAME_LEN"},
    {SQL_MAX_COLUMNS_IN_GROUP_BY, L"SQL_MAX_COLUMNS_IN_GROUP_BY"},
    {SQL_MAX_COLUMNS_IN_INDEX, L"SQL_MAX_COLUMNS_IN_INDEX"},
    {SQL_MAX_COLUMNS_IN_ORDER_BY, L"SQL_MAX_COLUMNS_IN_ORDER_BY"},
    {SQL_MAX_COLUMNS_IN_SELECT, L"SQL_MAX_COLUMNS_IN_SELECT"},
    {SQL_MAX_COLUMNS_IN_TABLE, L"SQL_MAX_COLUMNS_IN_TABLE"},
    {SQL_MAX_CONCURRENT_ACTIVITIES, L"SQL_MAX_CONCURRENT_ACTIVITIES"},
    {SQL_MAX_CURSOR_NAME_LEN, L"SQL_MAX_CURSOR_NAME_LEN"},
    {SQL_MAX_DRIVER_CONNECTIONS, L"SQL_MAX_DRIVER_CONNECTIONS"},
    {SQL_MAX_IDENTIFIER_LEN, L"SQL_MAX_IDENTIFIER_LEN"},
    {SQL_MAX_INDEX_SIZE, L"SQL_MAX_INDEX_SIZE"},
    {SQL_MAX_PROCEDURE_NAME_LEN, L"SQL_MAX_PROCEDURE_NAME_LEN"},
    {SQL_MAX_ROW_SIZE, L"SQL_MAX_ROW_SIZE"},
    {SQL_MAX_ROW_SIZE_INCLUDES_LONG, L"SQL_MAX_ROW_SIZE_INCLUDES_LONG"},
    {SQL_MAX_SCHEMA_NAME_LEN, L"SQL_MAX_SCHEMA_NAME_LEN"},
    {SQL_MAX_STATEMENT_LEN, L"SQL_MAX_STATEMENT_LEN"},
    {SQL_MAX_TABLE_NAME_LEN, L"SQL_MAX_TABLE_NAME_LEN"},
    {SQL_MAX_TABLES_IN_SELECT, L"SQL_MAX_TABLES_IN_SELECT"},
    {SQL_MAX_USER_NAME_LEN, L"SQL_MAX_USER_NAME_LEN"},
    {SQL_MAX_OWNER_NAME_LEN, L"SQL_MAX_OWNER_NAME_LEN"},
    {SQL_MAX_QUALIFIER_NAME_LEN, L"SQL_MAX_QUALIFIER_NAME_LEN"},
    {SQL_MULT_RESULT_SETS, L"SQL_MULT_RESULT_SETS"},
    {SQL_MULTIPLE_ACTIVE_TXN, L"SQL_MULTIPLE_ACTIVE_TXN"},
    {SQL_NEED_LONG_DATA_LEN, L"SQL_NEED_LONG_DATA_LEN"},
    {SQL_NON_NULLABLE_COLUMNS, L"SQL_NON_NULLABLE_COLUMNS"},
    {SQL_NNC_NON_NULL, L"SQL_NNC_NON_NULL"},
    {SQL_NULL_COLLATION, L"SQL_NULL_COLLATION"},
    {SQL_NC_LOW, L"SQL_NC_LOW"},
    {SQL_NUMERIC_FUNCTIONS, L"SQL_NUMERIC_FUNCTIONS"},
    {SQL_FN_NUM_ABS, L"SQL_FN_NUM_ABS"},
    {SQL_FN_NUM_ACOS, L"SQL_FN_NUM_ACOS"},
    {SQL_FN_NUM_ASIN, L"SQL_FN_NUM_ASIN"},
    {SQL_FN_NUM_ATAN, L"SQL_FN_NUM_ATAN"},
    {SQL_FN_NUM_ATAN2, L"SQL_FN_NUM_ATAN2"},
    {SQL_FN_NUM_CEILING, L"SQL_FN_NUM_CEILING"},
    {SQL_FN_NUM_COS, L"SQL_FN_NUM_COS"},
    {SQL_FN_NUM_COT, L"SQL_FN_NUM_COT"},
    {SQL_FN_NUM_DEGREES, L"SQL_FN_NUM_DEGREES"},
    {SQL_FN_NUM_EXP, L"SQL_FN_NUM_EXP"},
    {SQL_FN_NUM_FLOOR, L"SQL_FN_NUM_FLOOR"},
    {SQL_FN_NUM_LOG, L"SQL_FN_NUM_LOG"},
    {SQL_FN_NUM_LOG10, L"SQL_FN_NUM_LOG10"},
    {SQL_FN_NUM_MOD, L"SQL_FN_NUM_MOD"},
    {SQL_FN_NUM_PI, L"SQL_FN_NUM_PI"},
    {SQL_FN_NUM_POWER, L"SQL_FN_NUM_POWER"},
    {SQL_FN_NUM_RADIANS, L"SQL_FN_NUM_RADIANS"},
    {SQL_FN_NUM_RAND, L"SQL_FN_NUM_RAND"},
    {SQL_FN_NUM_ROUND, L"SQL_FN_NUM_ROUND"},
    {SQL_FN_NUM_SIGN, L"SQL_FN_NUM_SIGN"},
    {SQL_FN_NUM_SIN, L"SQL_FN_NUM_SIN"},
    {SQL_FN_NUM_SQRT, L"SQL_FN_NUM_SQRT"},
    {SQL_FN_NUM_TAN, L"SQL_FN_NUM_TAN"},
    {SQL_ODBC_API_CONFORMANCE, L"SQL_ODBC_API_CONFORMANCE"},
    {SQL_OAC_LEVEL2, L"SQL_OAC_LEVEL2"},
    {SQL_ODBC_SAG_CLI_CONFORMANCE, L"SQL_ODBC_SAG_CLI_CONFORMANCE"},
    {SQL_OSCC_NOT_COMPLIANT, L"SQL_OSCC_NOT_COMPLIANT"},
    {SQL_ODBC_SQL_CONFORMANCE, L"SQL_ODBC_SQL_CONFORMANCE"},
    {SQL_OSC_CORE, L"SQL_OSC_CORE"},
    {SQL_ODBC_SQL_OPT_IEF, L"SQL_ODBC_SQL_OPT_IEF"},
    {SQL_ODBC_VER, L"SQL_ODBC_VER"},
    {SQL_OJ_CAPABILITIES, L"SQL_OJ_CAPABILITIES"},
    {SQL_OJ_ALL_COMPARISON_OPS, L"SQL_OJ_ALL_COMPARISON_OPS"},
    {SQL_OJ_FULL, L"SQL_OJ_FULL"},
    {SQL_OJ_INNER, L"SQL_OJ_INNER"},
    {SQL_OJ_LEFT, L"SQL_OJ_LEFT"},
    {SQL_OJ_NESTED, L"SQL_OJ_NESTED"},
    {SQL_OJ_NOT_ORDERED, L"SQL_OJ_NOT_ORDERED"},
    {SQL_OJ_RIGHT, L"SQL_OJ_RIGHT"},
    {SQL_OUTER_JOINS, L"SQL_OUTER_JOINS"},
    {SQL_ORDER_BY_COLUMNS_IN_SELECT, L"SQL_ORDER_BY_COLUMNS_IN_SELECT"},
    {SQL_OWNER_USAGE, L"SQL_OWNER_USAGE"},
    {SQL_OU_DML_STATEMENTS, L"SQL_OU_DML_STATEMENTS"},
    {SQL_OU_INDEX_DEFINITION, L"SQL_OU_INDEX_DEFINITION"},
    {SQL_OU_PRIVILEGE_DEFINITION, L"SQL_OU_PRIVILEGE_DEFINITION"},
    {SQL_OU_PROCEDURE_INVOCATION, L"SQL_OU_PROCEDURE_INVOCATION"},
    {SQL_OU_TABLE_DEFINITION, L"SQL_OU_TABLE_DEFINITION"},
    {SQL_PARAM_ARRAY_ROW_COUNTS, L"SQL_PARAM_ARRAY_ROW_COUNTS"},
    {SQL_PARC_BATCH, L"SQL_PARC_BATCH"},
    {SQL_PARAM_ARRAY_SELECTS, L"SQL_PARAM_ARRAY_SELECTS"},
    {SQL_PAS_BATCH, L"SQL_PAS_BATCH"},
    {SQL_POS_OPERATIONS, L"SQL_POS_OPERATIONS"},
    {SQL_POS_ADD, L"SQL_POS_ADD"},
    {SQL_POS_DELETE, L"SQL_POS_DELETE"},
    {SQL_POS_POSITION, L"SQL_POS_POSITION"},
    {SQL_POS_REFRESH, L"SQL_POS_REFRESH"},
    {SQL_POS_UPDATE, L"SQL_POS_UPDATE"},
    {SQL_POSITIONED_STATEMENTS, L"SQL_POSITIONED_STATEMENTS"},
    {SQL_PS_POSITIONED_DELETE, L"SQL_PS_POSITIONED_DELETE"},
    {SQL_PS_POSITIONED_UPDATE, L"SQL_PS_POSITIONED_UPDATE"},
    {SQL_PS_SELECT_FOR_UPDATE, L"SQL_PS_SELECT_FOR_UPDATE"},
    {SQL_PROCEDURE_TERM, L"SQL_PROCEDURE_TERM"},
    {SQL_PROCEDURES, L"SQL_PROCEDURES"},
    {SQL_QUALIFIER_USAGE, L"SQL_QUALIFIER_USAGE"},
    {SQL_QUOTED_IDENTIFIER_CASE, L"SQL_QUOTED_IDENTIFIER_CASE"},
    {SQL_ROW_UPDATES, L"SQL_ROW_UPDATES"},
    {SQL_SCHEMA_TERM, L"SQL_SCHEMA_TERM"},
    {SQL_SCHEMA_USAGE, L"SQL_SCHEMA_USAGE"},
    {SQL_SCROLL_OPTIONS, L"SQL_SCROLL_OPTIONS"},
    {SQL_SO_DYNAMIC, L"SQL_SO_DYNAMIC"},
    {SQL_SO_FORWARD_ONLY, L"SQL_SO_FORWARD_ONLY"},
    {SQL_SO_KEYSET_DRIVEN, L"SQL_SO_KEYSET_DRIVEN"},
    {SQL_SO_STATIC, L"SQL_SO_STATIC"},
    {SQL_SCROLL_CONCURRENCY, L"SQL_SCROLL_CONCURRENCY"},
    {SQL_SCCO_LOCK, L"SQL_SCCO_LOCK"},
    {SQL_SCCO_OPT_ROWVER, L"SQL_SCCO_OPT_ROWVER"},
    {SQL_SCCO_OPT_VALUES, L"SQL_SCCO_OPT_VALUES"},
    {SQL_SCCO_READ_ONLY, L"SQL_SCCO_READ_ONLY"},
    {SQL_SEARCH_PATTERN_ESCAPE, L"SQL_SEARCH_PATTERN_ESCAPE"},
    {SQL_SERVER_NAME, L"SQL_SERVER_NAME"},
    {SQL_SPECIAL_CHARACTERS, L"SQL_SPECIAL_CHARACTERS"},
    {SQL_SQL92_DATETIME_FUNCTIONS, L"SQL_SQL92_DATETIME_FUNCTIONS"},
    {SQL_SQL92_FOREIGN_KEY_DELETE_RULE, L"SQL_SQL92_FOREIGN_KEY_DELETE_RULE"},
    {SQL_SQL92_FOREIGN_KEY_UPDATE_RULE, L"SQL_SQL92_FOREIGN_KEY_UPDATE_RULE"},
    {SQL_SQL92_GRANT, L"SQL_SQL92_GRANT"},
    {SQL_SG_WITH_GRANT_OPTION, L"SQL_SG_WITH_GRANT_OPTION"},
    {SQL_SQL92_NUMERIC_VALUE_FUNCTIONS, L"SQL_SQL92_NUMERIC_VALUE_FUNCTIONS"},
    {SQL_SQL92_PREDICATES, L"SQL_SQL92_PREDICATES"},
    {SQL_SP_EXISTS, L"SQL_SP_EXISTS"},
    {SQL_SP_ISNOTNULL, L"SQL_SP_ISNOTNULL"},
    {SQL_SP_ISNULL, L"SQL_SP_ISNULL"},
    {SQL_SP_LIKE, L"SQL_SP_LIKE"},
    {SQL_SP_IN, L"SQL_SP_IN"},
    {SQL_SP_BETWEEN, L"SQL_SP_BETWEEN"},
    {SQL_SP_UNIQUE, L"SQL_SP_UNIQUE"},
    {SQL_SQL92_RELATIONAL_JOIN_OPERATORS, L"SQL_SQL92_RELATIONAL_JOIN_OPERATORS"},
    {SQL_SRJO_CROSS_JOIN, L"SQL_SRJO_CROSS_JOIN"},
    {SQL_SRJO_FULL_OUTER_JOIN, L"SQL_SRJO_FULL_OUTER_JOIN"},
    {SQL_SRJO_INNER_JOIN, L"SQL_SRJO_INNER_JOIN"},
    {SQL_SRJO_LEFT_OUTER_JOIN, L"SQL_SRJO_LEFT_OUTER_JOIN"},
    {SQL_SRJO_RIGHT_OUTER_JOIN, L"SQL_SRJO_RIGHT_OUTER_JOIN"},
    {SQL_SRJO_UNION_JOIN, L"SQL_SRJO_UNION_JOIN"},
    {SQL_SQL92_REVOKE, L"SQL_SQL92_REVOKE"},
    {SQL_SR_GRANT_OPTION_FOR, L"SQL_SR_GRANT_OPTION_FOR"},
    {SQL_SQL92_ROW_VALUE_CONSTRUCTOR, L"SQL_SQL92_ROW_VALUE_CONSTRUCTOR"},
    {SQL_SRVC_DEFAULT, L"SQL_SRVC_DEFAULT"},
    {SQL_SRVC_NULL, L"SQL_SRVC_NULL"},
    {SQL_SRVC_ROW_SUBQUERY, L"SQL_SRVC_ROW_SUBQUERY"},
    {SQL_SRVC_VALUE_EXPRESSION, L"SQL_SRVC_VALUE_EXPRESSION"},
    {SQL_SQL92_STRING_FUNCTIONS, L"SQL_SQL92_STRING_FUNCTIONS"},
    {SQL_SSF_LOWER, L"SQL_SSF_LOWER"},
    {SQL_SSF_UPPER, L"SQL_SSF_UPPER"},
    {SQL_SQL92_VALUE_EXPRESSIONS, L"SQL_SQL92_VALUE_EXPRESSIONS"},
    {SQL_SVE_CASE, L"SQL_SVE_CASE"},
    {SQL_SVE_CAST, L"SQL_SVE_CAST"},
    {SQL_SVE_COALESCE, L"SQL_SVE_COALESCE"},
    {SQL_SVE_NULLIF, L"SQL_SVE_NULLIF"},
    {SQL_STANDARD_CLI_CONFORMANCE, L"SQL_STANDARD_CLI_CONFORMANCE"},
    {SQL_SCC_ISO92_CLI, L"SQL_SCC_ISO92_CLI"},
    {SQL_STATIC_CURSOR_ATTRIBUTES1, L"SQL_STATIC_CURSOR_ATTRIBUTES1"},
    {SQL_STATIC_CURSOR_ATTRIBUTES2, L"SQL_STATIC_CURSOR_ATTRIBUTES2"},
    {SQL_STATIC_SENSITIVITY, L"SQL_STATIC_SENSITIVITY"},
    {SQL_SS_ADDITIONS, L"SQL_SS_ADDITIONS"},
    {SQL_SS_UPDATES, L"SQL_SS_UPDATES"},
    {SQL_STRING_FUNCTIONS, L"SQL_STRING_FUNCTIONS"},
    {SQL_FN_STR_ASCII, L"SQL_FN_STR_ASCII"},
    {SQL_FN_STR_BIT_LENGTH, L"SQL_FN_STR_BIT_LENGTH"},
    {SQL_FN_STR_CHAR, L"SQL_FN_STR_CHAR"},
    {SQL_FN_STR_CONCAT, L"SQL_FN_STR_CONCAT"},
    {SQL_FN_STR_DIFFERENCE, L"SQL_FN_STR_DIFFERENCE"},
    {SQL_FN_STR_INSERT, L"SQL_FN_STR_INSERT"},
    {SQL_FN_STR_LCASE, L"SQL_FN_STR_LCASE"},
    {SQL_FN_STR_LEFT, L"SQL_FN_STR_LEFT"},
    {SQL_FN_STR_LENGTH, L"SQL_FN_STR_LENGTH"},
    {SQL_FN_STR_LOCATE_2, L"SQL_FN_STR_LOCATE_2"},
    {SQL_FN_STR_LTRIM, L"SQL_FN_STR_LTRIM"},
    {SQL_FN_STR_OCTET_LENGTH, L"SQL_FN_STR_OCTET_LENGTH"},
    {SQL_FN_STR_REPEAT, L"SQL_FN_STR_REPEAT"},
    {SQL_FN_STR_RIGHT, L"SQL_FN_STR_RIGHT"},
    {SQL_FN_STR_RTRIM, L"SQL_FN_STR_RTRIM"},
    {SQL_FN_STR_SOUNDEX, L"SQL_FN_STR_SOUNDEX"},
    {SQL_FN_STR_SPACE, L"SQL_FN_STR_SPACE"},
    {SQL_FN_STR_SUBSTRING, L"SQL_FN_STR_SUBSTRING"},
    {SQL_FN_STR_UCASE, L"SQL_FN_STR_UCASE"},
    {SQL_SUBQUERIES, L"SQL_SUBQUERIES"},
    {SQL_SQ_COMPARISON, L"SQL_SQ_COMPARISON"},
    {SQL_SQ_CORRELATED_SUBQUERIES, L"SQL_SQ_CORRELATED_SUBQUERIES"},
    {SQL_SQ_EXISTS, L"SQL_SQ_EXISTS"},
    {SQL_SQ_IN, L"SQL_SQ_IN"},
    {SQL_SQ_QUANTIFIED, L"SQL_SQ_QUANTIFIED"},
    {SQL_SYSTEM_FUNCTIONS, L"SQL_SYSTEM_FUNCTIONS"},
    {SQL_FN_SYS_DBNAME, L"SQL_FN_SYS_DBNAME"},
    {SQL_FN_SYS_IFNULL, L"SQL_FN_SYS_IFNULL"},
    {SQL_FN_SYS_USERNAME, L"SQL_FN_SYS_USERNAME"},
    {SQL_TABLE_TERM, L"SQL_TABLE_TERM"},
    {SQL_TIMEDATE_ADD_INTERVALS, L"SQL_TIMEDATE_ADD_INTERVALS"},
    {SQL_FN_TSI_DAY, L"SQL_FN_TSI_DAY"},
    {SQL_FN_TSI_FRAC_SECOND, L"SQL_FN_TSI_FRAC_SECOND"},
    {SQL_FN_TSI_HOUR, L"SQL_FN_TSI_HOUR"},
    {SQL_FN_TSI_MINUTE, L"SQL_FN_TSI_MINUTE"},
    {SQL_FN_TSI_MONTH, L"SQL_FN_TSI_MONTH"},
    {SQL_FN_TSI_QUARTER, L"SQL_FN_TSI_QUARTER"},
    {SQL_FN_TSI_SECOND, L"SQL_FN_TSI_SECOND"},
    {SQL_FN_TSI_WEEK, L"SQL_FN_TSI_WEEK"},
    {SQL_FN_TSI_YEAR, L"SQL_FN_TSI_YEAR"},
    {SQL_TIMEDATE_DIFF_INTERVALS, L"SQL_TIMEDATE_DIFF_INTERVALS"},
    {SQL_TIMEDATE_FUNCTIONS, L"SQL_TIMEDATE_FUNCTIONS"},
    {SQL_FN_TD_CURDATE, L"SQL_FN_TD_CURDATE"},
    {SQL_FN_TD_CURRENT_DATE, L"SQL_FN_TD_CURRENT_DATE"},
    {SQL_FN_TD_CURRENT_TIME, L"SQL_FN_TD_CURRENT_TIME"},
    {SQL_FN_TD_CURRENT_TIMESTAMP, L"SQL_FN_TD_CURRENT_TIMESTAMP"},
    {SQL_FN_TD_CURTIME, L"SQL_FN_TD_CURTIME"},
    {SQL_FN_TD_DAYNAME, L"SQL_FN_TD_DAYNAME"},
    {SQL_FN_TD_DAYOFMONTH, L"SQL_FN_TD_DAYOFMONTH"},
    {SQL_FN_TD_DAYOFWEEK, L"SQL_FN_TD_DAYOFWEEK"},
    {SQL_FN_TD_DAYOFYEAR, L"SQL_FN_TD_DAYOFYEAR"},
    {SQL_FN_TD_EXTRACT, L"SQL_FN_TD_EXTRACT"},
    {SQL_FN_TD_HOUR, L"SQL_FN_TD_HOUR"},
    {SQL_FN_TD_MINUTE, L"SQL_FN_TD_MINUTE"},
    {SQL_FN_TD_MONTH, L"SQL_FN_TD_MONTH"},
    {SQL_FN_TD_MONTHNAME, L"SQL_FN_TD_MONTHNAME"},
    {SQL_FN_TD_NOW, L"SQL_FN_TD_NOW"},
    {SQL_FN_TD_QUARTER, L"SQL_FN_TD_QUARTER"},
    {SQL_FN_TD_SECOND, L"SQL_FN_TD_SECOND"},
    {SQL_FN_TD_TIMESTAMPADD, L"SQL_FN_TD_TIMESTAMPADD"},
    {SQL_FN_TD_TIMESTAMPDIFF, L"SQL_FN_TD_TIMESTAMPDIFF"},
    {SQL_FN_TD_WEEK, L"SQL_FN_TD_WEEK"},
    {SQL_FN_TD_YEAR, L"SQL_FN_TD_YEAR"},
    {SQL_TXN_CAPABLE, L"SQL_TXN_CAPABLE"},
    {SQL_TC_ALL, L"SQL_TC_ALL"},
    {SQL_TXN_ISOLATION_OPTION, L"SQL_TXN_ISOLATION_OPTION"},
    {SQL_TXN_READ_UNCOMMITTED, L"SQL_TXN_READ_UNCOMMITTED"},
    {SQL_TXN_REPEATABLE_READ, L"SQL_TXN_REPEATABLE_READ"},
    {SQL_TXN_SERIALIZABLE, L"SQL_TXN_SERIALIZABLE"},
    {SQL_UNION, L"SQL_UNION"},
    {SQL_U_UNION, L"SQL_U_UNION"},
    {SQL_U_UNION_ALL, L"SQL_U_UNION_ALL"},
    {SQL_USER_NAME, L"SQL_USER_NAME"}
};
static const std::map<SQLSMALLINT, std::wstring> DataTypes = {
    {SQL_CHAR, L"SQL_CHAR"},
    {SQL_VARCHAR, L"SQL_VARCHAR"},
    {SQL_LONGVARCHAR, L"SQL_LONGVARCHAR"},
    {SQL_WCHAR, L"SQL_WCHAR"},
    {SQL_WVARCHAR, L"SQL_WVARCHAR"},
    {SQL_WLONGVARCHAR, L"SQL_WLONGVARCHAR"},
    {SQL_DECIMAL, L"SQL_DECIMAL"},
    {SQL_NUMERIC, L"SQL_NUMERIC"},
    {SQL_SMALLINT, L"SQL_SMALLINT"},
    {SQL_INTEGER, L"SQL_INTEGER"},
    {SQL_REAL, L"SQL_REAL"},
    {SQL_FLOAT, L"SQL_FLOAT"},
    {SQL_DOUBLE, L"SQL_DOUBLE"},
    {SQL_BIT, L"SQL_BIT"},
    {SQL_TINYINT, L"SQL_TINYINT"},
    {SQL_BIGINT, L"SQL_BIGINT"},
    {SQL_BINARY, L"SQL_BINARY"},
    {SQL_VARBINARY, L"SQL_VARBINARY"},
    {SQL_LONGVARBINARY, L"SQL_LONGVARBINARY"},
    {SQL_TYPE_DATE, L"SQL_TYPE_DATE"},
    {SQL_TYPE_TIME, L"SQL_TYPE_TIME"},
    {SQL_TYPE_TIMESTAMP, L"SQL_TYPE_TIMESTAMP"},
    {SQL_INTERVAL_MONTH, L"SQL_INTERVAL_MONTH"},
    {SQL_INTERVAL_YEAR, L"SQL_INTERVAL_YEAR"},
    {SQL_INTERVAL_YEAR_TO_MONTH, L"SQL_INTERVAL_YEAR_TO_MONTH"},
    {SQL_INTERVAL_DAY, L"SQL_INTERVAL_DAY"},
    {SQL_INTERVAL_HOUR, L"SQL_INTERVAL_HOUR"},
    {SQL_INTERVAL_MINUTE, L"SQL_INTERVAL_MINUTE"},
    {SQL_INTERVAL_SECOND, L"SQL_INTERVAL_SECOND"},
    {SQL_INTERVAL_DAY_TO_HOUR, L"SQL_INTERVAL_DAY_TO_HOUR"},
    {SQL_INTERVAL_DAY_TO_MINUTE, L"SQL_INTERVAL_DAY_TO_MINUTE"},
    {SQL_INTERVAL_DAY_TO_SECOND, L"SQL_INTERVAL_DAY_TO_SECOND"},
    {SQL_INTERVAL_HOUR_TO_MINUTE, L"SQL_INTERVAL_HOUR_TO_MINUTE"},
    {SQL_INTERVAL_HOUR_TO_SECOND, L"SQL_INTERVAL_HOUR_TO_SECOND"},
    {SQL_INTERVAL_MINUTE_TO_SECOND, L"SQL_INTERVAL_MINUTE_TO_SECOND"},
    {SQL_GUID, L"SQL_GUID"},
};
static const std::map<SQLSMALLINT, std::wstring> ParamIOTypes = {
    {SQL_PARAM_INPUT, L"SQL_PARAM_INPUT"},
    {SQL_PARAM_OUTPUT, L"SQL_PARAM_OUTPUT"},
    {SQL_PARAM_INPUT_OUTPUT, L"SQL_PARAM_INPUT_OUTPUT"},
};


RETCODE ret_success(RETCODE retcode){
    return retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO;
}


void DBColumn::load(HSTMT *hstmt, SQLUSMALLINT column){
    RETCODE retcode;

    index = column;

    SQLWCHAR wch_name[128];
    SQLSMALLINT len_buf = 128;
    SQLSMALLINT len_name;

    retcode = SQLDescribeCol(
                *hstmt, column, wch_name, len_buf,
                &len_name, &datatype, &datalen, &decimals, &nullable);

    if(!ret_success(retcode)){
        return;
    }

    for(SQLINTEGER ch = 0; ch < len_name; ch++){
        name += wch_name[ch];
    }
}


DBConnection::DBConnection(){}

void DBConnection::initialize(){

    RETCODE retcode;
    m_initialized = false;

    retcode = SQLAllocHandle(SQL_HANDLE_ENV, nullptr, &m_henv);
    if(!ret_success(retcode)){return;}

    SQLPOINTER *odbc_version = reinterpret_cast<SQLPOINTER*>(SQL_OV_ODBC3);
    retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, odbc_version, 0);
    if(!ret_success(retcode)){return;}

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
    if(!ret_success(retcode)){return;}

//    SQLPOINTER login_timeout = reinterpret_cast<SQLPOINTER>(120);
//    retcode = SQLSetConnectAttr(m_hdbc, SQL_LOGIN_TIMEOUT, login_timeout, 0);
//    if(!ret_success(retcode)){return;}

    m_initialized = true;
}
void DBConnection::connect(std::string drv, std::string sys, std::string usr, std::string pwd){

    m_connected = false;

    std::string str_conn;
    str_conn += "DRIVER=";
    str_conn += drv;
    str_conn += ";SYSTEM=";
    str_conn += sys;
    str_conn += ";UID=";
    str_conn += usr;
    str_conn += ";PWD=";
    str_conn += pwd;
    str_conn += ";DFTPKGLIB=QGPL;LANGUAGEID=ENU;PKG=QGPL/DEFAULT(IBM),2,0,1,0,512;QRYSTGLMT=-1;BLOCKSIZE=1;BLOCKFETCH=0;";

    std::wstring wstr_conn_in(str_conn.length(), L' ');
    std::copy(str_conn.begin(), str_conn.end(), wstr_conn_in.begin());

    SQLSMALLINT len_conn_in = static_cast<SQLSMALLINT>(wstr_conn_in.size());
    SQLWCHAR *str_conn_in = const_cast<SQLWCHAR *>(wstr_conn_in.c_str());

    SQLWCHAR str_conn_out[BUF_CONNSTR];
    SQLSMALLINT buf_conn_out;

    SQLUSMALLINT cflag = SQL_DRIVER_NOPROMPT;

    HWND desktopHandle = GetDesktopWindow();

    RETCODE retcode;
    retcode = SQLDriverConnect(m_hdbc, desktopHandle, str_conn_in,
                               len_conn_in,
                               str_conn_out, BUF_CONNSTR, &buf_conn_out, cflag);

    if(!ret_success(retcode)){
        db_error(SQL_HANDLE_DBC);
    }

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
    if(!ret_success(retcode)){return;}

    m_connected = true;
}
void DBConnection::disconnect(){

    RETCODE retcode;

    retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
    if(!ret_success(retcode)){
        return;
    }

    retcode = SQLDisconnect(m_hdbc);
    if(!ret_success(retcode)){
        return;
    }

    retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
    if(!ret_success(retcode)){
        return;
    }

    retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
    if(!ret_success(retcode)){
        return;
    }

    m_connected = false;
}

void DBConnection::prepare(std::string qry){

    std::wstring wstr_qry(qry.length(), L' ');
    std::copy(qry.begin(), qry.end(), wstr_qry.begin());

    SQLWCHAR *wch_qry = const_cast<SQLWCHAR *>(wstr_qry.c_str());
    SQLINTEGER sz_qry = static_cast<SQLINTEGER>(qry.size());

    RETCODE retcode;
    retcode = SQLPrepare(m_hstmt, wch_qry, sz_qry);
    if(!ret_success(retcode)){
        db_error(SQL_HANDLE_STMT);
        return;
    }
}
void DBConnection::execute(){

    RETCODE retcode;
    retcode = SQLExecute(m_hstmt);
    if(!ret_success(retcode)){
        db_error(SQL_HANDLE_STMT);
        return;
    }
}
void DBConnection::fetch(){

//    RETCODE retcode;
//    SQLWCHAR wch_data[128];
//    SQLINTEGER sz_data;

//    DBColumn col;
//    col.load(&m_hstmt, 1);
//    std::wcout << col.name << std::endl;

//    for(retcode = SQLFetch(m_hstmt); retcode == SQL_SUCCESS; retcode=SQLFetch(m_hstmt)){
//        SQLGetData(m_hstmt, 1, SQL_C_WCHAR, wch_data, sizeof(wch_data), &sz_data);
//        std::wcout << sz_data << std::endl;

//    }


    SQLSMALLINT col_count;
    SQLNumResultCols(m_hstmt, &col_count);
    for(SQLUSMALLINT i = 0; i < col_count; i++){
        DBColumn col;
        col.load(&m_hstmt, i);

        std::wcout << col.name << std::endl;
    }
}

void DBConnection::db_error(SQLSMALLINT handletype){

    SQLHANDLE handle = SQLHANDLE();
    switch(handletype){
        case SQL_HANDLE_STMT:{
            handle = m_hstmt;
            break;
        }
        case SQL_HANDLE_ENV:{
            handle = m_henv;
            break;
        }
        case SQL_HANDLE_DBC:{
            handle = m_hdbc;
            break;
        }
    }

    SQLWCHAR err_state[SQL_SQLSTATE_SIZE + 1];
    SQLWCHAR err_msg[DBC_MSG_BUFF];
    SQLINTEGER err_num;
    SQLSMALLINT len_text;

    SQLGetDiagRec(handletype, handle, 1, err_state, &err_num, err_msg, DBC_MSG_BUFF, &len_text);

    std::wcout << std::endl;
    std::wcout << L"SQLERROR: " << err_num << std::endl;
    std::wcout << L"SQLSTATE: " << err_state << std::endl;
    std::wcout << L"MESSAGE: " << std::endl << err_msg << std::endl;
    std::wcout << std::endl;
}


void DBConnection::getmaniinfo(SQLINTEGER pronum){

    std::wstring sproc = L"{ CALL MTDIRECT.GETMANIINFO(?,?,?,?,?,?,?,?,?,?,?,?,?) }";

    SQLLEN sz_pro = SQL_NTS;
    SQLLEN sz_otn = SQL_NTS;
    SQLLEN sz_org = SQL_NTS;
    SQLLEN sz_lvd = SQL_NTS;
    SQLLEN sz_lvt = SQL_NTS;
    SQLLEN sz_dtn = SQL_NTS;
    SQLLEN sz_dst = SQL_NTS;
    SQLLEN sz_ard = SQL_NTS;
    SQLLEN sz_art = SQL_NTS;
    SQLLEN sz_eda = SQL_NTS;
    SQLLEN sz_eta = SQL_NTS;
    SQLLEN sz_sts = SQL_NTS;
    SQLLEN sz_sys = SQL_NTS;

    SQLINTEGER ch_pro;
    SQLINTEGER ch_otn;
    SQLCHAR ch_org[LEN_ORG];
    SQLINTEGER ch_lvd;
    SQLINTEGER ch_lvt;
    SQLINTEGER ch_dtn;
    SQLCHAR ch_dst[LEN_DST];
    SQLINTEGER ch_ard;
    SQLINTEGER ch_art;
    SQLINTEGER ch_eda;
    SQLINTEGER ch_eta;
    SQLCHAR ch_sts[LEN_STS];
    SQLCHAR ch_sys[LEN_SYS];

    SQLSMALLINT iotype = SQL_PARAM_INPUT_OUTPUT;

    SQLBindParameter(m_hstmt, 1, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_PRO, 0, &ch_pro, 0, &sz_pro);
    SQLBindParameter(m_hstmt, 2, iotype, SQL_C_SLONG , SQL_DECIMAL, LEN_OTN, 0, &ch_otn, 0, &sz_otn);
    SQLBindParameter(m_hstmt, 3, iotype, SQL_C_CHAR, SQL_CHAR, LEN_ORG, 0, ch_org, LEN_ORG, &sz_org);
    SQLBindParameter(m_hstmt, 4, iotype, SQL_C_SLONG , SQL_DECIMAL, LEN_LVD, 0, &ch_lvd, 0, &sz_lvd);
    SQLBindParameter(m_hstmt, 5, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_LVT, 0, &ch_lvt, 0, &sz_lvt);
    SQLBindParameter(m_hstmt, 6, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_DTN, 0, &ch_dtn, 0, &sz_dtn);
    SQLBindParameter(m_hstmt, 7, iotype, SQL_C_CHAR, SQL_CHAR, LEN_DST, 0, ch_dst, LEN_DST, &sz_dst);
    SQLBindParameter(m_hstmt, 8, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_ARD, 0, &ch_ard, 0, &sz_ard);
    SQLBindParameter(m_hstmt, 9, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_ART, 0, &ch_art, 0, &sz_art);
    SQLBindParameter(m_hstmt, 10, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_EDA, 0, &ch_eda, 0, &sz_eda);
    SQLBindParameter(m_hstmt, 11, iotype, SQL_C_SLONG, SQL_DECIMAL, LEN_ETA, 0, &ch_eta, 0, &sz_eta);
    SQLBindParameter(m_hstmt, 12, iotype, SQL_C_CHAR, SQL_CHAR, LEN_STS, 0, ch_sts, LEN_STS, &sz_sts);
    SQLBindParameter(m_hstmt, 13, iotype, SQL_C_CHAR, SQL_CHAR, LEN_SYS, 0, ch_sys, LEN_SYS, &sz_sys);

    ch_pro = pronum; //18914753
    ch_otn = 0;
    strcpy_s((char*)ch_org, _countof(ch_org), "");
    ch_lvd = 0;
    ch_lvt = 0;
    ch_dtn = 0;
    strcpy_s((char*)ch_dst, _countof(ch_dst), "");
    ch_ard = 0;
    ch_art = 0;
    ch_eda = 0;
    ch_eta = 0;
    strcpy_s((char*)ch_sts, _countof(ch_sts), "");
    strcpy_s((char*)ch_sys, _countof(ch_sys), "MAN");

    RETCODE rc = SQLExecDirect(m_hstmt, const_cast<SQLWCHAR*>(sproc.c_str()), SQL_NTS);
    if(!ret_success(rc)){
        db_error(SQL_HANDLE_STMT);
        return;
    }

    std::string org;
    std::string dst;
    std::string sts;
    std::string sys;

    for(int i = 0; i < LEN_ORG; i++){
        org += static_cast<CHAR>(ch_org[i]);
    }
    for(int i = 0; i < LEN_DST; i++){
        dst += static_cast<CHAR>(ch_dst[i]);
    }
    for(int i = 0; i < LEN_STS; i++){
        sts += static_cast<CHAR>(ch_sts[i]);
    }
    for(int i = 0; i < LEN_SYS; i++){
        sys += static_cast<CHAR>(ch_sys[i]);
    }

    outparams.push_back(std::to_string(ch_pro));
    outparams.push_back(std::to_string(ch_otn));
    outparams.push_back(org);
    outparams.push_back(std::to_string(ch_lvd));
    outparams.push_back(std::to_string(ch_lvt));
    outparams.push_back(std::to_string(ch_dtn));
    outparams.push_back(dst);
    outparams.push_back(std::to_string(ch_ard));
    outparams.push_back(std::to_string(ch_art));
    outparams.push_back(std::to_string(ch_eda));
    outparams.push_back(std::to_string(ch_eta));
    outparams.push_back(sts);
    outparams.push_back(sys);
}

std::wstring DBConnection::sql_getinfo(const SQLUSMALLINT info){

    RETCODE retcode;

    SQLWCHAR info_value[2048];
    SQLSMALLINT len_str = 0;

    retcode = SQLGetInfo
            (m_hdbc, info,
            static_cast<SQLPOINTER>(&info_value),
            sizeof(info_value), &len_str);

    if(!ret_success(retcode)){
        return std::wstring();
    }

    std::wstring wch_value;
    for(SQLINTEGER ch = 0; ch < len_str; ch++){
        wch_value += info_value[ch];
    }

    return wch_value;
}
void DBConnection::write_db_info(std::string filepath){

    std::wstring dbinfo;
    std::map<SQLSMALLINT, std::wstring>::const_iterator iter;
    for(iter = InfoTypes.begin(); iter != InfoTypes.end(); iter++){

        std::wofstream wofs;
        wofs.open(filepath.c_str());

        std::wstring info = sql_getinfo(static_cast<USHORT>(iter->first));
        wofs << iter->second << std::endl << info << std::endl;

        if(!wofs.bad()){
            dbinfo += iter->second + L"\n" + info + L"\n\n";
        }

        wofs.close();
    }

    if(filepath.empty()){
        std::wcout << dbinfo << std::endl;
    }
    else {
        std::wofstream wofs;
        wofs.open(filepath.c_str());
        wofs << dbinfo << std::endl;
        wofs.close();
    }
}




////    SQLRETURN SQLColAttribute (
////          SQLHSTMT        StatementHandle,
////          SQLUSMALLINT    ColumnNumber,
////          SQLUSMALLINT    FieldIdentifier,
////          SQLPOINTER      CharacterAttributePtr,
////          SQLSMALLINT     BufferLength,
////          SQLSMALLINT *   StringLengthPtr,
////          SQLLEN *        NumericAttributePtr);


////    RETCODE retcode;

////    SQLUSMALLINT col_num = 1;
////    SQLUSMALLINT field_id = SQL_DESC_LABEL ;
////    SQLPOINTER ch_attrib[128];
////    SQLSMALLINT buff_len = 128;
////    SQLSMALLINT str_len;
////    SQLLEN num_attrib = 0;

////    retcode = SQLColAttribute(hstmt, col_num, field_id, ch_attrib, buff_len, &str_len, &num_attrib);


////    std::cout << std::endl;
////    std::cout << "retcode: " << retcode << std::endl;
////    std::cout << "number: " << col_num << std::endl;
////    std::cout << "field id: " << field_id << std::endl;
////    std::cout << "ch attrib: " << ch_attrib << std::endl;
////    std::cout << "str len: " << str_len << std::endl;
////    std::cout << "num attrib: " << num_attrib << std::endl;
////    std::cout << std::endl;
