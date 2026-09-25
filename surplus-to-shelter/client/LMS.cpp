#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

string toLowerString(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

class BookRecord {
public:
    int bookCode;
    string bookTitle;
    string authorName;
    bool isIssued;
    int issuedToId;

    BookRecord(int code, string title, string author) {
        bookCode = code;
        bookTitle = title;
        authorName = author;
        isIssued = false;
        issuedToId = -1;
    }
};

class MemberRecord {
public:
    int memberCode;
    string memberName;

    MemberRecord(int code, string name) {
        memberCode = code;
        memberName = name;
    }
};

class LibraryManager {
private:
    vector<BookRecord> bookInventory;
    vector<MemberRecord> memberDirectory;

public:
    void addNewBook() {
        int code;
        string title, author;

        cout << "\nEnter Book Code: ";
        cin >> code;
        cin.ignore();

        for (const auto &item : bookInventory) {
            if (item.bookCode == code) {
                cout << "Error: Book Code already exists in system.\n";
                return;
            }
        }

        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);

        bookInventory.emplace_back(code, title, author);
        cout << "Book registered successfully.\n";
    }

    void addNewMember() {
        int code;
        string name;

        cout << "\nEnter Member Code: ";
        cin >> code;
        cin.ignore();

        for (const auto &member : memberDirectory) {
            if (member.memberCode == code) {
                cout << "Error: Member Code already registered.\n";
                return;
            }
        }

        cout << "Enter Member Full Name: ";
        getline(cin, name);

        memberDirectory.emplace_back(code, name);
        cout << "Member registered successfully.\n";
    }

    void displayAllBooks() {
        if (bookInventory.empty()) {
            cout << "\nNo books available in inventory.\n";
            return;
        }

        cout << "\n" << string(75, '=') << "\n";
        cout << left << setw(12) << "Book Code" 
             << setw(30) << "Title" 
             << setw(22) << "Author" 
             << setw(10) << "Status" << "\n";
        cout << string(75, '-') << "\n";

        for (const auto &item : bookInventory) {
            string status = item.isIssued ? "Issued" : "Available";
            cout << left << setw(12) << item.bookCode 
                 << setw(30) << item.bookTitle 
                 << setw(22) << item.authorName 
                 << setw(10) << status << "\n";
        }
        cout << string(75, '=') << "\n";
    }

    void displayAllMembers() {
        if (memberDirectory.empty()) {
            cout << "\nNo registered members found.\n";
            return;
        }

        cout << "\n" << string(45, '=') << "\n";
        cout << left << setw(15) << "Member Code" 
             << setw(30) << "Full Name" << "\n";
        cout << string(45, '-') << "\n";

        for (const auto &member : memberDirectory) {
            cout << left << setw(15) << member.memberCode 
                 << setw(30) << member.memberName << "\n";
        }
        cout << string(45, '=') << "\n";
    }

    void searchBooks() {
        if (bookInventory.empty()) {
            cout << "\nInventory is currently empty.\n";
            return;
        }

        int searchType;
        cout << "\n--- Search Menu ---";
        cout << "\n1. Search by Title";
        cout << "\n2. Search by Author";
        cout << "\n3. Search by Book Code";
        cout << "\nEnter choice: ";
        cin >> searchType;
        cin.ignore();

        bool matchFound = false;

        if (searchType == 1 || searchType == 2) {
            string keyword;
            cout << "Enter search keyword: ";
            getline(cin, keyword);
            keyword = toLowerString(keyword);

            cout << "\n========== Search Results ==========\n";
            for (const auto &item : bookInventory) {
                string target = (searchType == 1) ? toLowerString(item.bookTitle) : toLowerString(item.authorName);
                if (target.find(keyword) != string::npos) {
                    cout << "Code: " << item.bookCode 
                         << " | Title: " << item.bookTitle 
                         << " | Author: " << item.authorName 
                         << " | Status: " << (item.isIssued ? "Issued" : "Available") << "\n";
                    matchFound = true;
                }
            }
        } else if (searchType == 3) {
            int targetCode;
            cout << "Enter Book Code: ";
            cin >> targetCode;

            for (const auto &item : bookInventory) {
                if (item.bookCode == targetCode) {
                    cout << "\n[Match Found] Code: " << item.bookCode 
                         << " | Title: " << item.bookTitle 
                         << " | Author: " << item.authorName 
                         << " | Status: " << (item.isIssued ? "Issued" : "Available") << "\n";
                    matchFound = true;
                    break;
                }
            }
        } else {
            cout << "Invalid selection.\n";
            return;
        }

        if (!matchFound) {
            cout << "No matching records found.\n";
        }
    }

    void issueBook() {
        int bCode, mCode;
        cout << "\nEnter Book Code to issue: ";
        cin >> bCode;

        BookRecord *targetBook = nullptr;
        for (auto &item : bookInventory) {
            if (item.bookCode == bCode) {
                targetBook = &item;
                break;
            }
        }

        if (!targetBook) {
            cout << "Error: Book not found in records.\n";
            return;
        }

        if (targetBook->isIssued) {
            cout << "Error: Book is already checked out.\n";
            return;
        }

        cout << "Enter Member Code: ";
        cin >> mCode;

        bool memberExists = false;
        for (const auto &member : memberDirectory) {
            if (member.memberCode == mCode) {
                memberExists = true;
                break;
            }
        }

        if (!memberExists) {
            cout << "Error: Invalid Member Code.\n";
            return;
        }

        targetBook->isIssued = true;
        targetBook->issuedToId = mCode;
        cout << "Book issued successfully.\n";
    }

    void returnBook() {
        int bCode;
        cout << "\nEnter Book Code to return: ";
        cin >> bCode;

        for (auto &item : bookInventory) {
            if (item.bookCode == bCode) {
                if (!item.isIssued) {
                    cout << "Notice: Book is already marked as available.\n";
                    return;
                }
                item.isIssued = false;
                item.issuedToId = -1;
                cout << "Book returned successfully.\n";
                return;
            }
        }
        cout << "Error: Book Code not found.\n";
    }

    void displayIssuedBooks() {
        bool hasIssued = false;
        cout << "\n============= ACTIVE ISSUANCE RECORDS =============\n";

        for (const auto &item : bookInventory) {
            if (item.isIssued) {
                string borrower = "Unknown Member";
                for (const auto &member : memberDirectory) {
                    if (member.memberCode == item.issuedToId) {
                        borrower = member.memberName;
                        break;
                    }
                }
                cout << "Book Code: " << item.bookCode 
                     << " | Title: " << item.bookTitle 
                     << " | Issued To: " << borrower 
                     << " (Member ID: " << item.issuedToId << ")\n";
                hasIssued = true;
            }
        }

        if (!hasIssued) {
            cout << "No books are currently issued.\n";
        }
    }

    void deleteBook() {
        int bCode;
        cout << "\nEnter Book Code to remove: ";
        cin >> bCode;

        for (auto it = bookInventory.begin(); it != bookInventory.end(); ++it) {
            if (it->bookCode == bCode) {
                if (it->isIssued) {
                    cout << "Error: Cannot delete a book that is currently issued.\n";
                    return;
                }
                bookInventory.erase(it);
                cout << "Book record successfully deleted.\n";
                return;
            }
        }
        cout << "Error: Book Code not found.\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n==========================================";
            cout << "\n         LIBRARY MANAGEMENT SYSTEM        ";
            cout << "\n==========================================";
            cout << "\n 1. Add New Book";
            cout << "\n 2. Add New Member";
            cout << "\n 3. Display All Books";
            cout << "\n 4. Display All Members";
            cout << "\n 5. Search Book";
            cout << "\n 6. Issue Book";
            cout << "\n 7. Return Book";
            cout << "\n 8. View Issued Records";
            cout << "\n 9. Delete Book";
            cout << "\n 10. Exit";
            cout << "\n==========================================";
            cout << "\nEnter selection (1-10): ";
            cin >> choice;

            switch (choice) {
                case 1: addNewBook(); break;
                case 2: addNewMember(); break;
                case 3: displayAllBooks(); break;
                case 4: displayAllMembers(); break;
                case 5: searchBooks(); break;
                case 6: issueBook(); break;
                case 7: returnBook(); break;
                case 8: displayIssuedBooks(); break;
                case 9: deleteBook(); break;
                case 10: 
                    cout << "\nExiting application. Goodbye!\n"; 
                    break;
                default: 
                    cout << "\nInvalid selection. Please try again.\n";
            }
        } while (choice != 10);
    }
};

int main() {
    LibraryManager manager;
    manager.run();
    return 0;
}