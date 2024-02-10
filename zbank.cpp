#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// Declare classes
class Transaction;
class Account;

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

    friend ostream &operator<<(ostream &os, const Transaction &transaction)
    {
        os << "Description: " << transaction.description << ", Amount: $" << transaction.amount;
        return os;
    }
};

// Single account
class Account
{
private:
    string username;
    string password;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accUsername, string accPassword) : username(accUsername), password(accPassword), balance(0) {}
    // Auth user
    bool authenticate(string accUsername, string accPassword)
    {
        return (accUsername == username and accPassword == password);
    }
    // Deposit money
    void deposit(double amount)
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
    // Withdraw money
    void withdraw(double amount)
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
    // Display transaction history
    void displayTransactionHistory() const
    {
        cout << "Transaction History for " << username << ":" << endl;
        for (const Transaction &transaction : transactions)
        {
            cout << transaction << endl;
        }
        cout << "Current Balance: $" << balance << endl;
    }
};

void clearScreen()
{
    system("clear"); // for linux users
    system("cls");   // for windows users
}

// Main
int main()
{
    // Logout flag
    bool logout = false;
    // You need to replace this with your server's API and create new instances of account and overwrite any matching fields
    Account account("zachary", "123");
    while (true)
    {
        // Authentication
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        clearScreen();
        cout << "Enter password: ";
        cin >> password;
        clearScreen();

        if (account.authenticate(username, password))
        {
            cout << "Authentication successful.\n\nWelcome to ZBanking, " << username << "!" << endl;
            while (true)
            {
                string userInput;
                cout << ">>> ";
                cin >> userInput;

                // The deposit and withdraw commands need integration with your chosen API
                if (userInput == "deposit")
                {
                    int depositAmount;
                    cout << "Deposit amount: $";
                    cin >> depositAmount;
                    account.deposit(depositAmount);
                }
                else if (userInput == "withdraw")
                {
                    int withdrawAmount;
                    cout << "Withdraw amount: $";
                    cin >> withdrawAmount;
                    account.withdraw(withdrawAmount);
                }
                else if (userInput == "history" or userInput == "transactions")
                {
                    account.displayTransactionHistory();
                }
                else if (userInput == "quit" or userInput == "exit")
                {
                    clearScreen();
                    break;
                }
                else
                {
                    cout << "We're sorry, the command you issued doesn't seem to exist. Please check your spelling, and if it still doesn't work, ask a nearby employee for help." << endl;
                }
            }
        }
        else
        {
            cout << "Incorrect username or password. Please try again." << endl;
            cin.get();
        }
    }
}