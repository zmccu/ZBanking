#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
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

enum class TransactionType
{
    DEPOSIT,
    WITHDRAW
};

tm parseDate(const std::string &dateStr)
{
    tm date = {};
    std::istringstream iss(dateStr);
    iss >> std::get_time(&date, "%Y-%m-%d");
    return date;
}

// Single transaction
class Transaction
{
private:
    string description;
    double amount;
    TransactionType type;
    time_t date;

public:
    Transaction(string transDescription, double transAmount, TransactionType transType, time_t transDate) : description(transDescription), amount{transAmount}, type(transType), date(transDate) {}
    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    TransactionType getType() const { return type; }
    time_t getDate() const { return date; }

    // overloaded << operator for transaction printing
    friend ostream &operator<<(ostream &os, const Transaction &transaction)
    {
        string typeString;
        switch (transaction.type)
        {
        case TransactionType::DEPOSIT:
            typeString = "Deposit";
            break;
        case TransactionType::WITHDRAW:
            typeString = "Withdrawal";
            break;
        default:
            typeString = "Unknown";
            break;
        }
        os << "Description: " << transaction.description << " | " << typeString << " of "
           << "$" << transaction.amount << " | Date: " << asctime(localtime(&transaction.date));
        return os;
    }
};

// Transaction List Node
class TransactionNode
{
public:
    Transaction transaction;
    TransactionNode *next;

    TransactionNode(Transaction trans) : transaction(trans), next(nullptr) {}
};

// Transaction List
class TransactionList
{
private:
    TransactionNode *head;

public:
    TransactionList() : head(nullptr) {}

    void addTransaction(Transaction trans)
    {
        TransactionNode *newNode = new TransactionNode(trans);
        if (!head)
        {
            head = newNode;
        }
        else
        {
            TransactionNode *temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void displayTransactions() const
    {
        TransactionNode *temp = head;
        while (temp)
        {
            cout << temp->transaction << endl;
            temp = temp->next;
        }
    }
    // Method to filter transactions within a date range
    void displayTransactionsInRange(time_t startDate, time_t endDate) const
    {
        TransactionNode *temp = head;
        while (temp)
        {
            if (temp->transaction.getDate() >= startDate && temp->transaction.getDate() <= endDate)
            {
                cout << temp->transaction << endl;
            }
            temp = temp->next;
        }
    }
    ~TransactionList()
    {
        while (head)
        {
            TransactionNode *temp = head;
            head = head->next;
            delete temp;
        }
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
    TransactionList transactions;

public:
    Account(string accUsername, string accPassword) : username(accUsername), password(accPassword), balance(0) {}

    double getBalance() const { return balance; }
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void displayTransactionHistory() const = 0;
    virtual void displayTransactionHistoryInRange(time_t startDate, time_t endDate) const = 0;
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
            time_t currentTime = time(nullptr);
            transactions.addTransaction(Transaction("Deposit", amount, TransactionType::DEPOSIT, currentTime));
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
            time_t currentTime = time(nullptr);
            transactions.addTransaction(Transaction("Withdrawal", -amount, TransactionType::WITHDRAW, currentTime));
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
        transactions.displayTransactions();
        cout << "Current Balance: $" << balance << endl;
    }

    void displayTransactionHistoryInRange(time_t startDate, time_t endDate) const override
    {
        cout << "Transaction History for Checking Account " << username << " within date range:" << endl;
        transactions.displayTransactionsInRange(startDate, endDate);
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
            time_t currentTime = time(nullptr);
            transactions.addTransaction(Transaction("Deposit", amount, TransactionType::DEPOSIT, currentTime));
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
            time_t currentTime = time(nullptr);
            transactions.addTransaction(Transaction("Withdrawal", -amount, TransactionType::WITHDRAW, currentTime));
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
        transactions.displayTransactions();
        cout << "Current Balance: $" << balance << endl;
    }
    void displayTransactionHistoryInRange(time_t startDate, time_t endDate) const override
    {
        cout << "Transaction History for Savings Account " << username << " within date range:" << endl;
        transactions.displayTransactionsInRange(startDate, endDate);
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
                        if (startswith(userInput, 'b')) // balance
                        {
                            cout << "Your current balance is: $" << acc->getBalance() << endl;
                        }
                        else if (startswith(userInput, 'd')) // deposit
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
                            cout << "Would you like to set range of time? (yes/no)" << endl;
                            cin >> userInput;
                            if (startswith(userInput, 'y'))
                            {
                                cout << "Enter start date (YYYY-MM-DD): ";
                                string startDateStr;
                                cin >> startDateStr;
                                tm startDate = parseDate(startDateStr);

                                cout << "Enter end date (YYYY-MM-DD): ";
                                string endDateStr;
                                cin >> endDateStr;
                                tm endDate = parseDate(endDateStr);

                                time_t startDateTime = mktime(&startDate);
                                time_t endDateTime = mktime(&endDate);

                                acc->displayTransactionHistoryInRange(startDateTime, endDateTime);
                            }
                            else
                            {
                                acc->displayTransactionHistory();
                            }
                        }
                        else if (startswith(userInput, 'c'))
                        {
                            cout << "Are sure you want to close your account? (yes/no)" << endl;
                            cin >> userInput;
                            if (startswith(userInput, 'y'))
                            {
                                cout << "This is the final warning, are you absolutely sure you want to close your account? (yes/no)" << endl;
                                cin >> userInput;
                                if (startswith(userInput, 'y'))
                                {
                                    if (acc->getBalance() > 0)
                                    {
                                        cout << "Sorry, you cannot close your account until you remove all money from your account." << endl;
                                    }
                                    else if (acc->getBalance() < 0)
                                    {
                                        cout << "Sorry, you cannot close your account until you pay your debts." << endl;
                                    }
                                    else
                                    {
                                        cout << "Closing account..." << endl;
                                        for (auto it = accounts.begin(); it != accounts.end(); ++it)
                                        {
                                            if (*it == acc)
                                            {
                                                delete *it;
                                                accounts.erase(it);
                                                break;
                                            }
                                        }
                                        clearScreen();
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                cout << "Account closure cancelled." << endl;
                            }
                        }
                        else if (startswith(userInput, 'q') or startswith(userInput, 'l')) // quit
                        {
                            clearScreen();
                            break;
                        }
                        else if (startswith(userInput, 'h')) // help
                        {
                            cout << "balance        | Displays your current account balance." << endl;
                            cout << "deposit        | Deposits money into your account." << endl;
                            cout << "withdraw       | Withdraws money from your account." << endl;
                            cout << "transactions   | Displays your transaction history." << endl;
                            cout << "close          | Closes your account." << endl;
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
