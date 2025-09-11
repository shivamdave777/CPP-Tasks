// LibraryManagement.cpp
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <stdexcept>
#include <vector>

using namespace std;

const int MAX_ITEMS = 100;

// Utility functions
bool isValidISBN(const string &isbn) {
    // Very simple ISBN validation: only digits (or digits + hyphens) and length 10 or 13 ignoring hyphens
    string digits;
    for (char c : isbn) {
        if (isdigit((unsigned char)c)) digits.push_back(c);
        else if (c == '-') continue;
        else return false;
    }
    return (digits.length() == 10 || digits.length() == 13);
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Abstract Base Class
class LibraryItem {
private:
    string title;
    string author;
    string dueDate; // empty if not checked out

protected:
    bool checkedOut;

public:
    LibraryItem(const string &t = "", const string &a = "") :
        title(t), author(a), dueDate(""), checkedOut(false) {}

    // Encapsulation: getters/setters
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getDueDate() const { return dueDate; }

    void setTitle(const string &newTitle) { title = newTitle; }
    void setAuthor(const string &newAuthor) { author = newAuthor; }
    void setDueDate(const string &newDueDate) { dueDate = newDueDate; }

    bool isCheckedOut() const { return checkedOut; }

    // Pure virtual functions - must be overridden
    virtual void checkOut() = 0;
    virtual void returnItem() = 0;
    virtual void displayDetails() const = 0;

    virtual ~LibraryItem() = default;
};

// Derived class: Book
class Book : public LibraryItem {
private:
    string isbn;
    int copies; // number of copies available

public:
    Book(const string &t = "", const string &a = "", const string &isbn_ = "", int copies_ = 1)
        : LibraryItem(t, a), isbn(isbn_), copies(copies_) 
    {
        if (copies_ < 0) throw invalid_argument("Copies cannot be negative");
        if (!isbn_.empty() && !isValidISBN(isbn_)) throw invalid_argument("Invalid ISBN format");
    }

    void setISBN(const string &newIsbn) {
        if (!isValidISBN(newIsbn)) throw invalid_argument("Invalid ISBN format");
        isbn = newIsbn;
    }
    string getISBN() const { return isbn; }

    void setCopies(int c) {
        if (c < 0) throw invalid_argument("Copies cannot be negative");
        copies = c;
    }
    int getCopies() const { return copies; }

    // Implement virtual functions
    void checkOut() override {
        if (copies <= 0) {
            cout << "No copies available to check out for \"" << getTitle() << "\".\n";
            return;
        }
        if (!isCheckedOut()) {
            string duedate;
            cout << "Enter due date (e.g., 2025-09-20) for the book \"" << getTitle() << "\": ";
            getline(cin, duedate);
            if (duedate.empty()) duedate = "N/A";
            setDueDate(duedate);
            checkedOut = true;
            copies--;
            cout << "Checked out book \"" << getTitle() << "\". Due date: " << getDueDate() << ". Remaining copies: " << copies << "\n";
        } else {
            // Even if the item object has checkedOut true, there might be more copies
            // We'll allow checkout if copies > 0 (already handled), so reaching here means checkedOut==true and copies>0
            string duedate;
            cout << "Enter due date (e.g., 2025-09-20) for the book \"" << getTitle() << "\": ";
            getline(cin, duedate);
            if (duedate.empty()) duedate = "N/A";
            setDueDate(duedate);
            copies--;
            cout << "Checked out another copy of \"" << getTitle() << "\". Due date: " << getDueDate() << ". Remaining copies: " << copies << "\n";
        }
    }

    void returnItem() override {
        if (getDueDate().empty() && !isCheckedOut() && copies == 0) {
            // edge-case
            cout << "This book does not appear to be checked out.\n";
            return;
        }
        // For simplicity, returning increases copies and resets dueDate only if all copies become available.
        copies++;
        setDueDate("");
        checkedOut = (copies == 0);
        cout << "Book \"" << getTitle() << "\" returned. Copies available: " << copies << "\n";
    }

    void displayDetails() const override {
        cout << "Type: Book\n";
        cout << "Title: " << getTitle() << "\n";
        cout << "Author: " << getAuthor() << "\n";
        cout << "ISBN: " << (isbn.empty() ? "N/A" : isbn) << "\n";
        cout << "Copies available: " << copies << "\n";
        cout << "Checked out: " << (checkedOut ? "Yes" : "No") << (getDueDate().empty() ? "" : " (Due: " + getDueDate() + ")") << "\n";
        cout << "---------------------------\n";
    }
};

// Derived class: DVD
class DVD : public LibraryItem {
private:
    int durationMinutes; // duration in minutes
    string regionCode;

public:
    DVD(const string &t = "", const string &a = "", int duration = 0, const string &region = "")
        : LibraryItem(t, a), durationMinutes(duration), regionCode(region)
    {
        if (duration < 0) throw invalid_argument("Duration cannot be negative");
    }

    void setDuration(int d) {
        if (d < 0) throw invalid_argument("Duration cannot be negative");
        durationMinutes = d;
    }
    int getDuration() const { return durationMinutes; }

    void setRegion(const string &r) { regionCode = r; }
    string getRegion() const { return regionCode; }

    void checkOut() override {
        if (isCheckedOut()) {
            cout << "This DVD \"" << getTitle() << "\" is already checked out. Due: " << (getDueDate().empty() ? "N/A" : getDueDate()) << "\n";
            return;
        }
        string duedate;
        cout << "Enter due date (e.g., 2025-09-20) for the DVD \"" << getTitle() << "\": ";
        getline(cin, duedate);
        if (duedate.empty()) duedate = "N/A";
        setDueDate(duedate);
        checkedOut = true;
        cout << "Checked out DVD \"" << getTitle() << "\". Due date: " << getDueDate() << "\n";
    }

    void returnItem() override {
        if (!isCheckedOut()) {
            cout << "This DVD is not currently checked out.\n";
            return;
        }
        setDueDate("");
        checkedOut = false;
        cout << "DVD \"" << getTitle() << "\" returned.\n";
    }

    void displayDetails() const override {
        cout << "Type: DVD\n";
        cout << "Title: " << getTitle() << "\n";
        cout << "Director/Author: " << getAuthor() << "\n";
        cout << "Duration: " << durationMinutes << " minutes\n";
        cout << "Region: " << (regionCode.empty() ? "N/A" : regionCode) << "\n";
        cout << "Checked out: " << (checkedOut ? "Yes" : "No") << (getDueDate().empty() ? "" : " (Due: " + getDueDate() + ")") << "\n";
        cout << "---------------------------\n";
    }
};

// Derived class: Magazine
class Magazine : public LibraryItem {
private:
    int issueNumber;
    string month;

public:
    Magazine(const string &t = "", const string &a = "", int issue = 0, const string &m = "")
        : LibraryItem(t, a), issueNumber(issue), month(m)
    {
        if (issue < 0) throw invalid_argument("Issue number cannot be negative");
    }

    void setIssueNumber(int i) {
        if (i < 0) throw invalid_argument("Issue number cannot be negative");
        issueNumber = i;
    }
    int getIssueNumber() const { return issueNumber; }

    void setMonth(const string &m) { month = m; }
    string getMonth() const { return month; }

    void checkOut() override {
        if (isCheckedOut()) {
            cout << "This magazine \"" << getTitle() << "\" is already checked out. Due: " << (getDueDate().empty() ? "N/A" : getDueDate()) << "\n";
            return;
        }
        string duedate;
        cout << "Enter due date (e.g., 2025-09-20) for the magazine \"" << getTitle() << "\": ";
        getline(cin, duedate);
        if (duedate.empty()) duedate = "N/A";
        setDueDate(duedate);
        checkedOut = true;
        cout << "Checked out magazine \"" << getTitle() << "\". Due date: " << getDueDate() << "\n";
    }

    void returnItem() override {
        if (!isCheckedOut()) {
            cout << "This magazine is not currently checked out.\n";
            return;
        }
        setDueDate("");
        checkedOut = false;
        cout << "Magazine \"" << getTitle() << "\" returned.\n";
    }

    void displayDetails() const override {
        cout << "Type: Magazine\n";
        cout << "Title: " << getTitle() << "\n";
        cout << "Editor/Author: " << getAuthor() << "\n";
        cout << "Issue Number: " << issueNumber << "\n";
        cout << "Month: " << (month.empty() ? "N/A" : month) << "\n";
        cout << "Checked out: " << (checkedOut ? "Yes" : "No") << (getDueDate().empty() ? "" : " (Due: " + getDueDate() + ")") << "\n";
        cout << "---------------------------\n";
    }
};

// Management of the library
class Library {
private:
    LibraryItem* items[MAX_ITEMS];
    int count;

public:
    Library() : count(0) {
        for (int i = 0; i < MAX_ITEMS; ++i) items[i] = nullptr;
    }

    ~Library() {
        // Release all allocated memory
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (items[i]) {
                delete items[i];
                items[i] = nullptr;
            }
        }
    }

    void addItem(LibraryItem* it) {
        if (count >= MAX_ITEMS) {
            cout << "Library is full. Cannot add more items.\n";
            delete it;
            return;
        }
        // find first null slot
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (items[i] == nullptr) {
                items[i] = it;
                count++;
                cout << "Item added successfully.\n";
                return;
            }
        }
    }

    void displayAll() const {
        bool any = false;
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (items[i]) {
                items[i]->displayDetails();
                any = true;
            }
        }
        if (!any) cout << "Library catalog is empty.\n";
    }

    LibraryItem* searchByTitle(const string &title) {
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (items[i] && items[i]->getTitle() == title) return items[i];
        }
        return nullptr;
    }

    void removeByTitle(const string &title) {
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (items[i] && items[i]->getTitle() == title) {
                delete items[i];
                items[i] = nullptr;
                count--;
                cout << "Item \"" << title << "\" removed from catalog.\n";
                return;
            }
        }
        cout << "Item not found.\n";
    }

    void checkOutItem(const string &title) {
        LibraryItem* it = searchByTitle(title);
        if (!it) {
            cout << "Item not found.\n";
            return;
        }
        try {
            it->checkOut();
        } catch (exception &e) {
            cout << "Error while checking out: " << e.what() << "\n";
        }
    }

    void returnItem(const string &title) {
        LibraryItem* it = searchByTitle(title);
        if (!it) {
            cout << "Item not found.\n";
            return;
        }
        try {
            it->returnItem();
        } catch (exception &e) {
            cout << "Error while returning item: " << e.what() << "\n";
        }
    }
};

