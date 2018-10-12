#include "db/dbtool.h"
#include <iostream>


int main() {

    std::string sqlpath = "C:/dev/dbtool/dbtool/tests/testsql.txt";
    std::string csvpath = "C:/Users/mablodgett/Desktop/test.csv";

    std::string qry = DBConnection::sql_from_file(sqlpath);

    std::string drv = "iSeries Access ODBC Driver";
    std::string sys = "10.3.0.3";
    std::string uid = "";
    std::string pwd = "";
    uid = "";

    DBConnString dbcns;
    dbcns.add_arg("DRIVER", drv);
    dbcns.add_arg("SYSTEM", sys);
    dbcns.add_arg("UID", uid);
    dbcns.add_arg("PWD", pwd);
    dbcns.add_arg("DFTPKGLIB", "QGPL");
    dbcns.add_arg("LANGUAGEID", "ENU");
    dbcns.add_arg("PKG", "QGPL/DEFAULT(IBM),2,0,1,0,512");
    dbcns.add_arg("QRYSTGLMT", "-1");
    dbcns.add_arg("BLOCKSIZE", "1");
    dbcns.add_arg("BLOCKFETCH", "0");

    DBConnection *dbc = new DBConnection();

    try {

        dbc->initialize();
        dbc->connect(dbcns);

        dbc->disconnect();

    } catch (DBException &dberr) {
        std::cout << dberr.what() << std::endl;

        std::cerr << dberr.msg() << std::endl;
    }

    return 0;
}



//std::vector<DBPrivilege> privs = dbc->get_table_privileges(
//            "FRED2", "LTL400V1A3", "FRP001");

//for(size_t i = 0; i < privs.size(); i++){
//    DBPrivilege p = privs[i];

//    std::cout << std::endl;
//    std::cout << "catalog: ";
//    std::cout << p.catalog;
//    std::cout << std::endl;
//    std::cout << "schema: ";
//    std::cout << p.schema;
//    std::cout << std::endl;
//    std::cout << "name: ";
//    std::cout << p.table;
//    std::cout << std::endl;
//    std::cout << "grantor: ";
//    std::cout << p.grantor;
//    std::cout << std::endl;
//    std::cout << "grantee: ";
//    std::cout << p.grantee;
//    std::cout << std::endl;
//    std::cout << "privilege: ";
//    std::cout << p.privilege;
//    std::cout << std::endl;
//    std::cout << "grantable: ";
//    std::cout << p.isGrantable;
//    std::cout << std::endl;
//    std::cout << "----------";
//    std::cout << std::endl;

//}




//std::vector<DBProcedure> procedures = dbc->get_procedures(
//            "FRED2", "MTDIRECT", "GETMANIINFO");

//for(size_t i = 0; i < procedures.size(); i++){
//    DBProcedure proc = procedures[i];

//    std::cout << std::endl;
//    std::cout << std::endl;
//    std::cout << proc.catalog << std::endl;
//    std::cout << proc.schema << std::endl;
//    std::cout << proc.name << std::endl;
//    std::cout << proc.numInParams << std::endl;
//    std::cout << proc.numOutParams << std::endl;
//    std::cout << proc.numResultSets << std::endl;
//    std::cout << proc.remarks << std::endl;
//    std::cout << proc.type << std::endl;
//}




//std::string catalog = "FRED2";
//std::string schema = "MTDIRECT";
//std::string proc = "GETMANIINFO";

//std::vector<DBColumn> columns = dbc->get_columns(
//            catalog, schema, proc, "", false);

//for(size_t i = 0; i < columns.size(); i++){
//    std::cout << columns[i].name << std::endl;
//}







//std::string catalog = "FRED2";
//std::string schema = "LTL400V1A3";
//std::string table = "FRP001";
//std::string column = "FHPRO";

//std::vector<DBColumn> columns = dbc->get_columns(
//            catalog, schema, table, column);

//for(size_t i = 0; i < columns.size(); i++){
//    std::cout << columns.at(i).name << std::endl;
//}




// COLUMN INFO
//for(size_t col = 1; col < m_columns.size() + 1; col++){

////        SQLSMALLINT fieldid = SQL_DESC_TYPE_NAME;
//    SQLSMALLINT fieldid = SQL_DESC_NAME;
////        SQLSMALLINT fieldid = SQL_DESC_LABEL;

//    SQLCHAR ch_data[DBC_BUF_CHAR_FIELD];
//    SQLSMALLINT strlen;
//    SQLINTEGER num = 0;

//    RETCODE rc = SQLColAttributeA(
//                m_hstmt,
//                static_cast<SQLSMALLINT>(col),
//                fieldid,
//                ch_data,
//                DBC_BUF_CHAR_FIELD,
//                &strlen,
//                &num);
//    if(rc_error(rc, SQL_HANDLE_STMT)){return;}

//    std::cout << col << ": " <<  ch_data << std::endl;
//}



// END
//dbc->initialize();
//dbc->connect(dbcns);
//dbc->execute(qry);
//DBRow row = dbc->fetch_one();
//std::cout << row.fields[0].value << std::endl;

//dbc->end();

//dbc->execute("SELECT * FROM LTL400V1A3.FRP001 FETCH FIRST 10 ROWS ONLY");
//DBRow row2 = dbc->fetch_one();
//std::cout << row2.fields[0].value << std::endl;

//dbc->disconnect();


// PRINT RESULTS
//if(dbc->connected()){
//    dbc->prepare(qry);
//    dbc->execute();

//    std::vector<DBRow> rows = dbc->fetch_all();
//    for(size_t ridx = 0; ridx < rows.size(); ridx++){
//        DBRow row = rows.at(ridx);
//        for(size_t i = 0; i < row.fields.size(); i++){
//            std::cout << row.fields.at(i).value << " - ";
//        }
//        std::cout << std::endl;
//    }

//    dbc->disconnect();
//}


// EXPORT
//DBConnection *dbc = new DBConnection();
//dbc->initialize();
//dbc->connect(dbcns);
//dbc->execute(qry);
//dbc->to_csv(csvpath);
//dbc->disconnect();

