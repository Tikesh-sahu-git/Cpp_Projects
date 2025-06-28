#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <map>
#include <sstream>
#include <queue>
#include <unordered_map>
using namespace std;

// ------------------- Utility Functions -------------------
string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ltm);
    return string(buffer);
}

// ------------------- Classes -------------------
class User {
private:
    string username;
    string password;
    string name;
    string phone;
    string email;
    bool isAdmin;

public:
    User(string uname, string pwd, string n, string ph, string em, bool admin = false)
        : username(uname), password(pwd), name(n), phone(ph), email(em), isAdmin(admin) {}

    // Getters
    string getUsername() const { return username; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    bool isAdministrator() const { return isAdmin; }

    // Authentication
    bool authenticate(string pwd) const { return password == pwd; }

    // Display user info
    void display() const {
        cout << left << setw(15) << username 
             << setw(20) << name
             << setw(15) << phone
             << setw(25) << email
             << (isAdmin ? "Admin" : "Customer") << endl;
    }
};

class Passenger {
private:
    string name;
    int age;
    char gender; // M/F/O
    string berthPreference; // Lower/Middle/Upper/Side
    string concessionType; // None/Senior/Student/Child
    string coach;
    int seatNumber;

public:
    Passenger(string n, int a, char g, string bp, string concession = "None") 
        : name(n), age(a), gender(g), berthPreference(bp), 
          concessionType(concession), coach(""), seatNumber(0) {}

    // Getters
    string getName() const { return name; }
    int getAge() const { return age; }
    char getGender() const { return gender; }
    string getBerthPreference() const { return berthPreference; }
    string getConcessionType() const { return concessionType; }
    string getCoach() const { return coach; }
    int getSeatNumber() const { return seatNumber; }

    // Setters
    void setCoach(string c) { coach = c; }
    void setSeatNumber(int sn) { seatNumber = sn; }
    void setConcession(string concession) { concessionType = concession; }

    // Display passenger details
    void display() const {
        cout << left << setw(20) << name 
             << setw(5) << age
             << setw(8) << gender
             << setw(10) << berthPreference
             << setw(10) << concessionType
             << setw(8) << coach
             << setw(5) << seatNumber << endl;
    }
};

class Train {
private:
    string trainNo;
    string name;
    string source;
    string destination;
    vector<string> stations;
    string departureTime;
    string arrivalTime;
    map<string, int> availableSeats;
    map<string, double> baseFares;
    map<string, queue<int>> waitingLists; // key: coach type, value: WL count
    bool isTatkalAvailable;

    // Tatkal timing check (10AM-12PM previous day)
    bool isTatkalTime(string date) {
        // Convert date to previous day
        // (In real implementation, use proper date manipulation)
        return true; // Simplified for demo
    }

public:
    Train(string no, string n, string src, string dest, vector<string> stns, 
          string dep, string arr, map<string, int> seats, map<string, double> fares)
        : trainNo(no), name(n), source(src), destination(dest), stations(stns),
          departureTime(dep), arrivalTime(arr), availableSeats(seats), baseFares(fares),
          isTatkalAvailable(false) {
        // Initialize waiting lists
        for (auto& seat : availableSeats) {
            waitingLists[seat.first] = queue<int>();
        }
    }

