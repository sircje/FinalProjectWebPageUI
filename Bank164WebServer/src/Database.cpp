#include "Database.h"
#include "Counterparty.h"

Database::Database(string fileName) : fileName(fileName), session("SQLite", fileName) {
}

void Database::getTransaction(long transactionId, Transaction &transaction) {
    /**
     * TODO: Complete the following statement (only one line required) so that
     * the transaction for the transactionId is loaded from the database and
     * stored into the passed-by-reference transaction. When it is working the
     * CLI should do the following:
     *
     * Account 1. Make a selection.
     * 1. Display balance.
     * 2. List all transactions.
     * 3. View transaction.
     * 4. Exit.
     * 3
     * Select transaction by id:
     * 2
     * Transaction 2 $-2122
     */

    session << "SELECT TransactionId, CounterpartyId, Amount, Timestamp FROM AccountTransaction WHERE TransactionId = ?"
    /* The SQL string is already provided, finish the statement here. */ ;
}

void Database::getAccount(long accountId, Account &account) {
    session << "SELECT AccountId, Balance FROM Account WHERE AccountId = ?", into(account.accountId), into(account.balance), use(accountId), now;
}

unique_ptr<Statement> Database::getAccounts(Account &account) {
    auto selectPtr = unique_ptr<Statement>(new Statement(session));
    *selectPtr << "SELECT AccountId, Balance FROM Account", into(account.accountId), into(account.balance), range(0, 1);    
    return selectPtr;
}

unique_ptr<Statement> Database::getTransactions(Account &account, Transaction &transaction) {
    auto selectPtr = unique_ptr<Statement>(new Statement(session));
    *selectPtr << "SELECT TransactionId, CounterpartyId, Amount, Timestamp FROM AccountTransaction WHERE AccountId = ?",
        into(transaction.transactionId),
        into(transaction.counterparty.counterPartyId),
        into(transaction.amount),
        into(transaction.timestamp),
        use(account.accountId), range(0, 1);
    return selectPtr;
}

void Database::performTransaction(Transaction &transaction) {
    Account& account = transaction.getAccount();
    if (transaction.getAmount() < 0) {
        account.withdraw(-transaction.getAmount());
    } else {
        account.deposit(transaction.getAmount());
    }
    session << "UPDATE Account SET Balance = ? WHERE AccountId = ?", use(account.balance), use(account.accountId), now;
}

long Database::getLatestRowId() {
    long id;
    session << "SELECT last_insert_rowid()", into(id), now;
    return id;
}

void Database::rebuildTestDatabase() {
    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS AccountTransaction", now;
    session << "DROP TABLE IF EXISTS Counterparty", now;
    session << "DROP TABLE IF EXISTS Account", now;
    
    // (re)create table
    session << "CREATE TABLE Account (AccountId INTEGER PRIMARY KEY, Balance REAL)", now;
    session << "CREATE TABLE Counterparty (CounterpartyId INTEGER PRIMARY KEY, RoutingNumber INTEGER(8), AccountId INTEGER(8), BusinessName TEXT, FirstName TEXT, LastName TEXT, FOREIGN KEY(AccountId) REFERENCES Account(AccountId))", now;
    session << "CREATE TABLE AccountTransaction (TransactionId INTEGER PRIMARY KEY, AccountId, CounterpartyId, Amount REAL, Timestamp INTEGER(8), FOREIGN KEY(AccountId) REFERENCES Account(AccountId), FOREIGN KEY(CounterpartyId) REFERENCES Counterparty(CounterpartyId))", now;
    
    Account account(0);
    Statement insertAccount(session);
    insertAccount << "INSERT INTO Account (Balance) VALUES (?)", use(account.balance);
    insertAccount.execute();
    long accountId1 = getLatestRowId();
    insertAccount.execute();
    long accountId2 = getLatestRowId();
    
    Counterparty counterParty(0, 1010, accountId2, "Acme", "Wile", "Cyote");
    Statement insertCounterParty(session);
    insertCounterParty << "INSERT INTO CounterParty (RoutingNumber, AccountId, BusinessName, FirstName, LastName) VALUES (?, ?, ?, ?, ?)", use(counterParty.routingNumber), use(counterParty.accountId), use(counterParty.businessName), use(counterParty.firstName), use(counterParty.lastName);
    insertCounterParty.execute();
    long counterpartyId1 = getLatestRowId();
    
    counterParty.routingNumber = 1011;
    counterParty.accountId = accountId2;
    counterParty.businessName = "LexCorp";
    counterParty.firstName = "Lex";
    counterParty.lastName = "Luthor";
    insertCounterParty.execute();
    long counterpartyId2 = getLatestRowId();
    
    time_t timestamp = 1570049000;
    account.accountId = accountId1;
    counterParty.accountId = counterpartyId1;
    Transaction transaction(0, account, counterParty, 4022, timestamp);
    Statement insertTransaction(session);
    insertTransaction << "INSERT INTO AccountTransaction (AccountId, CounterpartyId, Amount, Timestamp) VALUES (?, ?, ?, ?)", use(transaction.account.accountId), use(transaction.counterparty.counterPartyId), use(transaction.amount), use(transaction.timestamp);
    insertTransaction.execute();
    performTransaction(transaction);
    account.accountId = accountId1;
    transaction.amount = -2122;
    insertTransaction.execute();
    performTransaction(transaction);
    
    account.accountId = accountId1;
    counterParty.accountId = counterpartyId2;
    transaction.amount = 4102;
    insertTransaction.execute();
    performTransaction(transaction);
    
    account.accountId = accountId1;
    transaction.amount = -1002;
    insertTransaction.execute();
    performTransaction(transaction);
    
    account.accountId = accountId2;
    transaction.amount = 8000;
    insertTransaction.execute();
    performTransaction(transaction);
    
    account.accountId = accountId2;
    transaction.amount = -4000;
    insertTransaction.execute();
    performTransaction(transaction);
}
