#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "Account.h"
#include "Transaction.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

class Database {
private:
    string fileName;
    Session session;
    long getLatestRowId();
public:
    Database(string fileName);
    void rebuildTestDatabase();
    void performTransaction(Transaction &transaction);
    void getAccount(long accountId, Account &account);
    unique_ptr<Statement> getAccounts(Account &account);
    void getTransaction(long transactionId, Transaction &transaction);
    unique_ptr<Statement> getTransactions(Account &account, Transaction &transaction);
};

#endif // DATABASE_H