    // Getters
    string getTrainNo() const { return trainNo; }
    string getName() const { return name; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    vector<string> getStations() const { return stations; }
    string getDepartureTime() const { return departureTime; }
    string getArrivalTime() const { return arrivalTime; }
    map<string, int> getAvailableSeats() const { return availableSeats; }
    bool getTatkalStatus() const { return isTatkalAvailable; }

    // Get fare with concession and tatkal premium
    double getFare(string coachType, string concession = "None", bool isTatkal = false) {
        double fare = baseFares.at(coachType);
        
        // Apply concession
        if (concession == "Senior" || concession == "Student") {
            fare *= 0.75; // 25% discount
        } else if (concession == "Child") {
            fare *= 0.5; // 50% discount
        }
        
        // Apply Tatkal charges (30% premium)
        if (isTatkal) {
            fare *= 1.3;
        }
        
        return fare;
    }

    // Check station in route
    bool hasStation(string station) const {
        return find(stations.begin(), stations.end(), station) != stations.end();
    }

    // Book seats
    pair<bool, int> bookSeats(string coachType, int numSeats, bool isTatkal = false) {
        if (availableSeats[coachType] >= numSeats) {
            availableSeats[coachType] -= numSeats;
            return {true, 0}; // Confirmed
        } else {
            // Add to waiting list
            int wlNumber = waitingLists[coachType].size() + 1;
            for (int i = 0; i < numSeats; i++) {
                waitingLists[coachType].push(wlNumber);
            }
            return {false, wlNumber};
        }
    }

    // Cancel booking
    void cancelSeats(string coachType, int numSeats) {
        availableSeats[coachType] += numSeats;
        
        // Promote from waiting list if available
        while (!waitingLists[coachType].empty() && numSeats > 0) {
            waitingLists[coachType].pop();
            numSeats--;
        }
    }

    // Display train details
    void display() const {
        cout << "\n🚂 " << name << " (" << trainNo << ")" << endl;
        cout << "------------------------------------------------" << endl;
        cout << left << setw(15) << "From:" << source << " (" << departureTime << ")" << endl;
        cout << setw(15) << "To:" << destination << " (" << arrivalTime << ")" << endl;
        
        cout << "\nRoute: ";
        for (const auto& station : stations) {
            cout << station;
            if (station != stations.back()) cout << " → ";
        }
        
        cout << "\n\nAvailable Seats:" << endl;
        cout << "------------------------------------------------" << endl;
        cout << left << setw(15) << "Coach Type" << setw(10) << "Seats" << setw(10) << "WL" << setw(10) << "Base Fare" << endl;
        cout << "------------------------------------------------" << endl;
        for (const auto& seat : availableSeats) {
            cout << setw(15) << seat.first 
                 << setw(10) << seat.second
                 << setw(10) << waitingLists.at(seat.first).size()
                 << "₹" << baseFares.at(seat.first) << endl;
        }
        cout << "------------------------------------------------" << endl;
        if (isTatkalAvailable) {
            cout << "⚠️ Tatkal booking available (30% premium)" << endl;
        }
    }

    // Enable Tatkal booking
    void enableTatkal(string date) {
        isTatkalAvailable = isTatkalTime(date);
    }
};

class Ticket {
private:
    string pnr;
    Train* train;
    string date;
    string fromStation;
    string toStation;
    vector<Passenger> passengers;
    string coachType;
    double totalFare;
    string status; // Confirmed/Waiting/Cancelled
    bool isTatkal;
    string paymentId;
    string bookingTime;

