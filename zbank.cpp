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

// Single account
class Account
{
private:
    string username;
    string password;
    int ID;
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

// Main
int main()
{
    // You need to replace this with your server's API and create new instances of account and overwrite any matching fields
    Account account("zachary", "123");
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

            if (account.authenticate(username, password))
            {
                cout << "Authentication successful.\n\nWelcome to ZBanking, " << username << "!" << endl;
                while (true)
                {
                    string userInput;
                    cout << ">>> ";
                    cin >> userInput;

                    // The deposit and withdraw commands need integration with your chosen API
                    if (startswith(userInput, 'd')) // deposit
                    {
                        cout << "Are you sure you would like to deposit? (yes/no)" << endl;
                        cin >> userInput;
                        if (startswith(userInput, 'y')) // yes
                        {
                            int depositAmount;
                            cout << "Deposit amount: $";
                            cin >> depositAmount;
                            account.deposit(depositAmount);
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
                            account.withdraw(withdrawAmount);
                        }
                        else
                        {
                            cout << "Cancelling withdrawal process." << endl;
                        }
                    }
                    else if (startswith(userInput, 't')) // transactions
                    {
                        account.displayTransactionHistory();
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
            else
            {
                cout << "Incorrect username or password. Please try again." << endl;
                cin.get();
            }
        }
        else if (startswith(process, 'c'))
        {
            continue;
        }
        else if (startswith(process, 'h'))
        {
            cout << "login          | Log into your ZBanking account." << endl;
            cout << "create         | Create an account with ZBanking." << endl;
            cout << "help           | Displays this message." << endl;
        }
    }
}