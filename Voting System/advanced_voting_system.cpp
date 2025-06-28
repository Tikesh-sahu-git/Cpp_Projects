#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

// ------------------- Utility Functions -------------------
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void sleep(int milliseconds) {
    #ifdef _WIN32
    Sleep(milliseconds);
    #else
    usleep(milliseconds * 1000);
    #endif
}

// ------------------- Classes -------------------
class Voter {
private:
    string id;
    string name;
    string pin;
    bool hasVoted;
    vector<string> votedPositions; // Track which positions they've voted for

public:
    Voter(string i, string n, string p) : id(i), name(n), pin(p), hasVoted(false) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    bool getVotingStatus() const { return hasVoted; }
    bool hasVotedForPosition(const string& position) const {
        return find(votedPositions.begin(), votedPositions.end(), position) != votedPositions.end();
    }

    // Authentication
    bool verifyPin(string input) const { return pin == input; }

    // Voting
    void markVoted(const string& position) { 
        votedPositions.push_back(position);
        if (!hasVoted) hasVoted = true;
    }
};

class Candidate {
private:
    string id;
    string name;
    string party;
    string position; // President, Senator, etc.
    int votes;

public:
    Candidate(string i, string n, string p, string pos) 
        : id(i), name(n), party(p), position(pos), votes(0) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    string getParty() const { return party; }
    string getPosition() const { return position; }
    int getVotes() const { return votes; }

    // Add vote
    void addVote() { votes++; }
};

class Election {
private:
    string name;
    time_t startTime;
    time_t endTime;
    bool isActive;

public:
    Election(string n, time_t start, time_t end) 
        : name(n), startTime(start), endTime(end), isActive(false) {}

    // Check if election is running
    bool checkStatus() const {
        time_t now = time(nullptr);
        return now >= startTime && now <= endTime;
    }

    // Getters
    string getName() const { return name; }
    string getTimeRemaining() const {
        time_t now = time(nullptr);
        if (now > endTime) return "Election ended";
        
        int seconds = endTime - now;
        int hours = seconds / 3600;
        seconds %= 3600;
        int minutes = seconds / 60;
        seconds %= 60;
        
        return to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s";
    }
};

class VotingSystem {
private:
    vector<Voter> voters;
    vector<Candidate> candidates;
    vector<Election> elections;
    map<string, bool> voterLookup;
    vector<string> positions = {"President", "Senator", "Mayor"};
    
    // Files
    string votersFile = "voters.txt";
    string candidatesFile = "candidates.txt";
    string resultsFile = "results.txt";
    string auditLogFile = "audit.log";
    
    // Network properties
    bool isNetworked = false;
    string stationId = "MAIN";

    // Helper functions
    bool voterExists(string id) {
        return voterLookup.find(id) != voterLookup.end();
    }

    bool candidateExists(string id) {
        return any_of(candidates.begin(), candidates.end(),
            [id](const Candidate& c) { return c.getId() == id; });
    }

    void logAction(string action) {
        time_t now = time(nullptr);
        char timestamp[100];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        ofstream log(auditLogFile, ios::app);
        if (log.is_open()) {
            log << "[" << timestamp << "] [" << stationId << "] " << action << "\n";
            log.close();
        }
    }

public:
    VotingSystem() {
        // Initialize with a default election (starts now, ends in 24 hours)
        time_t now = time(nullptr);
        elections.emplace_back("General Election", now, now + 24*3600);
        loadData();
    }

    // ------------------- Core Methods -------------------
    void loadData() {
        // Load voters
        ifstream vFile(votersFile);
        if (vFile.is_open()) {
            string id, name, pin;
            while (vFile >> id >> name >> pin) {
                voters.emplace_back(id, name, pin);
                voterLookup[id] = false;
            }
            vFile.close();
        }

        // Load candidates
        ifstream cFile(candidatesFile);
        if (cFile.is_open()) {
            string line;
            while (getline(cFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1+1);
                size_t pos3 = line.find(',', pos2+1);
                
                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                    string id = line.substr(0, pos1);
                    string name = line.substr(pos1+1, pos2-pos1-1);
                    string party = line.substr(pos2+1, pos3-pos2-1);
                    string position = line.substr(pos3+1);
                    candidates.emplace_back(id, name, party, position);
                }
            }
            cFile.close();
        }

