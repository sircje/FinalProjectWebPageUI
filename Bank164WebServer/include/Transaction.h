#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>
#include "Account.h"
#include "Counterparty.h"

class Account;

class Transaction {
private:
    long transactionId;
    Account& account;
    Counterparty& counterparty;
    double amount;
    time_t timestamp;
    friend class Database;
public:
    Transaction(long transactionId, Account& account,
	Counterparty& counterparty, double amount, time_t timestamp);
    long getId() const { return transactionId; }
    double getAmount() const { return amount; }
    Account& getAccount() const { return account; }
    Counterparty& getCounterparty() const { return counterparty; }
};

#endif // TRANSACTION_H
