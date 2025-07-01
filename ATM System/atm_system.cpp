#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Account class to store user information
class Account {
private:
    string accountNumber;
    string pin;
    string name;
    double balance;
    vector<string> transactionHistory;

public:
    Account(string accNum, string pin, string name, double balance)
        : accountNumber(accNum), pin(pin), name(name), balance(balance) {}

    string getAccountNumber() const { return accountNumber; }
    string getPin() const { return pin; }
    string getName() const { return name; }
    double getBalance() const { return balance; }
    vector<string> getTransactionHistory() const { return transactionHistory; }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        transactionHistory.push_back("Withdrawal: -$" + to_string(amount));
        return true;
    }

    void deposit(double amount) {
        balance += amount;
        transactionHistory.push_back("Deposit: +$" + to_string(amount));
    }

    bool transfer(Account& recipient, double amount) {
        if (amount > balance) return false;
        balance -= amount;
        recipient.balance += amount;
        transactionHistory.push_back("Transfer to " + recipient.getAccountNumber() + ": -$" + to_string(amount));
        recipient.transactionHistory.push_back("Transfer from " + accountNumber + ": +$" + to_string(amount));
        return true;
    }

    void changePin(string newPin) {
        pin = newPin;
        transactionHistory.push_back("PIN changed");
    }
};

// ATM class to handle operations
class ATM {
private:
    map<string, Account*> accounts;
    Account* currentAccount;

    string generateAccountNumber() {
        srand(time(0));
        string accNum = "";
        for (int i = 0; i < 10; i++) {
            accNum += to_string(rand() % 10);
        }
        return accNum;
    }

    void displayMenu() {
        cout << "\nATM Menu:\n";
        cout << "1. Check Balance\n";
        cout << "2. Withdraw Money\n";
        cout << "3. Deposit Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Transaction History\n";
        cout << "6. Change PIN\n";
        cout << "7. Logout\n";
        cout << "Enter your choice: ";
    }

    bool authenticate(string accNum, string pin) {
        auto it = accounts.find(accNum);
        if (it != accounts.end() && it->second->getPin() == pin) {
            currentAccount = it->second;
            return true;
        }
        return false;
    }

public:
    ATM() : currentAccount(nullptr) {
        accounts["1234567890"] = new Account("1234567890", "1234", "John Doe", 1000.0);
        accounts["0987654321"] = new Account("0987654321", "4321", "Jane Smith", 500.0);
    }

    ~ATM() {
        for (auto& pair : accounts) {
            delete pair.second;
        }
    }

    void createAccount() {
        string name, pin;
        double initialDeposit;

        cout << "\nCreate New Account\n";
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Create a 4-digit PIN: ";
        cin >> pin;
        while (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
            cout << "PIN must be 4 digits. Please try again: ";
            cin >> pin;
        }

        cout << "Enter initial deposit amount: $";
        cin >> initialDeposit;
        while (initialDeposit <= 0) {
            cout << "Amount must be positive. Please try again: $";
            cin >> initialDeposit;
        }

        string accNum = generateAccountNumber();
        accounts[accNum] = new Account(accNum, pin, name, initialDeposit);

        cout << "\nAccount created successfully!\n";
        cout << "Your account number is: " << accNum << endl;
        cout << "Please remember this number for login.\n";
    }

    void login() {
        string accNum, pin;

        cout << "\nATM Login\n";
        cout << "Enter account number: ";
        cin >> accNum;

        cout << "Enter PIN: ";
        cin >> pin;

        if (authenticate(accNum, pin)) {
            cout << "\nWelcome, " << currentAccount->getName() << "!\n";
            showMainMenu();
        } else {
            cout << "Invalid account number or PIN. Please try again.\n";
        }
    }

    void showMainMenu() {
        int choice;
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 1: checkBalance(); break;
                case 2: withdraw(); break;
                case 3: deposit(); break;
                case 4: transfer(); break;
                case 5: showTransactionHistory(); break;
                case 6: changePin(); break;
                case 7:
                    cout << "Logging out...\n";
                    currentAccount = nullptr;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice != 7 && currentAccount != nullptr);
    }

    void checkBalance() {
        cout << "\nCurrent Balance: $" << fixed << setprecision(2) 
             << currentAccount->getBalance() << endl;
    }

    void withdraw() {
        double amount;
        cout << "\nEnter amount to withdraw: $";
        cin >> amount;

        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }

        if (currentAccount->withdraw(amount)) {
            cout << "Withdrawal successful. Please take your cash.\n";
            cout << "Remaining balance: $" << fixed << setprecision(2) 
                 << currentAccount->getBalance() << endl;
        } else {
            cout << "Insufficient funds.\n";
        }
    }

    void deposit() {
        double amount;
        cout << "\nEnter amount to deposit: $";
        cin >> amount;

        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }

        currentAccount->deposit(amount);
        cout << "Deposit successful.\n";
        cout << "New balance: $" << fixed << setprecision(2) 
             << currentAccount->getBalance() << endl;
    }

    void transfer() {
        string recipientAccNum;
        double amount;

        cout << "\nEnter recipient's account number: ";
        cin >> recipientAccNum;

        auto it = accounts.find(recipientAccNum);
        if (it == accounts.end()) {
            cout << "Recipient account not found.\n";
            return;
        }

        if (recipientAccNum == currentAccount->getAccountNumber()) {
            cout << "Cannot transfer to your own account.\n";
            return;
        }

        cout << "Enter amount to transfer: $";
        cin >> amount;

        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }

        if (currentAccount->transfer(*(it->second), amount)) {
            cout << "Transfer successful.\n";
            cout << "Remaining balance: $" << fixed << setprecision(2) 
                 << currentAccount->getBalance() << endl;
        } else {
            cout << "Insufficient funds for transfer.\n";
        }
    }

    void showTransactionHistory() {
        vector<string> history = currentAccount->getTransactionHistory();
        
        cout << "\nTransaction History for Account: " << currentAccount->getAccountNumber() << endl;
        if (history.empty()) {
            cout << "No transactions found.\n";
        } else {
            for (const string& transaction : history) {
                cout << transaction << endl;
            }
        }
    }

    void changePin() {
        string newPin, confirmPin;

        cout << "\nEnter new 4-digit PIN: ";
        cin >> newPin;
        while (newPin.length() != 4 || !all_of(newPin.begin(), newPin.end(), ::isdigit)) {
            cout << "PIN must be 4 digits. Please try again: ";
            cin >> newPin;
        }

        cout << "Confirm new PIN: ";
        cin >> confirmPin;

        if (newPin == confirmPin) {
            currentAccount->changePin(newPin);
            cout << "PIN changed successfully.\n";
        } else {
            cout << "PINs do not match. No changes made.\n";
        }
    }
};

// Main function
int main() {
    ATM atm;
    int choice;

    do {
        cout << "\nWelcome to the ATM System\n";
        cout << "1. Login\n";
        cout << "2. Create New Account\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: atm.login(); break;
            case 2: atm.createAccount(); break;
            case 3: cout << "Thank you for using our ATM. Goodbye!\n"; break;
            default:
                cout << "Invalid choice. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 3);

    return 0;
}
