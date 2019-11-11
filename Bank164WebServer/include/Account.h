#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include "Transaction.h"
#include "Counterparty.h"
using namespace std;

class Transaction;

class Account {
private:
    long accountId;
    double balance;
    /**
     * Account owns the transactions, they are deleted in the Account destructor.
     */
    vector<Transaction*> transactions;
    friend class Database;
public:
    Account(long accountId);
    double getBalance() const;
    long getId() const;
    void setId(long accountId);
    /**
     * Returns the amount withdrawn. If the requsted amount exceeds the total
     * balance nothing withdrawn and 0 is returned.
     */
    virtual double withdraw(double amount);
    /**
     * Returns the amount deposited.
     */
    double deposit(double amount);
    long getNumberTransactions();
};

#endif //ACCOUNT_H
