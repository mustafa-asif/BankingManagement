#include <iostream>
#include <string>
using namespace std;

struct User {
    string name;
    string password;
};

struct Transaction {
    int amount;
    int balanceAfter;
    string account_from_to, type;
};


struct Account {
    User user;
    int cur_balance = 500;
    Transaction transactions[100];
    int totalTransactions = 0;
};

struct ApplicationState {
    bool userLoggedIn = false;
    Account accounts[100] = {};
    int totalAccounts = 0;
};

ApplicationState app;
Account account;
bool cont = true;

void printOperations() {
    if (!app.userLoggedIn) {
        cout << "PRESS 1 TO OPEN AN ACCOUNT" << endl;
        cout << "PRESS 2 TO LIST OPENED ACCOUNTS" << endl;
        cout << "PRESS 3 TO LOGIN" << endl;
        cout << "PRESS 4 TO EXIT" << endl;
    }
    else {
        cout << "PRESS 1 TO SHOW ACCOUNT DETAILS" << endl;
        cout << "PRESS 2 TO LIST OPENED ACCOUNTS" << endl;
        cout << "PRESS 3 TO CREATE A TRANSACTION" << endl;
        cout << "PRESS 4 TO LOGOUT" << endl;
        cout << "PRESS 5 TO EXIT" << endl;
    }
}

int getUserChoice() {
    int choice;
    cout << "ENTER OPERATION: ";
    cin >> choice;
    return choice;
};
void addNewAccountToAccounts(Account account) {
    app.userLoggedIn = true;
    app.accounts[app.totalAccounts] = account;
    app.totalAccounts++;
}
bool validateUsername(string username) {
    if (!app.totalAccounts) return true;
    for (int i = 0; i < app.totalAccounts; i++) {
        if (app.accounts[i].user.name == username) return false;
    }
    return true;
}
void openNewAccount() {
tryAgain:
    string username, password;
    cout << "ENTER USERNAME: ";
    cin.ignore();
    getline(cin, username);
    if (!validateUsername(username)) {
        cout << "USER ALREADY EXISTS PLEASE TRY AGAIN" << endl;
        goto tryAgain;
    }
    cout << "ENTER PASSWORD: ";
    cin.ignore();
    getline(cin, password);
    account.user.name = username;
    account.user.password = password;
    addNewAccountToAccounts(account);
    cout << "SUCCESSFULLY CREATED YOUR ACCOUNT!! YOU ARE NOW LOGGED IN ;)" << endl;
}
void displayTransactionHistory() {
    for (int i = 0; i < account.totalTransactions; i++) {
        cout << "--------------------TRANSACTION #" << i + 1 << "--------------------" << endl;
        cout << "AMOUNT: " << account.transactions[i].amount << endl;
        if (account.transactions[i].type == "SENT") {
            cout << "TO: " << account.transactions[i].account_from_to << endl;
        }
        else {
            cout << "FROM: " << account.transactions[i].account_from_to << endl;
        }
        cout << "BALANCE AFTER TRANSACTION: " << account.transactions[i].balanceAfter << endl;
        cout << "--------------------------------------------------------------" << endl;
    }
}

void displayCurrentAccountDetails() {
    cout << "SHOWING DETAILS FOR " << account.user.name << endl;
    cout << "CURRENT BALANCE: " << account.cur_balance << endl;
    if (!account.totalTransactions)
        cout << "NO TRANSACTION HISTORY TO BE SHOWN" << endl;
    else displayTransactionHistory();
};

void displayAllAccounts() {
    if (!app.totalAccounts) {
        cout << "THERE ARE NO ACCOUNTS" << endl;
        return;
    }
    for (int i = 0; i < app.totalAccounts; i++) {
        cout << "USERNAME: " << app.accounts[i].user.name << endl;
    }
}

Account findUserByUsername(string username) {
    if (!app.totalAccounts) return {};
    for (int i = 0; i < app.totalAccounts; i++) {
        if (app.accounts[i].user.name == username) {
            return app.accounts[i];
        }
    }
    return {};
}


bool validateUser(string username, string password) {
    if (!app.totalAccounts) cout << "INCORRECT USERNAME OR PASSWORD" << endl;
    for (int i = 0; i < app.totalAccounts; i++) {
        if (app.accounts[i].user.name == username && app.accounts[i].user.password == password) {
            account = app.accounts[i];
            app.userLoggedIn = true;

            return true;
        };
    }
    return false;
}

