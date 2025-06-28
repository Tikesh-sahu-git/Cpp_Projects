#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

// Indian rupee symbol
const string INR = "₹";

class Car {
private:
    string licensePlate;
    string brand;
    string model;
    int year;
    double dailyRate; // in INR
    bool isAvailable;

public:
    Car(string lp, string b, string m, int y, double rate) 
        : licensePlate(lp), brand(b), model(m), year(y), dailyRate(rate), isAvailable(true) {}

    // Getters
    string getLicensePlate() const { return licensePlate; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    double getDailyRate() const { return dailyRate; }
    bool getAvailability() const { return isAvailable; }

    // Setters
    void setAvailability(bool available) { isAvailable = available; }

    // Display car details
    void display() const {
        cout << left << setw(15) << licensePlate 
             << setw(15) << brand
             << setw(15) << model
             << setw(10) << year
             << INR << setw(10) << fixed << setprecision(2) << dailyRate
             << setw(12) << (isAvailable ? "Available" : "Rented") << endl;
    }
};

class Customer {
private:
    string id;
    string name;
    string phone;
    string dlNumber; // Driving License Number

public:
    Customer(string i, string n, string p, string dl) 
        : id(i), name(n), phone(p), dlNumber(dl) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getDLNumber() const { return dlNumber; }

    // Display customer details
    void display() const {
        cout << left << setw(10) << id 
             << setw(25) << name
             << setw(15) << phone
             << setw(20) << dlNumber << endl;
    }
};

class Rental {
private:
    string rentalId;
    Car* car;
    Customer* customer;
    string rentalDate;
    string returnDate;
    double totalCost; // in INR

    // Calculate days between two dates (DD-MM-YYYY format)
    int calculateDays() {
        struct tm tm1 = {0}, tm2 = {0};
        strptime(rentalDate.c_str(), "%d-%m-%Y", &tm1);
        strptime(returnDate.c_str(), "%d-%m-%Y", &tm2);
        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);
        return difftime(time2, time1) / (60 * 60 * 24);
    }

public:
    Rental(string rid, Car* c, Customer* cust, string rDate, string retDate)
        : rentalId(rid), car(c), customer(cust), rentalDate(rDate), returnDate(retDate) {
            int days = calculateDays();
            totalCost = days * car->getDailyRate();
            car->setAvailability(false);
        }

    // Getters
    string getRentalId() const { return rentalId; }
    Car* getCar() const { return car; }
    Customer* getCustomer() const { return customer; }
    string getRentalDate() const { return rentalDate; }
    string getReturnDate() const { return returnDate; }
    double getTotalCost() const { return totalCost; }

    // Display rental details
    void display() const {
        cout << left << setw(15) << rentalId
             << setw(15) << car->getLicensePlate()
             << setw(25) << customer->getName()
             << setw(15) << rentalDate
             << setw(15) << returnDate
             << INR << setw(10) << fixed << setprecision(2) << totalCost << endl;
    }
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;
    string currentDate;

    // Get current date in DD-MM-YYYY format
    string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", ltm);
        return string(buffer);
    }

    // Validate date format (DD-MM-YYYY)
    bool isValidDate(const string& date) {
        if (date.length() != 10) return false;
        if (date[2] != '-' || date[5] != '-') return false;
        
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        
        if (year < 2025 || year > 2026) return false; // Only allow 2025-2026
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    }

