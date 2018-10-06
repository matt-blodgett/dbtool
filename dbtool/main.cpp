#include "dbtool.h"

#include <iostream>


int main() {
    DBConnection *dbc = new DBConnection();

    std::string qry = "SELECT * FROM LTL400V1A3.FRP001 FETCH FIRST 5 ROWS ONLY";

    std::string drv = "iSeries Access ODBC Driver";
    std::string sys = "FRED2";
    std::string usr = "";
    std::string pwd = "";

    dbc->initialize();
    dbc->connect(drv, sys, usr, pwd);
    dbc->prepare(qry);
    dbc->execute();
    dbc->fetch();
    dbc->disconnect();

    return 0;
}



//void test(DBConnection *dbc, std::string dsn, std::string usr, std::string pwd){

//std::wcout << _countof(L"DRIVER=iSeries Access ODBC Driver;SYSTEM=FRED2;DFTPKGLIB=QGPL;LANGUAGEID=ENU;PKG=QGPL/DEFAULT(IBM),2,0,1,0,512;QRYSTGLMT=-1;BLOCKSIZE=1;BLOCKFETCH=0;UID=mablodgett;PWD=$kipper4;") << std::endl;

//const wchar_t *ostr = L"DRIVER=iSeries Access ODBC Driver;SYSTEM=FRED2;DFTPKGLIB=QGPL;LANGUAGEID=ENU;PKG=QGPL/DEFAULT(IBM),2,0,1,0,512;QRYSTGLMT=-1;BLOCKSIZE=1;BLOCKFETCH=0;UID=mablodgett;PWD=$kipper4;";

//std::wcout << static_cast<SQLSMALLINT>(wcslen(ostr)) << std::endl;

////    std::string qry = "SELECT * FROM LTL400V1A3.FRP001 FETCH FIRST 5 ROWS ONLY";

//    std::string anixter = "0001265";

//    std::string qry =
//            "SELECT "
//            "LTL400V1A3.FRP001.FHFP, "
//            "LTL400V1A3.FRP001.FHPUDT, "
//            "LTL400V1A3.FRP001.FHPRO, "
//            "TRIM(LTL400V1A3.FRP001.FHTRM), "
//            "TRIM(LTL400V1A3.FRP001.FHTC), "
//            "LTL400V1A3.FRP001.FHRWGT, "
//            "LTL400V1A3.FRP001.FHBWGT, "
//            "LTL400V1A3.FRP001.FHSWGT, "
//            "LTL400V1A3.FRP001.FHINCC, "
//            "LTL400V1A3.FRP001.FHCHG, "
//            "LTL400V1A3.FRP001.FHPO$ "
//            "FROM "
//            "LTL400V1A3.FRP001 "
//            "WHERE "
//            "((LTL400V1A3.FRP001.FHFP = '0001265')"
//            "AND (LTL400V1A3.FRP001.FHPUDT BETWEEN 20180901 AND 20180930)) "
//            "ORDER BY "
//            "LTL400V1A3.FRP001.FHTC, "
//            "LTL400V1A3.FRP001.FHPO$";

//    dbc->initialize();
//    dbc->connect(dsn, usr, pwd);
//    dbc->prepare(qry);
//    dbc->execute();
//    dbc->fetch();
//    dbc->disconnect();
//}







