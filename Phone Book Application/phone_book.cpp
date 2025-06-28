#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;

class Contact {
private:
    string name;
    string phone;
    string email;
    string address;

public:
    // Constructor
    Contact(string n, string p, string e = "", string a = "")
        : name(n), phone(p), email(e), address(a) {}

    // Getters
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }

    // Setters
    void setPhone(string p) { phone = p; }
    void setEmail(string e) { email = e; }
    void setAddress(string a) { address = a; }

    // Display contact
    void display() const {
        cout << "\n📋 Contact Details:" << endl;
        cout << "--------------------------------" << endl;
        cout << left << setw(15) << "Name:" << name << endl;
        cout << setw(15) << "Phone:" << phone << endl;
        if (!email.empty()) cout << setw(15) << "Email:" << email << endl;
        if (!address.empty()) cout << setw(15) << "Address:" << address << endl;
        cout << "--------------------------------" << endl;
    }
};

class PhoneBook {
private:
    vector<Contact> contacts;

public:
    // Add new contact
    void addContact() {
        string name, phone, email, address;
        
        cout << "\n➕ Add New Contact" << endl;
        cout << "-----------------" << endl;
        cout << "Enter name: ";
        getline(cin >> ws, name);
        
        cout << "Enter phone number: ";
        getline(cin, phone);
        
        cout << "Enter email (optional, press Enter to skip): ";
        getline(cin, email);
        
        cout << "Enter address (optional, press Enter to skip): ";
        getline(cin, address);
        
        contacts.emplace_back(name, phone, email, address);
        cout << "✅ Contact added successfully!" << endl;
    }

    // Display all contacts
    void displayAll() const {
        if (contacts.empty()) {
            cout << "\n📭 Phone book is empty!" << endl;
            return;
        }
        
        cout << "\n📖 All Contacts (" << contacts.size() << ")" << endl;
        cout << "========================================" << endl;
        for (const auto& contact : contacts) {
            contact.display();
        }
    }

    // Search contacts
    void searchContact() const {
        if (contacts.empty()) {
            cout << "\n📭 Phone book is empty!" << endl;
            return;
        }

        string term;
        cout << "\n🔍 Search Contacts" << endl;
        cout << "Enter name or phone number to search: ";
        getline(cin >> ws, term);

        bool found = false;
        for (const auto& contact : contacts) {
            if (contact.getName().find(term) != string::npos || 
                contact.getPhone().find(term) != string::npos) {
                contact.display();
                found = true;
            }
        }

        if (!found) {
            cout << "❌ No matching contacts found!" << endl;
        }
    }

    // Delete contact
    void deleteContact() {
        if (contacts.empty()) {
            cout << "\n📭 Phone book is empty!" << endl;
            return;
        }

        string term;
        cout << "\n🗑️ Delete Contact" << endl;
        cout << "Enter name or phone number to delete: ";
        getline(cin >> ws, term);

        auto it = remove_if(contacts.begin(), contacts.end(),
            [&term](const Contact& c) {
                return c.getName().find(term) != string::npos || 
                       c.getPhone().find(term) != string::npos;
            });

        if (it != contacts.end()) {
            contacts.erase(it, contacts.end());
            cout << "✅ Contact deleted successfully!" << endl;
        } else {
            cout << "❌ No matching contacts found!" << endl;
        }
    }

    // Save to file
    void saveToFile() const {
        ofstream outFile("phonebook.txt");
        if (outFile.is_open()) {
            for (const auto& contact : contacts) {
                outFile << contact.getName() << ","
                         << contact.getPhone() << ","
                         << contact.getEmail() << ","
                         << contact.getAddress() << "\n";
            }
            outFile.close();
            cout << "\n💾 Contacts saved to phonebook.txt" << endl;
        } else {
            cout << "\n❌ Error saving to file!" << endl;
        }
    }

    // Load from file
    void loadFromFile() {
        ifstream inFile("phonebook.txt");
        if (inFile.is_open()) {
            contacts.clear();
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1+1);
                size_t pos3 = line.find(',', pos2+1);
                
                if (pos1 != string::npos) {
                    string name = line.substr(0, pos1);
                    string phone = line.substr(pos1+1, pos2-pos1-1);
                    string email = line.substr(pos2+1, pos3-pos2-1);
                    string address = line.substr(pos3+1);
                    contacts.emplace_back(name, phone, email, address);
                }
            }
            inFile.close();
            cout << "\n📂 Contacts loaded from phonebook.txt" << endl;
        }
    }
};

// Main menu
void displayMenu() {
    cout << "\n📱 PHONE BOOK APPLICATION" << endl;
    cout << "=========================" << endl;
    cout << "1. Add New Contact" << endl;
    cout << "2. Display All Contacts" << endl;
    cout << "3. Search Contact" << endl;
    cout << "4. Delete Contact" << endl;
    cout << "5. Save Contacts to File" << endl;
    cout << "6. Load Contacts from File" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice (1-7): ";
}

int main() {
    PhoneBook phoneBook;
    int choice;
    
    // Load existing contacts at startup
    phoneBook.loadFromFile();

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: phoneBook.addContact(); break;
            case 2: phoneBook.displayAll(); break;
            case 3: phoneBook.searchContact(); break;
            case 4: phoneBook.deleteContact(); break;
            case 5: phoneBook.saveToFile(); break;
            case 6: phoneBook.loadFromFile(); break;
            case 7: cout << "\n👋 Goodbye!" << endl; break;
            default: cout << "❌ Invalid choice! Try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