    // Generate PNR
    string generatePNR() {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string pnr;
        for (int i = 0; i < 10; ++i) {
            pnr += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        return pnr;
    }

    // Generate payment ID
    string generatePaymentId() {
        return "PAY" + to_string(rand() % 900000 + 100000);
    }

public:
    Ticket(Train* t, string d, string from, string to, vector<Passenger> p, 
           string ct, bool tatkal = false)
        : train(t), date(d), fromStation(from), toStation(to), passengers(p), 
          coachType(ct), isTatkal(tatkal) {
            pnr = generatePNR();
            paymentId = generatePaymentId();
            bookingTime = getCurrentDateTime();
            
            // Calculate total fare with concessions
            totalFare = 0;
            for (const auto& passenger : passengers) {
                totalFare += train->getFare(coachType, passenger.getConcessionType(), isTatkal);
            }
            
            // Book seats
            auto bookingStatus = train->bookSeats(coachType, passengers.size(), isTatkal);
            status = bookingStatus.first ? "Confirmed" : "WL" + to_string(bookingStatus.second);
            
            // Assign seat numbers if confirmed
            if (status == "Confirmed") {
                int startSeat = (rand() % 50) + 1;
                string coachPrefix = coachType.substr(0, 1) + to_string(rand() % 10 + 1);
                
                for (size_t i = 0; i < passengers.size(); i++) {
                    passengers[i].setCoach(coachPrefix);
                    passengers[i].setSeatNumber(startSeat + i);
                }
            }
        }

    // Getters
    string getPNR() const { return pnr; }
    Train* getTrain() const { return train; }
    string getDate() const { return date; }
    string getFromStation() const { return fromStation; }
    string getToStation() const { return toStation; }
    vector<Passenger> getPassengers() const { return passengers; }
    string getCoachType() const { return coachType; }
    double getTotalFare() const { return totalFare; }
    string getStatus() const { return status; }
    bool isTatkalBooking() const { return isTatkal; }
    string getPaymentId() const { return paymentId; }
    string getBookingTime() const { return bookingTime; }

    // Cancel ticket
    void cancel() {
        if (status.substr(0, 2) != "WL") { // If not waiting list
            train->cancelSeats(coachType, passengers.size());
        }
        status = "Cancelled";
    }

    // Display ticket details
    void display() const {
        cout << "\n🎫 Ticket PNR: " << pnr << " (" << status << ")" << endl;
        cout << "Booking Time: " << bookingTime << endl;
        cout << "Payment ID: " << paymentId << endl;
        cout << "============================================" << endl;
        cout << "Train: " << train->getName() << " (" << train->getTrainNo() << ")" << endl;
        cout << "Date: " << date << endl;
        cout << "From: " << fromStation << " (" << train->getDepartureTime() << ")" << endl;
        cout << "To: " << toStation << " (" << train->getArrivalTime() << ")" << endl;
        cout << "Coach: " << coachType << (isTatkal ? " (Tatkal)" : "") << endl;
        cout << "Total Fare: ₹" << totalFare << endl;
        
        cout << "\nPassenger Details:" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Name" << setw(5) << "Age" 
             << setw(8) << "Gender" << setw(10) << "Berth" 
             << setw(10) << "Concession" << setw(8) << "Coach" 
             << setw(5) << "Seat" << endl;
        cout << "------------------------------------------------------------" << endl;
        for (const auto& passenger : passengers) {
            passenger.display();
        }
        cout << "============================================" << endl;
    }

    // Send email/SMS confirmation
    void sendConfirmation(const User& user) const {
        // In a real system, this would connect to an email/SMS service
        cout << "\n✉️ Confirmation sent to " << user.getEmail() << " and SMS to " << user.getPhone() << endl;
        cout << "PNR: " << pnr << " | Train: " << train->getName() << " (" << train->getTrainNo() << ")" << endl;
        cout << "From: " << fromStation << " To: " << toStation << " on " << date << endl;
        cout << "Status: " << status << " | Total Fare: ₹" << totalFare << endl;
    }
};

class RailwayReservationSystem {
private:
    vector<Train> trains;
    vector<Ticket> tickets;
    unordered_map<string, User> users;
    User* currentUser;

    // Initialize with sample data
    void initializeData() {
        // Sample trains
        vector<string> rajdhaniStations = {"Delhi", "Jaipur", "Ahmedabad", "Mumbai"};
        map<string, int> rajdhaniSeats = {{"1A", 20}, {"2A", 50}, {"3A", 100}, {"SL", 200}};
        map<string, double> rajdhaniFares = {{"1A", 3500}, {"2A", 2500}, {"3A", 1500}, {"SL", 800}};
        trains.emplace_back("12301", "Rajdhani Express", "Delhi", "Mumbai", 
                          rajdhaniStations, "16:30", "08:00", rajdhaniSeats, rajdhaniFares);

        vector<string> shatabdiStations = {"Chennai", "Bangalore", "Hyderabad"};
        map<string, int> shatabdiSeats = {{"CC", 100}, {"EC", 50}};
        map<string, double> shatabdiFares = {{"CC", 1200}, {"EC", 2000}};
        trains.emplace_back("12007", "Shatabdi Express", "Chennai", "Hyderabad", 
                          shatabdiStations, "06:00", "14:30", shatabdiSeats, shatabdiFares);

        // Sample users
        users["admin"] = User("admin", "admin123", "Admin User", "9876543210", "admin@irctc.com", true);
        users["rahul"] = User("rahul", "pass123", "Rahul Sharma", "9876543211", "rahul@example.com");
    }

