#include<iostream>
#include "Bank.h"
#include "Cli.h"
#include "WebServer.h"
#pragma comment(lib, "ws2_32.lib") // for Visual Studio, pulls in a Windows library
#pragma comment(lib, "IPHLPAPI.lib") // for Visual Studio, pulls in a Windows library
using namespace std;

Bank::~Bank() {
    for (auto pair : idAccountMap) {
        delete pair.second;
    }
}

long Bank::getUniqueId() {
    return nextUniqueId++;
}

int main(int argc, char **argv) {
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();
    
    Database database("bank.sqlite");
    database.rebuildTestDatabase();
    
    Bank bank("1/64 Bank", database);
    
    WebServer webServer(bank, argc, argv);
    
    Cli cli(bank);
    cli.start();
    
    return 0;
}
