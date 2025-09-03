#include <iostream>
#include <cstring>
#include <limits>
using namespace std;

class Train {
private:
    int  trainNumber;
    char trainName[50];
    char source[50];
    char destination[50];
    char trainTime[10];
    bool active;                 // counts only real records

   
    static void copyText(char *dest, const char *src, size_t cap) {
        if (!src) { dest[0] = '\0'; return; }
        strncpy(dest, src, cap - 1);
        dest[cap - 1] = '\0';
    }

public:
    static int trainCount;

    // default: empty / inactive
    Train() : trainNumber(0), active(false) {
        trainName[0] = source[0] = destination[0] = trainTime[0] = '\0';
    }

     //an active record
    Train(int number, const char* name, const char* src, const char* dest, const char* time)
        : trainNumber(number), active(true) {
        copyText(trainName, name, sizeof(trainName));
        copyText(source, src, sizeof(source));
        copyText(destination, dest, sizeof(destination));
        copyText(trainTime, time, sizeof(trainTime));
        trainCount++;
    }

    // count only if the object actually represents a train
    ~Train() {
        if (active) trainCount--;
    }

    // encapsulated setters/getters
    void setTrainNumber(int number) { 
        if (!active) { active = true; trainCount++; }
        trainNumber = number; 
    }
    void setTrainName(const char* name) { 
        if (!active) { active = true; trainCount++; }
        copyText(trainName, name, sizeof(trainName)); 
    }
    void setSource(const char* src) { 
        if (!active) { active = true; trainCount++; }
        copyText(source, src, sizeof(source)); 
    }
    void setDestination(const char* dest) { 
        if (!active) { active = true; trainCount++; }
        copyText(destination, dest, sizeof(destination)); 
    }
    void setTrainTime(const char* time) { 
        if (!active) { active = true; trainCount++; }
        copyText(trainTime, time, sizeof(trainTime)); 
    }

    int getTrainNumber() const { return trainNumber; }
    const char* getTrainName() const { return trainName; }
    const char* getSource() const { return source; }
    const char* getDestination() const { return destination; }
    const char* getTrainTime() const { return trainTime; }
    static int getTrainCount() { return trainCount; }

    
    void setAll(int number, const char* name, const char* src, const char* dest, const char* time) {
        bool wasActive = active;
        if (!active) { active = true; trainCount++; }
        trainNumber = number;
        copyText(trainName, name, sizeof(trainName));
        copyText(source, src, sizeof(source));
        copyText(destination, dest, sizeof(destination));
        copyText(trainTime, time, sizeof(trainTime));
        (void)wasActive; // silence unused warning if any
    }

    // input one train from user
    void inputTrainDetails() {
        cout << "Enter Train Number: ";
        int number;
        while (!(cin >> number)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid. Enter Train Number: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        char name[50], src[50], dest[50], time[10];
        cout << "Enter Train Name: ";
        cin.getline(name, sizeof(name));
        cout << "Enter Source: ";
        cin.getline(src, sizeof(src));
        cout << "Enter Destination: ";
        cin.getline(dest, sizeof(dest));
        cout << "Enter Train Time: ";
        cin.getline(time, sizeof(time));

        setAll(number, name, src, dest, time);
    }

    // print one train nicely
    void displayTrainDetails(int idx = -1) const {
        if (idx >= 0) cout << "Train " << idx << " details:\n";
        cout << "Train Number: " << trainNumber << '\n'
             << "Train Name: " << trainName << '\n'
             << "Source: " << source << '\n'
             << "Destination: " << destination << '\n'
             << "Train Time: " << trainTime << '\n';
    }

    bool isActive() const { return active; }
};

int Train::trainCount = 0;

class RailwaySystem {
private:
    Train trains[100];
    int totalTrains;

public:
    RailwaySystem() : totalTrains(0) {
        //records
        addPreset(101, "Okha Express", "Surat", "Mumbai", "10 AM");
        addPreset(102, "Saurashtra Mail", "Rajkot", "Ahmedabad", "02 PM");
        addPreset(103, "Garib Rath", "Vadodara", "Delhi", "09 PM");
    }

    void addTrain() {
        if (totalTrains >= 100) {
            cout << "Storage full! Cannot add more trains.\n";
            return;
        }
        trains[totalTrains].inputTrainDetails();
        totalTrains++;
    }

    void displayAllTrains() const {
        if (totalTrains == 0) {
            cout << "No train records available.\n";
            return;
        }
        for (int i = 0; i < totalTrains; ++i) {
            cout << "\n";
            trains[i].displayTrainDetails(i + 1);
        }
        cout << "\nTotal active trains: " << Train::getTrainCount() << "\n";
    }

    void searchTrainByNumber(int number) const {
        for (int i = 0; i < totalTrains; ++i) {
            if (trains[i].isActive() && trains[i].getTrainNumber() == number) {
                trains[i].displayTrainDetails(i + 1);
                return;
            }
        }
        cout << "Train with number " << number << " not found!\n";
    }

private:
    void addPreset(int number, const char* name, const char* src, const char* dest, const char* time) {
        trains[totalTrains].setAll(number, name, src, dest, time);
        totalTrains++;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    RailwaySystem system;

    while (true) {
        cout << "\n--- Railway Reservation System Menu ---\n";
        cout << "1. Add New Train Record\n";
        cout << "2. Display All Train Records\n";
        cout << "3. Search Train by Number\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice.\n";
            continue;
        }

        if (choice == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system.addTrain();
        } else if (choice == 2) {
            system.displayAllTrains();
        } else if (choice == 3) {
            cout << "Enter Train Number to search: ";
            int num;
            if (cin >> num) {
                system.searchTrainByNumber(num);
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid train number.\n";
            }
        } else if (choice == 4) {
            cout << "Exiting the system. Goodbye!\n";
            break;
        } else {
            cout << "Please choose 1-4.\n";
        }
    }
    return 0;
}
