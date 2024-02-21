#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Some utils
bool startswith(const string &s, char c)
{
    return !s.empty() && s[0] == c;
}

void clearScreen()
{
    system("clear"); // for linux users
    system("cls");   // for windows users
}

// Single transaction
class Transaction
{
private:
    string description;
    double amount;

public:
    Transaction(string transDescription, double transAmount) : description(transDescription), amount{transAmount} {}
    string getDescription() const { return description; }
    double getAmount() const { return amount; }

    // overloaded << operator for transaction printing
    friend ostream &operator<<(ostream &os, const Transaction &transaction)
    {
        os << "Description: " << transaction.description << ", Amount: $" << transaction.amount;
        return os;
    }
};

// Base Account class
class Account
{
protected:
    string username;
    string password;
    int ID;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accUsername, string accPassword) : username(accUsername), password(accPassword), balance(0) {}

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void displayTransactionHistory() const = 0;
    virtual bool authenticate(string accUsername, string accPassword) const = 0;
};

// Checking Account subclass
class CheckingAccount : public Account
{
private:
    double overdraftLimit;

public:
    CheckingAccount(string accUsername, string accPassword, double overdraft) : Account(accUsername, accPassword), overdraftLimit(overdraft) {}

    void deposit(double amount) override
    {
        if (amount <= 0 or cin.fail())
        {
            cout << "Sorry, the amount you entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            balance += amount;
            transactions.push_back(Transaction("Deposit", amount));
            cout << "Deposit of $" << amount << " successful.\nCurrent balance: $" << balance << endl;
        }
    }

    void withdraw(double amount) override
    {
        if (amount <= 0 or cin.fail())
        {
            cout << "Sorry, the amount you entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (balance + overdraftLimit >= amount)
        {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", -amount));
            cout << "Withdrawal of $" << amount << " successful.\nRemaining balance: $" << balance << endl;
        }
        else
        {
            cout << "Insufficient funds." << endl;
        }
    }

    void displayTransactionHistory() const override
    {
        cout << "Transaction History for Checking Account " << username << ":" << endl;
        for (const Transaction &transaction : transactions)
        {
            cout << transaction << endl;
        }
        cout << "Current Balance: $" << balance << endl;
    }

    bool authenticate(string accUsername, string accPassword) const override
    {
        return (accUsername == username && accPassword == password);
    }
};

// Savings Account subclass
class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    SavingsAccount(string accUsername, string accPassword, double interest) : Account(accUsername, accPassword), interestRate(interest) {}

    void deposit(double amount) override
    {
        if (amount <= 0 or cin.fail())
        {
            cout << "Sorry, the amount you entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            balance += amount;
            transactions.push_back(Transaction("Deposit", amount));
            cout << "Deposit of $" << amount << " successful.\nCurrent balance: $" << balance << endl;
        }
    }

    void withdraw(double amount) override
    {
        if (amount <= 0 or cin.fail())
        {
            cout << "Sorry, the amount you entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (balance >= amount)
        {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", -amount));
            cout << "Withdrawal of $" << amount << " successful.\nRemaining balance: $" << balance << endl;
        }
        else
        {
            cout << "Insufficient funds." << endl;
        }
    }

    void displayTransactionHistory() const override
    {
        cout << "Transaction History for Savings Account " << username << ":" << endl;
        for (const Transaction &transaction : transactions)
        {
            cout << transaction << endl;
        }
        cout << "Current Balance: $" << balance << endl;
    }

    bool authenticate(string accUsername, string accPassword) const override
    {
        return (accUsername == username && accPassword == password);
    }
};

// Main
int main()
{
    vector<Account *> accounts;
    accounts.push_back(new CheckingAccount("zachary", "checking", 100));
    accounts.push_back(new SavingsAccount("zachary", "savings", 0.05));

    while (true)
    {
        string process;
        cout << "Welcome to ZBanking, what would you like to do?" << endl;
        cin >> process;
        if (startswith(process, 'l')) // login
        {
            // Authentication
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            clearScreen();
            cout << "Enter password: ";
            cin >> password;
            clearScreen();

            bool loggedIn = false;
            for (Account *acc : accounts)
            {
                if (acc->authenticate(username, password))
                {
                    loggedIn = true;
                    cout << "Authentication successful.\n\nWelcome to ZBanking, " << username << "!" << endl;
                    while (true)
                    {
                        string userInput;
                        cout << ">>> ";
                        cin >> userInput;

                        if (startswith(userInput, 'd')) // deposit
                        {
                            cout << "Are you sure you would like to deposit? (yes/no)" << endl;
                            cin >> userInput;
                            if (startswith(userInput, 'y')) // yes
                            {
                                int depositAmount;
                                cout << "Deposit amount: $";
                                cin >> depositAmount;
                                acc->deposit(depositAmount);
                            }
                            else
                            {
                                cout << "Cancelling deposit process." << endl;
                            }
                        }
                        else if (startswith(userInput, 'w')) // withdraw
                        {
                            cout << "Are you sure you would like to withdraw? (yes/no)" << endl;
                            cin >> userInput;
                            if (startswith(userInput, 'y')) // yes
                            {
                                int withdrawAmount;
                                cout << "Withdraw amount: $";
                                cin >> withdrawAmount;
                                acc->withdraw(withdrawAmount);
                            }
                            else
                            {
                                cout << "Cancelling withdrawal process." << endl;
                            }
                        }
                        else if (startswith(userInput, 't')) // transactions
                        {
                            acc->displayTransactionHistory();
                        }
                        else if (startswith(userInput, 'q')) // quit
                        {
                            clearScreen();
                            break;
                        }
                        else if (startswith(userInput, 'h')) // help
                        {
                            cout << "deposit        | Deposits money into your account." << endl;
                            cout << "withdraw       | Withdraws money from your account." << endl;
                            cout << "transactions   | Displays your transaction history." << endl;
                            cout << "help           | Displays this message." << endl;
                            cout << "quit           | Logs out of your account." << endl;
                        }
                        else
                        {
                            cout << "We're sorry, the command you issued doesn't seem to exist. Please check your spelling, and if it still doesn't work, ask a nearby employee for help." << endl;
                        }
                    }
                }
            }
            if (!loggedIn)
            {
                cout << "Incorrect username or password. Please try again." << endl;
                cin.get();
            }
        }
        else if (startswith(process, 'h'))
        {
            cout << "login          | Log into your ZBanking account." << endl;
            cout << "help           | Displays this message." << endl;
        }
    }
}