// Menu helpers to create items interactively
void addBookInteractive(Library &lib) {
    try {
        string title, author, isbn;
        int copies;
        cout << "Enter book title: ";
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter ISBN (10 or 13 digits, hyphens allowed): ";
        getline(cin, isbn);
        cout << "Enter number of copies: ";
        if (!(cin >> copies)) {
            clearInput();
            throw invalid_argument("Invalid number for copies");
        }
        clearInput();
        // will validate in constructor
        Book* b = new Book(title, author, isbn, copies);
        lib.addItem(b);
    } catch (exception &e) {
        cout << "Failed to add book: " << e.what() << "\n";
    }
}

void addDVDInteractive(Library &lib) {
    try {
        string title, director, region;
        int duration;
        cout << "Enter DVD title: ";
        getline(cin, title);
        cout << "Enter director/author: ";
        getline(cin, director);
        cout << "Enter duration in minutes: ";
        if (!(cin >> duration)) {
            clearInput();
            throw invalid_argument("Invalid duration");
        }
        clearInput();
        cout << "Enter region code (optional): ";
        getline(cin, region);
        DVD* d = new DVD(title, director, duration, region);
        lib.addItem(d);
    } catch (exception &e) {
        cout << "Failed to add DVD: " << e.what() << "\n";
    }
}