        logAction("System initialized");
    }

    void saveData() {
        // Save voters
        ofstream vFile(votersFile);
        if (vFile.is_open()) {
            for (const auto& voter : voters) {
                vFile << voter.getId() << " " << voter.getName() << " " << "1234" << "\n"; // Simplified for example
            }
            vFile.close();
        }

        // Save candidates
        ofstream cFile(candidatesFile);
        if (cFile.is_open()) {
            for (const auto& candidate : candidates) {
                cFile << candidate.getId() << ","
                      << candidate.getName() << ","
                      << candidate.getParty() << ","
                      << candidate.getPosition() << "\n";
            }
            cFile.close();
        }

        // Save results
        ofstream rFile(resultsFile);
        if (rFile.is_open()) {
            for (const auto& candidate : candidates) {
                rFile << candidate.getId() << ","
                      << candidate.getName() << ","
                      << candidate.getParty() << ","
                      << candidate.getPosition() << ","
                      << candidate.getVotes() << "\n";
            }
            rFile.close();
        }

        logAction("Data saved to files");
    }

    // ------------------- Enhanced Features -------------------
    void registerVoter() {
        string id, name, pin;
        cout << "\n📝 Voter Registration" << endl;
        cout << "Enter voter ID: ";
        cin >> id;
        
        if (voterExists(id)) {
            cout << "❌ Voter already registered!" << endl;
            return;
        }

        cout << "Enter voter name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Set 4-digit PIN: ";
        cin >> pin;
        while (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
            cout << "Invalid PIN. Enter 4 digits: ";
            cin >> pin;
        }

        voters.emplace_back(id, name, pin);
        voterLookup[id] = false;
        logAction("Voter registered: " + id);
        cout << "✅ Voter registered successfully!" << endl;
    }

    void addCandidate() {
        string id, name, party, position;
        cout << "\n👔 Add Candidate" << endl;
        cout << "Enter candidate ID: ";
        cin >> id;
        
        if (candidateExists(id)) {
            cout << "❌ Candidate already exists!" << endl;
            return;
        }

        cout << "Enter candidate name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter party: ";
        getline(cin, party);

        // Select position
        cout << "Select position:" << endl;
        for (int i = 0; i < positions.size(); i++) {
            cout << i+1 << ". " << positions[i] << endl;
        }
        int posChoice;
        cout << "Enter choice (1-" << positions.size() << "): ";
        cin >> posChoice;
        position = positions[posChoice-1];

        candidates.emplace_back(id, name, party, position);
        logAction("Candidate added: " + name + " for " + position);
        cout << "✅ Candidate added successfully!" << endl;
    }

    void castVote() {
        // Check election status
        if (!elections[0].checkStatus()) {
            cout << "❌ Election is not currently active." << endl;
            cout << "Time remaining: " << elections[0].getTimeRemaining() << endl;
            return;
        }

        string voterId, pin;
        cout << "\n✍️ Voter Authentication" << endl;
        cout << "Enter your voter ID: ";
        cin >> voterId;

        if (!voterExists(voterId)) {
            cout << "❌ You are not registered to vote!" << endl;
            return;
        }

        // Find voter
        auto voterIt = find_if(voters.begin(), voters.end(),
            [voterId](const Voter& v) { return v.getId() == voterId; });

        // PIN verification
        cout << "Enter your 4-digit PIN: ";
        cin >> pin;
        if (!voterIt->verifyPin(pin)) {
            cout << "❌ Invalid PIN!" << endl;
            logAction("Failed login attempt for voter: " + voterId);
            return;
        }

        // Vote for each position
        for (const auto& position : positions) {
            if (voterIt->hasVotedForPosition(position)) {
                continue; // Skip if already voted for this position
            }

            clearScreen();
            cout << "\n🗳️ Voting for " << position << endl;
            cout << "-------------------------" << endl;

            // Filter candidates for this position
            vector<Candidate*> positionCandidates;
            for (auto& c : candidates) {
                if (c.getPosition() == position) {
                    positionCandidates.push_back(&c);
                }
            }

            if (positionCandidates.empty()) {
                cout << "No candidates available for this position." << endl;
                continue;
            }

            // Display candidates
            cout << left << setw(5) << "ID" << setw(20) << "Name" 
                 << setw(15) << "Party" << endl;
            cout << "-------------------------" << endl;
            for (const auto& c : positionCandidates) {
                cout << setw(5) << c->getId() 
                     << setw(20) << c->getName()
                     << setw(15) << c->getParty() << endl;
            }

            // Get vote
            string candidateId;
            cout << "\nEnter candidate ID to vote for (or 'skip'): ";
            cin >> candidateId;

            if (candidateId == "skip") {
                continue;
            }

            bool valid = false;
            for (auto& c : positionCandidates) {
                if (c->getId() == candidateId) {
                    c->addVote();
                    voterIt->markVoted(position);
                    valid = true;
                    logAction("Vote cast by " + voterId + " for " + position + ": " + c->getName());
                    cout << "✅ Vote recorded for " << c->getName() << endl;
                    sleep(1000);
                    break;
                }
            }

            if (!valid) {
                cout << "❌ Invalid candidate ID!" << endl;
            }
        }

        cout << "\n✅ Thank you for voting!" << endl;
    }

    void displayResults() {
        if (candidates.empty()) {
            cout << "\n❌ No candidates available!" << endl;
            return;
        }

        cout << "\n🏆 Election Results" << endl;
        cout << "========================================" << endl;

        // Results by position
        for (const auto& position : positions) {
            cout << "\n" << position << " Results:" << endl;
            cout << "----------------------------------------" << endl;
            cout << left << setw(5) << "ID" << setw(20) << "Name" 
                 << setw(15) << "Party" << setw(10) << "Votes" << endl;
            cout << "----------------------------------------" << endl;
            
            // Filter and sort candidates for this position
            vector<Candidate> positionCandidates;
            copy_if(candidates.begin(), candidates.end(), back_inserter(positionCandidates),
                [position](const Candidate& c) { return c.getPosition() == position; });

            sort(positionCandidates.begin(), positionCandidates.end(),
                [](const Candidate& a, const Candidate& b) {
                    return a.getVotes() > b.getVotes();
                });

            // Display results
            for (const auto& candidate : positionCandidates) {
                cout << setw(5) << candidate.getId() 
                     << setw(20) << candidate.getName()
                     << setw(15) << candidate.getParty()
                     << setw(10) << candidate.getVotes() << endl;
            }

            // Announce winner if votes exist
            if (!positionCandidates.empty() && positionCandidates[0].getVotes() > 0) {
                cout << "\n🎉 Winner: " << positionCandidates[0].getName()
                     << " (" << positionCandidates[0].getParty() << ") with "
                     << positionCandidates[0].getVotes() << " votes!" << endl;
            }
            cout << "----------------------------------------" << endl;
        }
        
        logAction("Results viewed");
    }

    void displayStats() {
        int totalVoters = voters.size();
        int votedCount = count_if(voters.begin(), voters.end(),
            [](const Voter& v) { return v.getVotingStatus(); });
        double turnout = (totalVoters > 0) ? (votedCount * 100.0 / totalVoters) : 0;

        cout << "\n📊 Election Statistics" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Election: " << elections[0].getName() << endl;
        cout << "Time Remaining: " << elections[0].getTimeRemaining() << endl;
        cout << "Total Registered Voters: " << totalVoters << endl;
        cout << "Voters Who Have Voted: " << votedCount << endl;
        cout << fixed << setprecision(1);
        cout << "Voter Turnout: " << turnout << "%" << endl;
        cout << "----------------------------------------" << endl;
        
        logAction("Statistics viewed");
    }

    void viewAuditLog() {
        ifstream log(auditLogFile);
        if (log.is_open()) {
            cout << "\n📜 Audit Log" << endl;
            cout << "----------------------------------------" << endl;
            string line;
            while (getline(log, line)) {
                cout << line << endl;
            }
            cout << "----------------------------------------" << endl;
            log.close();
        } else {
            cout << "❌ No audit log entries found!" << endl;
        }
    }

    void configureNetwork() {
        cout << "\n🌐 Configure Network Settings" << endl;
        cout << "Current station ID: " << stationId << endl;
        cout << "Enter new station ID: ";
        cin >> stationId;
        isNetworked = true;
        logAction("Network configured. Station ID: " + stationId);
        cout << "✅ Network settings updated!" << endl;
    }
};

