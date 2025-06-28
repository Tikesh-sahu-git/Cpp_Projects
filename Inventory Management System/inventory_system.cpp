#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int quantity;

public:
    Product(int i, string n, string c, double p, int q)
        : id(i), name(n), category(c), price(p), quantity(q) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void updateQuantity(int change) { quantity += change; }

    // Display product
    void display() const {
        cout << left << setw(5) << id 
             << setw(20) << name 
             << setw(15) << category
             << "$" << setw(10) << fixed << setprecision(2) << price
             << setw(10) << quantity << endl;
    }
};

class Inventory {
private:
    vector<Product> products;
    string filename = "inventory.txt";

    // Generate unique ID
    int generateId() {
        static int id = 1000;
        return id++;
    }

public:
    // Add new product
    void addProduct() {
        string name, category;
        double price;
        int quantity;

        cout << "\n➕ Add New Product" << endl;
        cout << "-----------------" << endl;
        cout << "Enter product name: ";
        getline(cin >> ws, name);
        
        cout << "Enter category: ";
        getline(cin, category);
        
        cout << "Enter price: $";
        cin >> price;
        
        cout << "Enter initial quantity: ";
        cin >> quantity;

        int id = generateId();
        products.emplace_back(id, name, category, price, quantity);
        cout << "✅ Product added successfully! (ID: " << id << ")" << endl;
    }

    // Display all products
    void displayAll() const {
        if (products.empty()) {
            cout << "\n📭 Inventory is empty!" << endl;
            return;
        }
        
        cout << "\n📋 Inventory List (" << products.size() << " products)" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(5) << "ID" 
             << setw(20) << "Name" 
             << setw(15) << "Category"
             << setw(11) << "Price"
             << setw(10) << "Qty" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (const auto& product : products) {
            product.display();
        }
        
        cout << "------------------------------------------------------------" << endl;
    }

    // Search products
    void searchProduct() const {
        if (products.empty()) {
            cout << "\n📭 Inventory is empty!" << endl;
            return;
        }

        string term;
        cout << "\n🔍 Search Product" << endl;
        cout << "Enter product name or ID to search: ";
        getline(cin >> ws, term);

        bool found = false;
        for (const auto& product : products) {
            if (to_string(product.getId()) == term || 
                product.getName().find(term) != string::npos) {
                cout << "\n📋 Product Details:" << endl;
                cout << "------------------------------------------------------------" << endl;
                cout << left << setw(5) << "ID" 
                     << setw(20) << "Name" 
                     << setw(15) << "Category"
                     << setw(11) << "Price"
                     << setw(10) << "Qty" << endl;
                cout << "------------------------------------------------------------" << endl;
                product.display();
                cout << "------------------------------------------------------------" << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "❌ No matching products found!" << endl;
        }
    }

    // Update stock
    void updateStock() {
        if (products.empty()) {
            cout << "\n📭 Inventory is empty!" << endl;
            return;
        }

        int id, change;
        cout << "\n🔄 Update Stock" << endl;
        cout << "Enter product ID: ";
        cin >> id;
        
        auto it = find_if(products.begin(), products.end(),
            [id](const Product& p) { return p.getId() == id; });

        if (it != products.end()) {
            cout << "Current quantity: " << it->getQuantity() << endl;
            cout << "Enter quantity change (+/-): ";
            cin >> change;
            
            int newQty = it->getQuantity() + change;
            if (newQty < 0) {
                cout << "❌ Error: Negative quantity not allowed!" << endl;
                return;
            }
            
            it->updateQuantity(change);
            cout << "✅ Stock updated! New quantity: " << newQty << endl;
        } else {
            cout << "❌ Product not found!" << endl;
        }
    }

    // Save to file
    void saveToFile() const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& product : products) {
                outFile << product.getId() << ","
                         << product.getName() << ","
                         << product.getCategory() << ","
                         << product.getPrice() << ","
                         << product.getQuantity() << "\n";
            }
            outFile.close();
            cout << "\n💾 Inventory saved to " << filename << endl;
        } else {
            cout << "\n❌ Error saving to file!" << endl;
        }
    }

    // Load from file
    void loadFromFile() {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            products.clear();
            int id, quantity;
            string name, category;
            double price;
            char comma;
            
            while (inFile >> id >> comma &&
                   getline(inFile, name, ',') &&
                   getline(inFile, category, ',') &&
                   inFile >> price >> comma >> quantity) {
                products.emplace_back(id, name, category, price, quantity);
            }
            inFile.close();
            cout << "\n📂 Inventory loaded from " << filename << endl;
        }
    }

    // Generate report
    void generateReport() const {
        if (products.empty()) {
            cout << "\n📭 Inventory is empty!" << endl;
            return;
        }

        double totalValue = 0;
        cout << "\n📊 Inventory Report" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(5) << "ID" 
             << setw(20) << "Name" 
             << setw(15) << "Category"
             << setw(11) << "Price"
             << setw(10) << "Qty"
             << setw(12) << "Value" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (const auto& product : products) {
            double value = product.getPrice() * product.getQuantity();
            totalValue += value;
            cout << left << setw(5) << product.getId() 
                 << setw(20) << product.getName() 
                 << setw(15) << product.getCategory()
                 << "$" << setw(10) << fixed << setprecision(2) << product.getPrice()
                 << setw(10) << product.getQuantity()
                 << "$" << setw(10) << value << endl;
        }
        
        cout << "------------------------------------------------------------" << endl;
        cout << "Total Inventory Value: $" << totalValue << endl;
        cout << "------------------------------------------------------------" << endl;
    }
};

// Main menu
void displayMenu() {
    cout << "\n📦 INVENTORY MANAGEMENT SYSTEM" << endl;
    cout << "==============================" << endl;
    cout << "1. Add New Product" << endl;
    cout << "2. Display All Products" << endl;
    cout << "3. Search Product" << endl;
    cout << "4. Update Stock" << endl;
    cout << "5. Generate Report" << endl;
    cout << "6. Save Inventory" << endl;
    cout << "7. Load Inventory" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice (1-8): ";
}

int main() {
    Inventory inventory;
    int choice;
    
    // Load existing inventory at startup
    inventory.loadFromFile();

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: inventory.addProduct(); break;
            case 2: inventory.displayAll(); break;
            case 3: inventory.searchProduct(); break;
            case 4: inventory.updateStock(); break;
            case 5: inventory.generateReport(); break;
            case 6: inventory.saveToFile(); break;
            case 7: inventory.loadFromFile(); break;
            case 8: cout << "\n👋 Goodbye!" << endl; break;
            default: cout << "❌ Invalid choice! Try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
