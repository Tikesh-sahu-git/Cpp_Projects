#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <map>
using namespace std;

// Indian rupee symbol
const string INR = "₹";

class Car {
private:
    string licensePlate;
    string brand;
    string model;
    int year;
    double baseDailyRate; // in INR
    bool isAvailable;
    string currentLocation;
    double fuelLevel; // 0-100%
    bool hasDamage;

public:
    Car(string lp, string b, string m, int y, double rate, string loc) 
        : licensePlate(lp), brand(b), model(m), year(y), baseDailyRate(rate), 
          isAvailable(true), currentLocation(loc), fuelLevel(100.0), hasDamage(false) {}

    // Getters
    string getLicensePlate() const { return licensePlate; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    int getYear() const { return year; }
    double getBaseRate() const { return baseDailyRate; }
    bool getAvailability() const { return isAvailable; }
    string getLocation() const { return currentLocation; }
    double getFuelLevel() const { return fuelLevel; }
    bool hasDamageReport() const { return hasDamage; }

    // Setters
    void setAvailability(bool available) { isAvailable = available; }
    void setLocation(string loc) { currentLocation = loc; }
    void setFuelLevel(double level) { fuelLevel = level; }
    void setDamage(bool damage) { hasDamage = damage; }

    // Calculate current rate with location premium
    double getDailyRate() const {
        double rate = baseDailyRate;
        // Premium for metro cities
        if (currentLocation == "Delhi" || currentLocation == "Mumbai" || 
            currentLocation == "Bangalore") {
            rate *= 1.15; // 15% higher in metro cities
        }
        return rate;
    }

    // Display car details
    void display() const {
        cout << left << setw(15) << licensePlate 
             << setw(15) << brand
             << setw(15) << model
             << setw(10) << year
             << INR << setw(10) << fixed << setprecision(2) << getDailyRate()
             << setw(15) << currentLocation
             << setw(10) << fuelLevel << "%"
             << setw(12) << (isAvailable ? "Available" : "Rented")
             << setw(10) << (hasDamage ? "Damaged" : "OK") << endl;
    }
};

class Customer {
private:
    string id;
    string name;
    string phone;
    string dlNumber;
    int loyaltyPoints;
    vector<string> rentalHistory;

public:
    Customer(string i, string n, string p, string dl) 
        : id(i), name(n), phone(p), dlNumber(dl), loyaltyPoints(0) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getDLNumber() const { return dlNumber; }
    int getLoyaltyPoints() const { return loyaltyPoints; }

    // Add loyalty points
    void addLoyaltyPoints(int points) { 
        loyaltyPoints += points; 
        if (loyaltyPoints > 1000) loyaltyPoints = 1000; // Cap at 1000
    }

    // Get discount based on loyalty (5% per 200 points, max 25%)
    double getLoyaltyDiscount() const {
        int discountTier = loyaltyPoints / 200;
        return min(0.25, discountTier * 0.05); // 5% per tier, max 25%
    }

    // Add rental to history
    void addRental(string rentalId) {
        rentalHistory.push_back(rentalId);
    }

    // Display customer details
    void display() const {
        cout << left << setw(10) << id 
             << setw(25) << name
             << setw(15) << phone
             << setw(20) << dlNumber
             << setw(15) << loyaltyPoints
             << setw(10) << fixed << setprecision(0) << (getLoyaltyDiscount()*100) << "%" << endl;
    }
};

class Rental {
private:
    string rentalId;
    Car* car;
    Customer* customer;
    string rentalDate;
    string returnDate;
    bool insuranceSelected;
    bool isAdvanceBooking;
    double baseCost;
    double totalCost;
    string status; // "Confirmed", "Completed", "Cancelled"
    string damageReport;

    // Calculate days between two dates (DD-MM-YYYY format)
    int calculateDays() {
        struct tm tm1 = {0}, tm2 = {0};
        strptime(rentalDate.c_str(), "%d-%m-%Y", &tm1);
        strptime(returnDate.c_str(), "%d-%m-%Y", &tm2);
        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);
        return difftime(time2, time1) / (60 * 60 * 24);
    }