public:
    CarRentalSystem() {
        currentDate = getCurrentDate();
        // Initialize with some sample data
        cars.push_back(Car("MH01AB1234", "Maruti", "Swift", 2023, 1500));
        cars.push_back(Car("KA02CD5678", "Hyundai", "Creta", 2024, 2500));
        cars.push_back(Car("DL03EF9012", "Tata", "Nexon EV", 2025, 3000));
        cars.push_back(Car("TN04GH3456", "Mahindra", "Thar", 2024, 3500));
        cars.push_back(Car("AP05IJ7890", "Kia", "Seltos", 2023, 2800));
    }

    // Add a new car
    void addCar() {
        string lp, brand, model;
        int year;
        double rate;

        cout << "\n🚗 Add New Car" << endl;
        cout << "--------------" << endl;
        cout << "Enter License Plate: ";
        getline(cin >> ws, lp);
        cout << "Enter Brand: ";
        getline(cin, brand);
        cout << "Enter Model: ";
        getline(cin, model);
        cout << "Enter Year: ";
        cin >> year;
        cout << "Enter Daily Rate (INR): ";
        cin >> rate;

        cars.push_back(Car(lp, brand, model, year, rate));
        cout << "\n✅ Car added successfully!" << endl;
    }

    // Register a new customer
    void registerCustomer() {
        string id, name, phone, dl;

        cout << "\n👤 Register New Customer" << endl;
        cout << "-----------------------" << endl;
        cout << "Enter Customer ID: ";
        getline(cin >> ws, id);
        cout << "Enter Full Name: ";
        getline(cin, name);
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        cout << "Enter Driving License Number: ";
        getline(cin, dl);

        customers.push_back(Customer(id, name, phone, dl));
        cout << "\n✅ Customer registered successfully!" << endl;
    }

    // Rent a car
    void rentCar() {
        if (cars.empty()) {
            cout << "\n❌ No cars available in the system!" << endl;
            return;
        }
        if (customers.empty()) {
            cout << "\n❌ No customers registered in the system!" << endl;
            return;
        }

        // Display available cars
        cout << "\nAvailable Cars:" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << left << setw(15) << "License" << setw(15) << "Brand" 
             << setw(15) << "Model" << setw(10) << "Year" 
             << setw(10) << "Rate/Day" << setw(12) << "Status" << endl;
        cout << "----------------------------------------------------------------" << endl;
        
        for (const auto& car : cars) {
            if (car.getAvailability()) {
                car.display();
            }
        }

        // Get car selection
        string licensePlate;
        cout << "\nEnter License Plate of car to rent: ";
        getline(cin >> ws, licensePlate);

        auto carIt = find_if(cars.begin(), cars.end(), 
            [licensePlate](const Car& c) { 
                return c.getLicensePlate() == licensePlate && c.getAvailability(); 
            });

        if (carIt == cars.end()) {
            cout << "❌ Invalid selection or car not available!" << endl;
            return;
        }

        // Display customers
        cout << "\nRegistered Customers:" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << left << setw(10) << "ID" << setw(25) << "Name" 
             << setw(15) << "Phone" << setw(20) << "DL Number" << endl;
        cout << "--------------------------------------------------------" << endl;
        
        for (const auto& customer : customers) {
            customer.display();
        }

        // Get customer selection
        string customerId;
        cout << "\nEnter Customer ID: ";
        getline(cin >> ws, customerId);

        auto custIt = find_if(customers.begin(), customers.end(), 
            [customerId](const Customer& c) { return c.getId() == customerId; });

        if (custIt == customers.end()) {
            cout << "❌ Invalid customer ID!" << endl;
            return;
        }

        // Get rental dates
        string rentalDate, returnDate;
        cout << "\nCurrent Date: " << currentDate << endl;
        cout << "Enter Rental Date (DD-MM-YYYY): ";
        getline(cin >> ws, rentalDate);
        cout << "Enter Return Date (DD-MM-YYYY): ";
        getline(cin >> ws, returnDate);

        if (!isValidDate(rentalDate) || !isValidDate(returnDate)) {
            cout << "❌ Invalid date format! Use DD-MM-YYYY and ensure year is 2025-2026." << endl;
            return;
        }

        // Create rental
        string rentalId = "RENT" + to_string(rentals.size() + 1001);
        rentals.emplace_back(rentalId, &(*carIt), &(*custIt), rentalDate, returnDate);
        
        cout << "\n✅ Car rented successfully!" << endl;
        cout << "Rental ID: " << rentalId << endl;
        cout << "Total Cost: " << INR << rentals.back().getTotalCost() << endl;
    }

    // Return a car
    void returnCar() {
        string rentalId;
        cout << "\nEnter Rental ID to return: ";
        getline(cin >> ws, rentalId);

        auto rentIt = find_if(rentals.begin(), rentals.end(), 
            [rentalId](const Rental& r) { return r.getRentalId() == rentalId; });

        if (rentIt == rentals.end()) {
            cout << "❌ Rental not found!" << endl;
            return;
        }

        rentIt->getCar()->setAvailability(true);
        cout << "\n✅ Car returned successfully!" << endl;
        cout << "License Plate: " << rentIt->getCar()->getLicensePlate() << endl;
        cout << "Total Charges: " << INR << rentIt->getTotalCost() << endl;
    }

    // Display all rentals
    void displayRentals() {
        if (rentals.empty()) {
            cout << "\n📭 No rental records found!" << endl;
            return;
        }

        cout << "\n📋 Rental Records" << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Rental ID" << setw(15) << "Car" 
             << setw(25) << "Customer" << setw(15) << "Rental Date" 
             << setw(15) << "Return Date" << setw(10) << "Cost" << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
        
        for (const auto& rental : rentals) {
            rental.display();
        }
    }

    // Display menu
    void displayMenu() {
        cout << "\n🚘 Car Rental System - 2025 (" << currentDate << ")" << endl;
        cout << "============================================" << endl;
        cout << "1. Add New Car" << endl;
        cout << "2. Register Customer" << endl;
        cout << "3. Rent a Car" << endl;
        cout << "4. Return a Car" << endl;
        cout << "5. View All Rentals" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
    }
};

int main() {
    CarRentalSystem system;
    int choice;

    do {
        system.displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: system.addCar(); break;
            case 2: system.registerCustomer(); break;
            case 3: system.rentCar(); break;
            case 4: system.returnCar(); break;
            case 5: system.displayRentals(); break;
            case 6: cout << "\n🚗 Thank you for using Car Rental System! 🚗" << endl; break;
            default: cout << "❌ Invalid choice! Please try again." << endl;
        }

        if (choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
    } while (choice != 6);

    return 0;
}