    // Find train by number
    Train* findTrain(string trainNo) {
        for (auto& train : trains) {
            if (train.getTrainNo() == trainNo) {
                return &train;
            }
        }
        return nullptr;
    }

    // Find ticket by PNR
    Ticket* findTicket(string pnr) {
        for (auto& ticket : tickets) {
            if (ticket.getPNR() == pnr) {
                return &ticket;
            }
        }
        return nullptr;
    }

    // Display available trains between stations
    void displayAvailableTrains(string from, string to, string date) {
        cout << "\nAvailable Trains from " << from << " to " << to << " on " << date << ":" << endl;
        cout << "==================================================================" << endl;
        bool found = false;
        
        for (auto& train : trains) {
            if (train.hasStation(from) && train.hasStation(to)) {
                found = true;
                // Enable Tatkal if applicable
                train.enableTatkal(date);
                train.display();
            }
        }
        
        if (!found) {
            cout << "No trains found for this route." << endl;
        }
    }

    // User authentication
    bool authenticateUser() {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        cin.ignore();

        if (users.find(username) != users.end() && users[username].authenticate(password)) {
            currentUser = &users[username];
            return true;
        }
        return false;
    }

    // User registration
    void registerUser() {
        string username, password, name, phone, email;
        
        cout << "\n👤 New User Registration" << endl;
        cout << "-----------------------" << endl;
        cout << "Username: ";
        cin >> username;
        
        if (users.find(username) != users.end()) {
            cout << "❌ Username already exists!" << endl;
            return;
        }
        
        cout << "Password: ";
        cin >> password;
        cout << "Full Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Phone Number: ";
        cin >> phone;
        cout << "Email: ";
        cin >> email;
        
        users[username] = User(username, password, name, phone, email);
        currentUser = &users[username];
        cout << "\n✅ Registration successful! You are now logged in." << endl;
    }

public:
    RailwayReservationSystem() : currentUser(nullptr) {
        initializeData();
    }

    // Login menu
    bool showLoginMenu() {
        int choice;
        do {
            cout << "\n🚉 Indian Railway Reservation System" << endl;
            cout << "============================================" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice (1-3): ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: 
                    if (authenticateUser()) {
                        cout << "\n✅ Login successful! Welcome, " << currentUser->getName() << endl;
                        return true;
                    } else {
                        cout << "❌ Invalid username or password!" << endl;
                    }
                    break;
                case 2:
                    registerUser();
                    if (currentUser != nullptr) return true;
                    break;
                case 3:
                    return false;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
        } while (true);
    }