    // Calculate costs
    void calculateCosts() {
        int days = calculateDays();
        baseCost = days * car->getDailyRate();
        
        // Apply loyalty discount
        double discount = customer->getLoyaltyDiscount();
        double discountedAmount = baseCost * (1 - discount);
        
        // Add insurance if selected (10% of base cost)
        double insuranceCost = insuranceSelected ? baseCost * 0.10 : 0;
        
        // Add GST (18%)
        double gst = (discountedAmount + insuranceCost) * 0.18;
        
        totalCost = discountedAmount + insuranceCost + gst;
    }

public:
    Rental(string rid, Car* c, Customer* cust, string rDate, string retDate, 
           bool insurance, bool advance, string stat = "Confirmed")
        : rentalId(rid), car(c), customer(cust), rentalDate(rDate), returnDate(retDate),
          insuranceSelected(insurance), isAdvanceBooking(advance), status(stat), damageReport("None") {
            calculateCosts();
            if (status == "Confirmed") {
                car->setAvailability(false);
            }
            customer->addRental(rentalId);
        }

    // Getters
    string getRentalId() const { return rentalId; }
    Car* getCar() const { return car; }
    Customer* getCustomer() const { return customer; }
    string getRentalDate() const { return rentalDate; }
    string getReturnDate() const { return returnDate; }
    double getBaseCost() const { return baseCost; }
    double getTotalCost() const { return totalCost; }
    bool hasInsurance() const { return insuranceSelected; }
    bool isAdvanceBooking() const { return isAdvanceBooking; }
    string getStatus() const { return status; }
    string getDamageReport() const { return damageReport; }

    // Set damage report
    void setDamageReport(string report) { 
        damageReport = report; 
        car->setDamage(true);
    }

    // Complete rental
    void completeRental(double fuelLevel) {
        status = "Completed";
        car->setAvailability(true);
        car->setFuelLevel(fuelLevel);
        
        // Add loyalty points (10 points per day, min 1 day)
        int days = max(1, calculateDays());
        customer->addLoyaltyPoints(days * 10);
    }

    // Cancel rental
    void cancelRental() {
        status = "Cancelled";
        if (!isAdvanceBooking) {
            car->setAvailability(true);
        }
    }

    // Display rental details
    void display() const {
        cout << left << setw(15) << rentalId
             << setw(15) << car->getLicensePlate()
             << setw(25) << customer->getName()
             << setw(15) << rentalDate
             << setw(15) << returnDate
             << setw(10) << (insuranceSelected ? "Yes" : "No")
             << setw(10) << (isAdvanceBooking ? "Yes" : "No")
             << INR << setw(10) << fixed << setprecision(2) << totalCost
             << setw(12) << status << endl;
    }

    // Display detailed invoice
    void displayInvoice() const {
        int days = calculateDays();
        double discount = customer->getLoyaltyDiscount() * baseCost;
        
        cout << "\n📋 Rental Invoice: " << rentalId << endl;
        cout << "============================================" << endl;
        cout << "Customer: " << customer->getName() << endl;
        cout << "Car: " << car->getBrand() << " " << car->getModel() << endl;
        cout << "License Plate: " << car->getLicensePlate() << endl;
        cout << "Rental Period: " << rentalDate << " to " << returnDate << " (" << days << " days)" << endl;
        cout << "Pickup Location: " << car->getLocation() << endl;
        cout << "------------------------------------------------" << endl;
        cout << left << setw(30) << "Base Cost (" << days << " days @ " << INR << car->getDailyRate() << ")" 
             << INR << setw(10) << right << baseCost << endl;
        cout << left << setw(30) << "Loyalty Discount (" << (customer->getLoyaltyDiscount()*100) << "%)" 
             << INR << setw(10) << right << discount << endl;
        if (insuranceSelected) {
            cout << left << setw(30) << "Insurance Coverage" 
                 << INR << setw(10) << right << (baseCost * 0.10) << endl;
        }
        cout << left << setw(30) << "GST (18%)" 
             << INR << setw(10) << right << (totalCost - (baseCost - discount + (insuranceSelected ? baseCost * 0.10 : 0))) << endl;
        cout << "------------------------------------------------" << endl;
        cout << left << setw(30) << "TOTAL COST" 
             << INR << setw(10) << right << totalCost << endl;
        cout << "============================================" << endl;
        if (!damageReport.empty() && damageReport != "None") {
            cout << "Damage Report: " << damageReport << endl;
        }
    }
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;
    vector<string> locations = {"Delhi", "Mumbai", "Bangalore", "Hyderabad", "Chennai", "Kolkata"};
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
        
