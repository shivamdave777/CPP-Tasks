#include <iostream>
#include <string>
using namespace std;

// Base Class
class BankAccount {
protected:
    int accNo;
    string holderName;
    double balance;

public:
    // constructor
    BankAccount(int no, string name, double bal = 0.0) {
        accNo = no;
        holderName = name;
        balance = bal;
    }

    // encapsulation : keeping balance private to outside world
    double getBalance() {
        return balance;
    }

    virtual void deposit(double amt) {
        if(amt > 0) {
            balance += amt;
            cout << "Deposited: " << amt << " | Balance: " << balance << endl;
        } else {
            cout << "Invalid deposit!" << endl;
        }
    }

    virtual void withdraw(double amt) {
        if(amt > 0 && amt <= balance) {
            balance -= amt;
            cout << "Withdrawn: " << amt << " | Balance: " << balance << endl;
        } else {
            cout << "Not enough balance or wrong amount!" << endl;
        }
    }

    virtual void displayInfo() {
        cout << "\n--- Account Info ---\n";
        cout << "Acc No: " << accNo << endl;
        cout << "Holder: " << holderName << endl;
        cout << "Balance: " << balance << endl;
    }

    // polymorphism - will be overriden
    virtual void calculateInterest() {
        cout << "No interest for base account.\n";
    }

    virtual ~BankAccount() {}
};

// Savings Account
class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(int no, string name, double bal, double rate)
    : BankAccount(no, name, bal) {
        interestRate = rate;
    }

    void calculateInterest() override {
        double intr = balance * interestRate / 100;
        cout << "Savings Interest: " << intr << endl;
    }
};

// Checking Account
class CheckingAccount : public BankAccount {
    double overdraftLimit;
public:
    CheckingAccount(int no, string name, double bal, double limit)
    : BankAccount(no, name, bal) {
        overdraftLimit = limit;
    }

    void withdraw(double amt) override {
        if(amt > 0 && amt <= balance + overdraftLimit) {
            balance -= amt;
            cout << "Withdrawn: " << amt << " | Balance: " << balance << endl;
        } else {
            cout << "Exceeds overdraft limit!\n";
        }
    }

    void checkOverdraft() {
        if(balance < 0) {
            cout << "Warning: In overdraft zone!\n";
        } else {
            cout << "Account balance is safe.\n";
        }
    }
};

// Fixed Deposit
class FixedDepositAccount : public BankAccount {
    int term; // months
    double rate;
public:
    FixedDepositAccount(int no, string name, double bal, int t, double r)
    : BankAccount(no, name, bal) {
        term = t;
        rate = r;
    }

    void calculateInterest() override {
        double intr = balance * (rate/100) * (term/12.0);
        cout << "FD Interest for " << term << " months: " << intr << endl;
    }
};

// Menu Driven Program
int main() {
    BankAccount *acc = NULL;
    int choice;

    cout << "===== Banking System =====\n";
    cout << "1. Savings Account\n";
    cout << "2. Checking Account\n";
    cout << "3. Fixed Deposit Account\n";
    cout << "Enter choice: ";
    cin >> choice;

    int no;
    string name;
    double bal;

    cout << "Enter Account No: ";
    cin >> no;
    cout << "Enter Holder Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Starting Balance: ";
    cin >> bal;

    if(choice == 1) {
        double rate;
        cout << "Enter Interest Rate (%): ";
        cin >> rate;
        acc = new SavingsAccount(no, name, bal, rate);
    }
    else if(choice == 2) {
        double limit;
        cout << "Enter Overdraft Limit: ";
        cin >> limit;
        acc = new CheckingAccount(no, name, bal, limit);
    }
    else if(choice == 3) {
        int t;
        double r;
        cout << "Enter Term (months): ";
        cin >> t;
        cout << "Enter Interest Rate (%): ";
        cin >> r;
        acc = new FixedDepositAccount(no, name, bal, t, r);
    }
    else {
        cout << "Wrong choice!\n";
        return 0;
    }

    int act;
    do {
        cout << "\n=== Menu ===\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Show Info\n";
        cout << "4. Calculate Interest\n";
        cout << "5. Exit\n";
        cout << "Enter action: ";
        cin >> act;

        if(act == 1) {
            double amt;
            cout << "Amount: ";
            cin >> amt;
            acc->deposit(amt);
        }
        else if(act == 2) {
            double amt;
            cout << "Amount: ";
            cin >> amt;
            acc->withdraw(amt);
        }
        else if(act == 3) {
            acc->displayInfo();
        }
        else if(act == 4) {
            acc->calculateInterest();
        }
        else if(act == 5) {
            cout << "Thank you!\n";
        }
        else {
            cout << "Invalid action!\n";
        }

    } while(act != 5);

    delete acc;
    return 0;
}