// ------------------- Main Menu -------------------
void displayAdminMenu() {
    clearScreen();
    cout << "\n🔒 ADMINISTRATOR MENU" << endl;
    cout << "========================" << endl;
    cout << "1. Register Voter" << endl;
    cout << "2. Add Candidate" << endl;
    cout << "3. View Results" << endl;
    cout << "4. View Statistics" << endl;
    cout << "5. View Audit Log" << endl;
    cout << "6. Configure Network" << endl;
    cout << "7. Save Data" << endl;
    cout << "8. Back to Main Menu" << endl;
    cout << "Enter your choice (1-8): ";
}

void displayMainMenu() {
    clearScreen();
    cout << "\n🗳️ ELECTION VOTING SYSTEM" << endl;
    cout << "========================" << endl;
    cout << "1. Vote" << endl;
    cout << "2. Admin Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice (1-3): ";
}

int main() {
    VotingSystem votingSystem;
    string adminPass = "admin123"; // Default admin password
    int choice;

    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1: // Vote
                votingSystem.castVote();
                break;
                
            case 2: { // Admin login
                string password;
                cout << "\nEnter admin password: ";
                cin >> password;
                
                if (password == adminPass) {
                    int adminChoice;
                    do {
                        displayAdminMenu();
                        cin >> adminChoice;
                        
                        switch (adminChoice) {
                            case 1: votingSystem.registerVoter(); break;
                            case 2: votingSystem.addCandidate(); break;
                            case 3: votingSystem.displayResults(); break;
                            case 4: votingSystem.displayStats(); break;
                            case 5: votingSystem.viewAuditLog(); break;
                            case 6: votingSystem.configureNetwork(); break;
                            case 7: votingSystem.saveData(); break;
                            case 8: break;
                            default: cout << "❌ Invalid choice!" << endl;
                        }
                        
                        if (adminChoice != 8) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore();
                            cin.get();
                        }
                    } while (adminChoice != 8);
                } else {
                    cout << "❌ Invalid password!" << endl;
                }
                break;
            }
                
            case 3: // Exit
                votingSystem.saveData();
                cout << "\n👋 Exiting system. Goodbye!" << endl;
                break;
                
            default:
                cout << "❌ Invalid choice!" << endl;
        }
        
        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 3);

    return 0;
}
