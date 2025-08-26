#include <iostream>
using namespace std;

// --------------------
// Class Declaration
// --------------------
class TimeConverter {
public:
    // Function to convert seconds to HH:MM:SS
    void secondsToHHMMSS(int totalSeconds) {
        int hours, minutes, seconds;

        hours = totalSeconds / 3600;               // 1 hour = 3600 seconds
        totalSeconds = totalSeconds % 3600;        // remaining seconds

        minutes = totalSeconds / 60;               // 1 minute = 60 seconds
        seconds = totalSeconds % 60;               // remaining seconds

        cout << "HH:MM:SS => " << hours << ":" << minutes << ":" << seconds << endl;
    }

    // Function to convert HH:MM:SS to total seconds
    void HHMMSSToSeconds(int h, int m, int s) {
        int totalSeconds = (h * 3600) + (m * 60) + s;
        cout << "Total seconds: " << totalSeconds << endl;
    }
};

// --------------------
// Main Function
// --------------------
int main() {
    TimeConverter tc;  // create object of class
    int choice;

    cout << "==============================" << endl;
    cout << "      TIME CONVERTER MENU     " << endl;
    cout << "==============================" << endl;
    cout << "1. Convert Seconds to HH:MM:SS" << endl;
    cout << "2. Convert HH:MM:SS to Seconds" << endl;
    cout << "3. Exit" << endl;

    cout << "\nEnter your choice: ";
    cin >> choice;

    switch(choice) {
        case 1: {
            int totalSeconds;
            cout << "Enter total seconds: ";
            cin >> totalSeconds;
            tc.secondsToHHMMSS(totalSeconds);
            break;
        }
        case 2: {
            int h, m, s;
            cout << "Enter hours: ";
            cin >> h;
            cout << "Enter minutes: ";
            cin >> m;
            cout << "Enter seconds: ";
            cin >> s;
            tc.HHMMSSToSeconds(h, m, s);
            break;
        }
        case 3:
            cout << "Exiting program... Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please run the program again." << endl;
    }

    return 0;
}