    // Book ticket
    void bookTicket() {
        string from, to, date, trainNo, coachType;
        
        // Get journey details
        cout << "\n🚉 Enter Journey Details" << endl;
        cout << "From Station: ";
        getline(cin >> ws, from);
        cout << "To Station: ";
        getline(cin, to);
        cout << "Date of Journey (DD-MM-YYYY): ";
        getline(cin, date);
        
        // Show available trains
        displayAvailableTrains(from, to, date);
        
        // Select train
        cout << "\nEnter Train Number: ";
        getline(cin, trainNo);
        Train* selectedTrain = findTrain(trainNo);
        if (!selectedTrain) {
            cout << "❌ Invalid train number!" << endl;
            return;
        }
        
        // Check if stations are valid
        if (!selectedTrain->hasStation(from) || !selectedTrain->hasStation(to)) {
            cout << "❌ Selected stations not on this train's route!" << endl;
            return;
        }
        
        // Select coach type
        cout << "\nAvailable Coach Types:" << endl;
        for (const auto& seat : selectedTrain->getAvailableSeats()) {
            cout << seat.first << " (₹" << selectedTrain->getFare(seat.first) << ") ";
        }
        cout << "\nEnter Coach Type: ";
        getline(cin, coachType);
        
        // Check coach type
        if (selectedTrain->getAvailableSeats().find(coachType) == selectedTrain->getAvailableSeats().end()) {
            cout << "❌ Invalid coach type!" << endl;
            return;
        }
        
        // Tatkal booking option
        bool isTatkal = false;
        if (selectedTrain->getTatkalStatus()) {
            char tatkalChoice;
            cout << "Tatkal booking available (30% premium). Book Tatkal? (y/n): ";
            cin >> tatkalChoice;
            isTatkal = (tatkalChoice == 'y' || tatkalChoice == 'Y');
            cin.ignore();
        }
        
        // Get passenger details
        int numPassengers;
        cout << "Number of Passengers: ";
        cin >> numPassengers;
        cin.ignore();
        
        vector<Passenger> passengers;
        for (int i = 0; i < numPassengers; i++) {
            string name, berthPref, concession;
            int age;
            char gender;
            
            cout << "\nPassenger " << i+1 << " Details:" << endl;
            cout << "Name: ";
            getline(cin >> ws, name);
            cout << "Age: ";
            cin >> age;
            cout << "Gender (M/F/O): ";
            cin >> gender;
            gender = toupper(gender);
            cout << "Berth Preference (Lower/Middle/Upper/Side): ";
            cin.ignore();
            getline(cin, berthPref);
            
            // Concession option
            if (age >= 60) {
                concession = "Senior";
            } else if (age < 5) {
                concession = "Child";
            } else {
                cout << "Concession Type (None/Student): ";
                getline(cin, concession);
            }
            
            passengers.emplace_back(name, age, gender, berthPref, concession);
        }
        
        // Create ticket
        tickets.emplace_back(selectedTrain, date, from, to, passengers, coachType, isTatkal);
        
        // Display ticket and send confirmation
        cout << "\n✅ Ticket Booked Successfully!" << endl;
        tickets.back().display();
        tickets.back().sendConfirmation(*currentUser);
    }

    // Cancel ticket
    void cancelTicket() {
        string pnr;
        cout << "\nEnter PNR Number to cancel: ";
        getline(cin >> ws, pnr);
        
        Ticket* ticket = findTicket(pnr);
        if (!ticket) {
            cout << "❌ Ticket not found!" << endl;
            return;
        }
        
        ticket->cancel();
        cout << "\n✅ Ticket cancelled successfully!" << endl;
        ticket->display();
        
        // Send cancellation confirmation
        cout << "\n✉️ Cancellation confirmation sent to " << currentUser->getEmail() << endl;
    }

    // Check PNR status
    void checkPNRStatus() {
        string pnr;
        cout << "\nEnter PNR Number to check: ";
        getline(cin >> ws, pnr);
        
        Ticket* ticket = findTicket(pnr);
        if (!ticket) {
            cout << "❌ Ticket not found!" << endl;
            return;
        }
        
        ticket->display();
    }

    // Display train schedule
    void displayTrainSchedule() {
        cout << "\n🚂 Train Schedule" << endl;
        cout << "==================================================================" << endl;
        for (const auto& train : trains) {
            train.display();
        }
    }

    // Admin functions
    void adminPanel() {
        if (!currentUser || !currentUser->isAdministrator()) {
            cout << "❌ Access denied!" << endl;
            return;
        }

        int choice;
        do {
            cout << "\n🛠️ Admin Panel" << endl;
            cout << "============================================" << endl;
            cout << "1. Add New Train" << endl;
            cout << "2. View All Users" << endl;
            cout << "3. Back to Main Menu" << endl;
            cout << "Enter your choice (1-3): ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    addNewTrain();
                    break;
                case 2:
                    viewAllUsers();
                    break;
                case 3:
                    return;
                default:
                    cout << "❌ Invalid choice!" << endl;
            }
        } while (true);
    }

