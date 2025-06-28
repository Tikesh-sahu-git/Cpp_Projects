#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

class Voter {
private:
    string id;
    string name;
    bool hasVoted;

public:
    Voter(string i, string n) : id(i), name(n), hasVoted(false) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    bool getVotingStatus() const { return hasVoted; }

    // Mark as voted
    void vote() { hasVoted = true; }
};

class Candidate {
private:
    string id;
    string name;
    string party;
    int votes;

public:
    Candidate(string i, string n, string p) : id(i), name(n), party(p), votes(0) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    string getParty() const { return party; }
    int getVotes() const { return votes; }

    // Add vote
    void addVote() { votes++; }
};

class VotingSystem {
private:
    vector<Voter> voters;
    vector<Candidate> candidates;
    map<string, bool> voterLookup;  // For quick voter verification
    string votersFile = "voters.txt";
    string candidatesFile = "candidates.txt";
    string resultsFile = "results.txt";

    // Check if voter exists
    bool voterExists(string id) {
        return voterLookup.find(id) != voterLookup.end();
    }

    // Check if candidate exists
    bool candidateExists(string id) {
        for (const auto& c : candidates) {
            if (c.getId() == id) return true;
        }
        return false;
    }

    // Display candidates
    void displayCandidates() {
        cout << "\n🗳️ Available Candidates:" << endl;
        cout << "----------------------------------------" << endl;
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Party" << endl;
        cout << "----------------------------------------" << endl;
        
        for (const auto& candidate : candidates) {
            cout << setw(5) << candidate.getId() 
                 << setw(20) << candidate.getName()
                 << setw(15) << candidate.getParty() << endl;
        }
        cout << "----------------------------------------" << endl;
    }

public:
    // Load initial data
    void loadData() {
        // Load voters
        ifstream vFile(votersFile);
        if (vFile.is_open()) {
            string id, name;
            while (vFile >> id >> name) {
                voters.emplace_back(id, name);
                voterLookup[id] = false;
            }
            vFile.close();
        }

        // Load candidates
        ifstream cFile(candidatesFile);
        if (cFile.is_open()) {
            string id, name, party;
            while (getline(cFile, id, ',') &&
                   getline(cFile, name, ',') &&
                   getline(cFile, party)) {
                candidates.emplace_back(id, name, party);
            }
            cFile.close();
        }
    }

    // Register new voter
    void registerVoter() {
        string id, name;
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

        voters.emplace_back(id, name);
        voterLookup[id] = false;
        cout << "✅ Voter registered successfully!" << endl;
    }

    // Add candidate
    void addCandidate() {
        string id, name, party;
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

        candidates.emplace_back(id, name, party);
        cout << "✅ Candidate added successfully!" << endl;
    }

    // Cast vote
    void castVote() {
        string voterId, candidateId;
        cout << "\n✍️ Cast Your Vote" << endl;
        cout << "Enter your voter ID: ";
        cin >> voterId;

        if (!voterExists(voterId)) {
            cout << "❌ You are not registered to vote!" << endl;
            return;
        }

        // Find voter
        auto voterIt = find_if(voters.begin(), voters.end(),
            [voterId](const Voter& v) { return v.getId() == voterId; });

        if (voterIt->getVotingStatus()) {
            cout << "❌ You have already voted!" << endl;
            return;
        }

        displayCandidates();
        cout << "Enter candidate ID to vote for: ";
        cin >> candidateId;

        if (!candidateExists(candidateId)) {
            cout << "❌ Invalid candidate ID!" << endl;
            return;
        }

        // Find candidate and add vote
        auto candidateIt = find_if(candidates.begin(), candidates.end(),
            [candidateId](const Candidate& c) { return c.getId() == candidateId; });

        candidateIt->addVote();
        voterIt->vote();
        cout << "✅ Thank you for voting!" << endl;
    }

    // Display results
    void displayResults() {
        if (candidates.empty()) {
            cout << "\n❌ No candidates available!" << endl;
            return;
        }

        // Sort candidates by votes (descending)
        sort(candidates.begin(), candidates.end(),
            [](const Candidate& a, const Candidate& b) {
                return a.getVotes() > b.getVotes();
            });

        cout << "\n🏆 Election Results" << endl;
        cout << "----------------------------------------" << endl;
        cout << left << setw(5) << "ID" << setw(20) << "Name" 
             << setw(15) << "Party" << setw(10) << "Votes" << endl;
        cout << "----------------------------------------" << endl;
        
        for (const auto& candidate : candidates) {
            cout << setw(5) << candidate.getId() 
                 << setw(20) << candidate.getName()
                 << setw(15) << candidate.getParty()
                 << setw(10) << candidate.getVotes() << endl;
        }
        cout << "----------------------------------------" << endl;

        // Announce winner
        if (candidates[0].getVotes() > 0) {
            cout << "\n🎉 Winner: " << candidates[0].getName()
                 << " (" << candidates[0].getParty() << ") with "
                 << candidates[0].getVotes() << " votes!" << endl;
        } else {
            cout << "\nNo votes have been cast yet." << endl;
        }
    }

    // Save data to files
    void saveData() {
        // Save voters
        ofstream vFile(votersFile);
        if (vFile.is_open()) {
            for (const auto& voter : voters) {
                vFile << voter.getId() << " " << voter.getName() << "\n";
            }
            vFile.close();
        }

        // Save candidates
        ofstream cFile(candidatesFile);
        if (cFile.is_open()) {
            for (const auto& candidate : candidates) {
                cFile << candidate.getId() << ","
                      << candidate.getName() << ","
                      << candidate.getParty() << "\n";
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
                      << candidate.getVotes() << "\n";
            }
            rFile.close();
        }

        cout << "\n💾 All data saved successfully!" << endl;
    }

    // Display statistics
    void displayStats() {
        int totalVoters = voters.size();
        int votedCount = count_if(voters.begin(), voters.end(),
            [](const Voter& v) { return v.getVotingStatus(); });
        double turnout = (totalVoters > 0) ? (votedCount * 100.0 / totalVoters) : 0;

        cout << "\n📊 Election Statistics" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Total Registered Voters: " << totalVoters << endl;
        cout << "Voters Who Have Voted: " << votedCount << endl;
        cout << fixed << setprecision(1);
        cout << "Voter Turnout: " << turnout << "%" << endl;
        cout << "----------------------------------------" << endl;
    }
};

// Main menu
void displayMenu() {
    cout << "\n🗳️ ELECTION VOTING SYSTEM" << endl;
    cout << "==========================" << endl;
    cout << "1. Register Voter" << endl;
    cout << "2. Add Candidate" << endl;
    cout << "3. Cast Vote" << endl;
    cout << "4. View Results" << endl;
    cout << "5. View Statistics" << endl;
    cout << "6. Save Data" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice (1-7): ";
}

int main() {
    VotingSystem votingSystem;
    votingSystem.loadData();

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: votingSystem.registerVoter(); break;
            case 2: votingSystem.addCandidate(); break;
            case 3: votingSystem.castVote(); break;
            case 4: votingSystem.displayResults(); break;
            case 5: votingSystem.displayStats(); break;
            case 6: votingSystem.saveData(); break;
            case 7: 
                votingSystem.saveData();
                cout << "\n👋 Exiting system. Goodbye!" << endl;
                break;
            default: cout << "❌ Invalid choice! Try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