        if (year < 2025 || year > 2026) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    }

    // Check if date is in future
    bool isFutureDate(const string& date) {
        struct tm tm1 = {0}, tm2 = {0};
        strptime(currentDate.c_str(), "%d-%m-%Y", &tm1);
        strptime(date.c_str(), "%d-%m-%Y", &tm2);
        return difftime(mktime(&tm2), mktime(&tm1)) > 0;
    }

public:
    CarRentalSystem() {
        currentDate = getCurrentDate();
        // Initialize with sample data
        cars.push_back(Car("MH01AB1234", "Maruti", "Swift", 2023, 1500, "Mumbai"));
        cars.push_back(Car("KA02CD5678", "Hyundai", "Creta", 2024, 2500, "Bangalore"));
        cars.push_back(Car("DL03EF9012", "Tata", "Nexon EV", 2025, 3000, "Delhi"));
        cars.push_back(Car("TN04GH3456", "Mahindra", "Thar", 2024, 3500, "Chennai"));
        cars.push_back(Car("AP05IJ7890", "Kia", "Seltos", 2023, 2800, "Hyderabad"));
        
        customers.push_back(Customer("CUST1001", "Rahul Sharma", "9876543210", "DL1234567890"));
        customers.push_back(Customer("CUST1002", "Priya Patel", "8765432109", "MH9876543210"));
        customers.push_back(Customer("CUST1003", "Amit Singh", "7654321098", "KA5678901234"));
    }

    // Add a new car
    void addCar() {
        string lp, brand, model, loc;
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
        cout << "Enter Base Daily Rate (INR): ";
        cin >> rate;
        
        // Select location
        cout << "Select Location:" << endl;
        for (int i = 0; i < locations.size(); i++) {
            cout << i+1 << ". " << locations[i] << endl;
        }
        int locChoice;
        cout << "Enter choice (1-" << locations.size() << "): ";
        cin >> locChoice;
        loc = locations[locChoice-1];

        cars.push_back(Car(lp, brand, model, year, rate, loc));
        cout << "\n✅ Car added successfully to " << loc << " location!" << endl;
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

        // Select location
        cout << "\nSelect Location:" << endl;
        for (int i = 0; i < locations.size(); i++) {
            cout << i+1 << ". " << locations[i] << endl;
        }
        int locChoice;
        cout << "Enter choice (1-" << locations.size() << "): ";
        cin >> locChoice;
        string location = locations[locChoice-1];

        // Display available cars at selected location
        cout << "\nAvailable Cars at " << location << ":" << endl;
        cout << "------------------------------------------------------------------------------------------------" << endl;
        cout << left << setw(15) << "License" << setw(15) << "Brand" 
             << setw(15) << "Model" << setw(10) << "Year" 
             << setw(10) << "Rate/Day" << setw(15) << "Location"
             << setw(10) << "Fuel" << setw(12) << "Status"
             << setw(10) << "Condition" << endl;
        cout << "------------------------------------------------------------------------------------------------" << endl;
        
        for (const auto& car : cars) {
            if (car.getAvailability() && car.getLocation() == location) {
                car.display();
            }
        }

        // Get car selection
        string licensePlate;
        cout << "\nEnter License Plate of car to rent: ";
        getline(cin >> ws, licensePlate);

        auto carIt = find_if(cars.begin(), cars.end(), 
            [licensePlate, location](const Car& c) { 
                return c.getLicensePlate() == licensePlate && 
                       c.getAvailability() && 
                       c.getLocation() == location; 
            });

        if (carIt == cars.end()) {
            cout << "❌ Invalid selection or car not available at this location!" << endl;
            return;
        }

        // Display customers
        cout << "\nRegistered Customers:" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << left << setw(10) << "ID" << setw(25) << "Name" 
             << setw(15) << "Phone" << setw(20) << "DL Number"
             << setw(15) << "Loyalty Pts" << setw(10) << "Discount" << endl;
        cout << "----------------------------------------------------------------" << endl;
        
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

        // Check if advance booking
        bool isAdvance = isFutureDate(rentalDate);
        if (isAdvance) {
            cout << "⚠️ This is an advance booking (rental starts on " << rentalDate << ")" << endl;
        }

        // Insurance option
        char insuranceChoice;
        cout << "Add insurance coverage (10% of base cost)? (y/n): ";
        cin >> insuranceChoice;
        bool hasInsurance = (insuranceChoice == 'y' || insuranceChoice == 'Y');

        // Create rental
        string rentalId = "RENT" + to_string(rentals.size() + 1001);
        rentals.emplace_back(rentalId, &(*carIt), &(*custIt), rentalDate, returnDate, hasInsurance, isAdvance);
        
        cout << "\n✅ Car rented successfully!" << endl;
        cout << "Rental ID: " << rentalId << endl;
        rentals.back().displayInvoice();
    }

    // Return a car
    void returnCar() {
        string rentalId;
        cout << "\nEnter Rental ID to return: ";
        getline(cin >> ws, rentalId);

        auto rentIt = find_if(rentals.begin(), rentals.end(), 
            [rentalId](const Rental& r) { 
                return r.getRentalId() == rentalId && r.getStatus() == "Confirmed"; 
            });

        if (rentIt == rentals.end()) {
            cout << "❌ Rental not found or already completed/cancelled!" << endl;
            return;
        }

        // Fuel level check
        double fuelLevel;
        cout << "Enter current fuel level (0-100%): ";
        cin >> fuelLevel;
        while (fuelLevel < 0 || fuelLevel > 100) {
            cout << "Invalid fuel level! Enter value between 0-100: ";
            cin >> fuelLevel;
        }

        // Damage report
        cin.ignore();
        string damageReport;
        cout << "Enter damage report (if any, or 'none'): ";
        getline(cin, damageReport);
        if (damageReport != "none" && !damageReport.empty()) {
            rentIt->setDamageReport(damageReport);
        }

        // Complete rental
        rentIt->completeRental(fuelLevel);
        
        cout << "\n✅ Car returned successfully!" << endl;
        rentIt->displayInvoice();
        
        // Apply fuel charge if needed
        if (fuelLevel < 80) {
            double fuelCharge = (80 - fuelLevel) * 100; // ₹100 per % below 80%
            cout << "Fuel Refill Charge: " << INR << fuelCharge << endl;
            cout << "Final Amount Due: " << INR << (rentIt->getTotalCost() + fuelCharge) << endl;
        }
    }

    // Display all rentals
    void displayRentals() {
        if (rentals.empty()) {
            cout << "\n📭 No rental records found!" << endl;
            return;
        }

        cout << "\n📋 Rental Records" << endl;
        cout << "-------------------------------------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Rental ID" << setw(15) << "Car" 
             << setw(25) << "Customer" << setw(15) << "Rental Date" 
             << setw(15) << "Return Date" << setw(10) << "Insurance"
             << setw(10) << "Advance" << setw(10) << "Cost"
             << setw(12) << "Status" << endl;
        cout << "-------------------------------------------------------------------------------------------" << endl;
        
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
