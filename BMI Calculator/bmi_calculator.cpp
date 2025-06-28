#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

// Structure to store BMI records
struct BMIRecord {
    string date;
    float bmi;
    string category;
};

// Function prototypes
float calculateBMI(float height, float weight, bool isMetric);
string getBMICategory(float bmi, int age, char sex);
void saveToFile(const vector<BMIRecord>& records);
void displayTrend(const vector<BMIRecord>& records);
void displayIdealWeight(float height, bool isMetric, char sex);

int main() {
    vector<BMIRecord> records;
    char unitChoice, sex, repeat;
    int age;
    float height, weight, bmi;
    string currentDate;

    cout << "⚖️ ADVANCED BMI CALCULATOR ⚖️" << endl;
    cout << "---------------------------" << endl;

    do {
        cout << "\nChoose unit system:" << endl;
        cout << "1. Metric (kg & meters)" << endl;
        cout << "2. Imperial (lbs & inches)" << endl;
        cout << "Enter choice (1 or 2): ";
        cin >> unitChoice;

        while (unitChoice != '1' && unitChoice != '2') {
            cout << "Invalid choice! Please enter 1 or 2: ";
            cin >> unitChoice;
        }

        cout << "Enter your age: ";
        cin >> age;
        cout << "Enter your sex (M/F): ";
        cin >> sex;
        sex = toupper(sex);

        if (unitChoice == '1') {
            cout << "Enter height in meters: ";
            cin >> height;
            cout << "Enter weight in kilograms: ";
            cin >> weight;
        } else {
            cout << "Enter height in inches: ";
            cin >> height;
            cout << "Enter weight in pounds: ";
            cin >> weight;
        }

        cout << "Enter today's date (DD-MM-YYYY): ";
        cin >> currentDate;

        bmi = calculateBMI(height, weight, unitChoice == '1');
        string category = getBMICategory(bmi, age, sex);

        // Store the record
        records.push_back({currentDate, bmi, category});

        cout << fixed << setprecision(1);
        cout << "\nYour BMI: " << bmi << endl;
        cout << "BMI Category: " << category << endl;

        // Display ideal weight range
        displayIdealWeight(height, unitChoice == '1', sex);

        // Display BMI trend if we have multiple records
        if (records.size() > 1) {
            displayTrend(records);
        }

        // Save to file
        saveToFile(records);

        cout << "\nCalculate again? (y/n): ";
        cin >> repeat;
    } while (toupper(repeat) == 'Y');

    cout << "\nThank you for using the BMI Calculator! Stay healthy! 💪" << endl;
    return 0;
}

float calculateBMI(float height, float weight, bool isMetric) {
    if (isMetric) {
        return weight / (height * height);
    } else {
        return (weight / (height * height)) * 703;
    }
}

string getBMICategory(float bmi, int age, char sex) {
    // Age and sex adjusted categories
    if (age < 18) {
        return "Use pediatric BMI charts for children";
    }
    
    if (sex == 'F') {
        // Slightly different ranges for women
        if (bmi < 18.5) return "Underweight";
        else if (bmi < 24) return "Normal weight";
        else if (bmi < 29) return "Overweight";
        else return "Obese";
    } else {
        // Standard ranges for men
        if (bmi < 18.5) return "Underweight";
        else if (bmi < 25) return "Normal weight";
        else if (bmi < 30) return "Overweight";
        else return "Obese";
    }
}

void displayIdealWeight(float height, bool isMetric, char sex) {
    // Calculate ideal weight range (BMI 18.5-24.9)
    float lowerWeight, upperWeight;
    
    if (isMetric) {
        lowerWeight = 18.5 * height * height;
        upperWeight = 24.9 * height * height;
        cout << "\nIdeal weight range: " << lowerWeight << " - " << upperWeight << " kg" << endl;
    } else {
        lowerWeight = (18.5 * height * height) / 703;
        upperWeight = (24.9 * height * height) / 703;
        cout << "\nIdeal weight range: " << lowerWeight << " - " << upperWeight << " lbs" << endl;
    }
}

void displayTrend(const vector<BMIRecord>& records) {
    cout << "\nBMI Trend:" << endl;
    cout << "Date\t\tBMI\tCategory" << endl;
    cout << "----------------------------" << endl;
    
    for (const auto& record : records) {
        cout << record.date << "\t" << record.bmi << "\t" << record.category << endl;
    }
    
    // Simple trend analysis
    if (records.size() > 1) {
        float change = records.back().bmi - records[records.size()-2].bmi;
        cout << "\nLast change: " << (change >= 0 ? "+" : "") << change << " BMI points" << endl;
    }
}

void saveToFile(const vector<BMIRecord>& records) {
    ofstream outFile("bmi_history.txt");
    if (outFile.is_open()) {
        outFile << "BMI History" << endl;
        outFile << "Date\t\tBMI\tCategory" << endl;
        outFile << "----------------------------" << endl;
        
        for (const auto& record : records) {
            outFile << record.date << "\t" << record.bmi << "\t" << record.category << endl;
        }
        
        outFile.close();
    }
}
