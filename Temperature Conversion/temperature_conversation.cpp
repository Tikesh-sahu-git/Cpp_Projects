#include <iostream>
#include <iomanip>  // For setprecision()
using namespace std;

// Function prototypes
double celsiusToFahrenheit(double celsius);
double fahrenheitToCelsius(double fahrenheit);
double celsiusToKelvin(double celsius);
double kelvinToCelsius(double kelvin);
double fahrenheitToKelvin(double fahrenheit);
double kelvinToFahrenheit(double kelvin);

int main() {
    int choice;
    double temperature, convertedTemp;
    char repeat;

    cout << "🌡️ TEMPERATURE CONVERTER 🌡️" << endl;
    cout << "---------------------------" << endl;

    do {
        cout << "\nChoose conversion type:" << endl;
        cout << "1. Celsius to Fahrenheit" << endl;
        cout << "2. Fahrenheit to Celsius" << endl;
        cout << "3. Celsius to Kelvin" << endl;
        cout << "4. Kelvin to Celsius" << endl;
        cout << "5. Fahrenheit to Kelvin" << endl;
        cout << "6. Kelvin to Fahrenheit" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        // Input validation
        while (choice < 1 || choice > 6) {
            cout << "Invalid choice! Please enter 1-6: ";
            cin >> choice;
        }

        cout << "Enter temperature: ";
        cin >> temperature;

        switch (choice) {
            case 1:
                convertedTemp = celsiusToFahrenheit(temperature);
                cout << temperature << "°C = " << fixed << setprecision(2) << convertedTemp << "°F" << endl;
                break;
            case 2:
                convertedTemp = fahrenheitToCelsius(temperature);
                cout << temperature << "°F = " << fixed << setprecision(2) << convertedTemp << "°C" << endl;
                break;
            case 3:
                convertedTemp = celsiusToKelvin(temperature);
                cout << temperature << "°C = " << fixed << setprecision(2) << convertedTemp << "K" << endl;
                break;
            case 4:
                convertedTemp = kelvinToCelsius(temperature);
                cout << temperature << "K = " << fixed << setprecision(2) << convertedTemp << "°C" << endl;
                break;
            case 5:
                convertedTemp = fahrenheitToKelvin(temperature);
                cout << temperature << "°F = " << fixed << setprecision(2) << convertedTemp << "K" << endl;
                break;
            case 6:
                convertedTemp = kelvinToFahrenheit(temperature);
                cout << temperature << "K = " << fixed << setprecision(2) << convertedTemp << "°F" << endl;
                break;
        }

        cout << "\nDo another conversion? (y/n): ";
        cin >> repeat;
    } while (repeat == 'y' || repeat == 'Y');

    cout << "\nThanks for using the Temperature Converter! ❄️🔥" << endl;
    return 0;
}

// Conversion functions
double celsiusToFahrenheit(double celsius) {
    return (celsius * 9/5) + 32;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32) * 5/9;
}

double celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

double fahrenheitToKelvin(double fahrenheit) {
    return (fahrenheit - 32) * 5/9 + 273.15;
}

double kelvinToFahrenheit(double kelvin) {
    return (kelvin - 273.15) * 9/5 + 32;
}