    // Add new train (admin only)
    void addNewTrain() {
        string no, name, src, dest, dep, arr;
        vector<string> stations;
        map<string, int> seats;
        map<string, double> fares;
        
        cout << "\n🚉 Add New Train" << endl;
        cout << "----------------" << endl;
        cout << "Train Number: ";
        getline(cin, no);
        cout << "Train Name: ";
        getline(cin, name);
        cout << "Source Station: ";
        getline(cin, src);
        cout << "Destination Station: ";
        getline(cin, dest);
        
        // Add intermediate stations
        cout << "Enter intermediate stations (comma separated): ";
        string stationsInput;
        getline(cin, stationsInput);
        stringstream ss(stationsInput);
        string station;
        while (getline(ss, station, ',')) {
            stations.push_back(station);
        }
        
        // Insert source and destination
        stations.insert(stations.begin(), src);
        stations.push_back(dest);
        
        cout << "Departure Time (HH:MM): ";
        getline(cin, dep);
        cout << "Arrival Time (HH:MM): ";
        getline(cin, arr);
        
        // Add coach types
        cout << "Enter coach types and seats (format: 1A 20, 2A 50): ";
        string coachesInput;
        getline(cin, coachesInput);
        stringstream css(coachesInput);
        string coachInfo;
        while (getline(css, coachInfo, ',')) {
            stringstream ciss(coachInfo);
            string type;
            int count;
            ciss >> type >> count;
            seats[type] = count;
        }
        
        // Add fares
        for (auto& seat : seats) {
            cout << "Enter fare for " << seat.first << ": ₹";
            double fare;
            cin >> fare;
            fares[seat.first] = fare;
            cin.ignore();
        }
        
        trains.emplace_back(no, name, src, dest, stations, dep, arr, seats, fares);
        cout << "\n✅ Train added successfully!" << endl;
    }

    // View all users (admin only)
    void viewAllUsers() {
        cout << "\n👥 Registered Users" << endl;
        cout << "==================================================================" << endl;
        cout << left << setw(15) << "Username" << setw(20) << "Name" 
             << setw(15) << "Phone" << setw(25) << "Email" << "Role" << endl;
        cout << "==================================================================" << endl;
        for (const auto& user : users) {
            user.second.display();
        }
    }

    // Main menu
    void showMainMenu() {
        int choice;
        do {
            cout << "\n🚉 Indian Railway Reservation System" << endl;
            cout << "============================================" << endl;
            cout << "1. Book Ticket" << endl;
            cout << "2. Cancel Ticket" << endl;
            cout << "3. Check PNR Status" << endl;
            cout << "4. View Train Schedule" << endl;
            if (currentUser && currentUser->isAdministrator()) {
                cout << "5. Admin Panel" << endl;
                cout << "6. Logout" << endl;
                cout << "Enter your choice (1-6): ";
            } else {
                cout << "5. Logout" << endl;
                cout << "Enter your choice (1-5): ";
            }
            
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: bookTicket(); break;
                case 2: cancelTicket(); break;
                case 3: checkPNRStatus(); break;
                case 4: displayTrainSchedule(); break;
                case 5: 
                    if (currentUser && currentUser->isAdministrator()) {
                        adminPanel();
                    } else {
                        currentUser = nullptr;
                        return;
                    }
                    break;
                case 6: 
                    if (currentUser && currentUser->isAdministrator()) {
                        currentUser = nullptr;
                        return;
                    }
                    // fall through
                default: 
                    cout << "❌ Invalid choice!" << endl;
            }
            
            if (choice >= 1 && choice <= (currentUser && currentUser->isAdministrator() ? 6 : 5)) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
            }
        } while (true);
    }
};

int main() {
    RailwayReservationSystem irctc;
    
    while (irctc.showLoginMenu()) {
        irctc.showMainMenu();
    }
    
    cout << "\n🚉 Thank you for using Indian Railway Reservation System! 🚉" << endl;
    return 0;
}
