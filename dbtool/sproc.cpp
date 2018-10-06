#include "dbtool.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc < 6){
        return -1;
    }

    DBConnection *dbc = new DBConnection();

    std::string drv = argv[1];
    std::string sys = argv[2];
    std::string usr = argv[3];
    std::string pwd = argv[4];
    long pro = std::atol(argv[5]);

    dbc->initialize();
    dbc->connect(drv, sys, usr, pwd);
    dbc->getmaniinfo(pro);

    for(size_t i = 0; i < dbc->outparams.size(); i++){
        std::cout << dbc->outparams[i] << std::endl;
    }

    dbc->disconnect();

    return 0;
}
