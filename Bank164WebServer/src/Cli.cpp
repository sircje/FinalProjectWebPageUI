#include "Cli.h"
#include "Transaction.h"

void Cli::printAccounts() {
    cout << endl << "Bank Accounts" << endl;
    
    Account account(0);
    unique_ptr<Statement> statement = bank.getDatabase().getAccounts(account);
   
    /**
     * TODO: Display all accounts. You have been provded with a unique_ptr
     * that points to a Statement object. An SQL select query has already
     * been prepared in statement, you can see how it was done in
     * Database::getAccounts(). You only need to iterate through the
     * results produced by statement. When it is working the CLI should do
     * the following:
     *
     * 1. List all accounts.
     * 2. View account.
     * 3. Exit.
     * 1
     *
     * Bank Accounts
     * Account 1 has balance 5000
     * Account 2 has balance 9000
     */
}

void Cli::printTransactions(Account &account) {
    cout << endl << "Transactions for account " << account.getId() << endl;
    
    Counterparty counterparty(0, 0, 0, "", "", "");
    Transaction transaction(0, account, counterparty, 0, 0);
    unique_ptr<Statement> statement = bank.getDatabase().getTransactions(account, transaction);
    
    /**
     * TODO: Display all transactions. You have been provded with a unique_ptr
     * that points to a Statement object. An SQL select query has already
     * been prepared in statement, you can see how it was done in
     * Database::getAccounts(). You only need to iterate through the
     * results produced by statement. When it is working the CLI should do
     * the following:
     *
     * Account 1. Make a selection.
     * 1. Display balance.
     * 2. List all transactions.
     * 3. View transaction.
     * 4. Exit.
     * 2
     *
     * Transactions for account 1
     * Transaction 1. 1 4022
     * Transaction 2. 2 -2122
     * Transaction 3. 3 4102
     * Transaction 4. 4 -1002
     */
}

void Cli::printTransaction(long transactionId) {
    Account account(0);
    Counterparty counterparty(0, 0, 0, "", "", "");
    Transaction transaction(0, account, counterparty, 0, 0);
    bank.getDatabase().getTransaction(transactionId, transaction);
    cout << "Transaction " << transaction.getId() << " $" << transaction.getAmount() << endl;
}

void Cli::printGreeting() {
    cout << "============================" << endl;
    cout << "=  Welcome to " << bank.getName() << ".   =" << endl;
    cout << "============================" << endl;
}

void Cli::accountMenu(long accountId) {
    int selection;
    bool exit = false;
    
    Account account(0);
    bank.getDatabase().getAccount(accountId, account);

    while (!exit) {
        cout << endl;
        cout << "Account " << account.getId() << ". Make a selection." << endl;
        cout << "1. Display balance. " << endl;
        cout << "2. List all transactions. " << endl;
        cout << "3. View transaction. " << endl;
        cout << "4. Exit. " << endl;
        cin >> selection;
        switch (selection) {
            case 1:
                cout << endl << "Current balance is $" << account.getBalance() << endl;
                break;
            case 2:
                printTransactions(account);
                break;
            case 3:
                int transactionId;
                cout << "Select transaction by id: " << endl;
                cin >> transactionId;
                printTransaction(transactionId);
                break;
            case 4:
                exit = true;
                break;
            default:
                cout << "Invlaid selection." << endl;
        }
    }
}

void Cli::mainMenu() {
    int selection;
    long accountId;
    bool exit = false;
    
    while (!exit) {
        cout << endl;
        printGreeting();
        cout << "1. List all accounts. " << endl;
        cout << "2. View account. " << endl;
        cout << "3. Exit. " << endl;
        cin >> selection;
        switch (selection) {
            case 1:
                printAccounts();
                break;
            case 2:
                cout << "Select account: ";
                cin >> accountId;
                accountMenu(accountId);
                cout << endl;
                break;
            case 3:
                cout << "Goodbye." << endl;
                exit = true;
                break;
            default:
                cout << "Invlaid selection." << endl;
        }
    }
}

void Cli::start() {
    mainMenu();
}
