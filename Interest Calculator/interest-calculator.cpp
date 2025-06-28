#include <iostream>
#include <cmath> // For pow() function used in compound interest
#include <iomanip> // For formatting output
using namespace std;

int main() {
    int choice;
    double principal, rate, time, interest, amount;

    cout << "INTEREST CALCULATOR" << endl;
    cout << "-------------------" << endl;
    cout << "1. Simple Interest" << endl;
    cout << "2. Compound Interest" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    // Input common values
    cout << "Enter principal amount: ";
    cin >> principal;
    cout << "Enter annual interest rate (%): ";
    cin >> rate;
    cout << "Enter time period (in years): ";
    cin >> time;

    // Convert rate from percentage to decimal
    rate = rate / 100;

    // Calculate based on user choice
    switch(choice) {
        case 1: // Simple Interest
            interest = principal * rate * time;
            amount = principal + interest;
            cout << fixed << setprecision(2); // Format output to 2 decimal places
            cout << "\nSIMPLE INTEREST CALCULATION" << endl;
            cout << "Principal: $" << principal << endl;
            cout << "Interest Rate: " << rate*100 << "%" << endl;
            cout << "Time: " << time << " years" << endl;
            cout << "Interest Earned: $" << interest << endl;
            cout << "Total Amount: $" << amount << endl;
            break;

        case 2: // Compound Interest
            int compoundingFrequency;
            cout << "Enter compounding frequency per year (1 for annual, 12 for monthly): ";
            cin >> compoundingFrequency;

            amount = principal * pow(1 + (rate/compoundingFrequency), compoundingFrequency * time);
            interest = amount - principal;

            cout << fixed << setprecision(2);
            cout << "\nCOMPOUND INTEREST CALCULATION" << endl;
            cout << "Principal: $" << principal << endl;
            cout << "Interest Rate: " << rate*100 << "%" << endl;
            cout << "Time: " << time << " years" << endl;
            cout << "Compounding Frequency: " << compoundingFrequency << " times per year" << endl;
            cout << "Interest Earned: $" << interest << endl;
            cout << "Total Amount: $" << amount << endl;
            break;

        default:
            cout << "Invalid choice! Please select 1 or 2." << endl;
    }

    return 0;
}