void login() {
tryAgain:
    string username, password;
    cout << "ENTER USERNAME: ";
    cin.ignore();
    getline(cin, username);
    cout << "ENTER PASSWORD: ";
    cin.ignore();
    getline(cin, password);
    if (!validateUser(username, password)) {
        cout << "INCORRECT USERNAME OR PASSWORD PLEASE TRY AGAIN" << endl;
        goto tryAgain;
    }
    else {
        cout << "WELCOME BACK!! YOU ARE LOGGED IN" << endl;
    }
}

void logoutUser() {
    app.userLoggedIn = false;
    account = {};
}

void transactMoneyToAccount(Account to_account, int amount) {
    Transaction newTransaction;
    newTransaction.account_from_to = account.user.name;
    newTransaction.amount = amount;
    newTransaction.balanceAfter = to_account.cur_balance + amount;
    newTransaction.type = "RECIEVED";
    to_account.cur_balance = newTransaction.balanceAfter;
    to_account.transactions[to_account.totalTransactions] = newTransaction;
    to_account.totalTransactions++;
    for (int i = 0; i < app.totalAccounts; i++) {
        if (to_account.user.name == app.accounts[i].user.name) {
            app.accounts[i] = to_account;
        }
    }
}
void transactMoneyFromAccount(Account to_account, int amount) {
    Transaction newTrasaction;
    newTrasaction.account_from_to = to_account.user.name;
    newTrasaction.amount = amount;
    newTrasaction.balanceAfter = account.cur_balance - amount;
    newTrasaction.type = "SENT";
    account.cur_balance = newTrasaction.balanceAfter;
    account.transactions[account.totalTransactions] = newTrasaction;
    account.totalTransactions++;
    for (int i = 0; i < app.totalAccounts; i++) {
        if (account.user.name == app.accounts[i].user.name) {
            app.accounts[i] = account;
        }
    }
}

void createTransaction() {
tryAgain:
    int amount;
    string to;
    cout << "ENTER AMOUNT YOU WANT TO SEND" << endl;
    cin >> amount;
    if (amount > account.cur_balance) {
        cout << "INSUFFICIENT MONEY!" << endl;
        goto tryAgain;
    }
    cout << "ENTER NAME OF THE USER YOU WANT TO SEND THE MONEY TO: " << endl;
    cin.ignore();
    getline(cin, to);
    if (to == account.user.name) {
        cout << "CAN NOT SEND MONEY INTO YOUR OWN ACCOUNT! " << endl;
        goto tryAgain;
    }
    for (int i = 0; i < app.totalAccounts; i++) {
        if (app.accounts[i].user.name == to) {
            cout << "USER FOUND CREATING TRANSACTION" << endl;
            transactMoneyToAccount(app.accounts[i], amount);
            transactMoneyFromAccount(app.accounts[i], amount);
            return;
        }
    }
    cout << "ACCOUNT NOT FOUND PLEASE TRY AGAIN" << endl;
    goto tryAgain;
}
void exitProgram() {
    cont = false;
}

void performTask() {
    printOperations();
    int choice = getUserChoice();
    if (!app.userLoggedIn) {
        switch (choice)
        {
        case 1:
            openNewAccount();
            break;
        case 2:
            displayAllAccounts();
            break;
        case 3:
            login();
            break;
        case 4:
            exitProgram();
            break;
        default:
            cout << "invalid choice please try again." << endl;
            break;
        }
    }
    else {
        switch (choice)
        {
        case 1:
            displayCurrentAccountDetails();
            break;
        case 2:
            displayAllAccounts();
            break;
        case 3:
            createTransaction();
            break;
        case 4:
            logoutUser();
            break;
        case 5:
            exitProgram();
            break;
        default:
            cout << "invalid choice please try again." << endl;
            break;
        }
    }
}

void greet() {
    cout << "WELCOME TO BANKIST" << endl;
    cout << "WHAT WOULD YOU LIKE TO PERFORM" << endl;
};



int main() {
    greet();
    while (cont)
    {
        performTask();
    }
    cout << "BYEBYE";
}