void addMagazineInteractive(Library &lib) {
    try {
        string title, editor, month;
        int issue;
        cout << "Enter magazine title: ";
        getline(cin, title);
        cout << "Enter editor/author: ";
        getline(cin, editor);
        cout << "Enter issue number: ";
        if (!(cin >> issue)) {
            clearInput();
            throw invalid_argument("Invalid issue number");
        }
        clearInput();
        cout << "Enter month (e.g., September): ";
        getline(cin, month);
        Magazine* m = new Magazine(title, editor, issue, month);
        lib.addItem(m);
    } catch (exception &e) {
        cout << "Failed to add magazine: " << e.what() << "\n";
    }
}

void showMenu() {
    cout << "\n--- Library Menu ---\n";
    cout << "1. Add Book\n";
    cout << "2. Add DVD\n";
    cout << "3. Add Magazine\n";
    cout << "4. Display All Items\n";
    cout << "5. Search by Title\n";
    cout << "6. Check Out Item\n";
    cout << "7. Return Item\n";
    cout << "8. Remove Item by Title\n";
    cout << "9. Exit\n";
    cout << "Choice: ";
}

int main() {
    Library lib;
    bool running = true;

    while (running) {
        try {
            showMenu();
            int choice;
            if (!(cin >> choice)) {
                clearInput();
                cout << "Invalid input. Please enter a numeric choice.\n";
                continue;
            }
            clearInput();

            switch (choice) {
                case 1:
                    addBookInteractive(lib);
                    break;
                case 2:
                    addDVDInteractive(lib);
                    break;
                case 3:
                    addMagazineInteractive(lib);
                    break;
                case 4:
                    lib.displayAll();
                    break;
                case 5: {
                    string title;
                    cout << "Enter title to search: ";
                    getline(cin, title);
                    LibraryItem* found = lib.searchByTitle(title);
                    if (found) found->displayDetails();
                    else cout << "Item not found.\n";
                    break;
                }
                case 6: {
                    string title;
                    cout << "Enter title to check out: ";
                    getline(cin, title);
                    lib.checkOutItem(title);
                    break;
                }
                case 7: {
                    string title;
                    cout << "Enter title to return: ";
                    getline(cin, title);
                    lib.returnItem(title);
                    break;
                }
                case 8: {
                    string title;
                    cout << "Enter title to remove: ";
                    getline(cin, title);
                    lib.removeByTitle(title);
                    break;
                }
                case 9:
                    running = false;
                    cout << "Exiting. Releasing resources...\n";
                    break;
                default:
                    cout << "Invalid choice. Choose again.\n";
            }
        } catch (exception &e) {
            cout << "An error occurred: " << e.what() << "\n";
        }
    }

    // destructor will free memory
    return 0;
}
