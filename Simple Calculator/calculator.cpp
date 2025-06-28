#include <iostream>
using namespace std;

int main() {
    char operation;
    double num1, num2, result;

    // Display welcome message
    cout << "SIMPLE CALCULATOR" << endl;
    cout << "-----------------" << endl;
    cout << "Enter an operation (+, -, *, /): ";
    cin >> operation;

    // Input numbers
    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter second number: ";
    cin >> num2;

    // Perform calculation based on operation
    switch(operation) {
        case '+':
            result = num1 + num2;
            cout << "Result: " << num1 << " + " << num2 << " = " << result;
            break;
        case '-':
            result = num1 - num2;
            cout << "Result: " << num1 << " - " << num2 << " = " << result;
            break;
        case '*':
            result = num1 * num2;
            cout << "Result: " << num1 << " * " << num2 << " = " << result;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                cout << "Result: " << num1 << " / " << num2 << " = " << result;
            } else {
                cout << "Error! Division by zero is not allowed.";
            }
            break;
        default:
            cout << "Invalid operation! Please use +, -, *, or /.";
    }

    cout << endl;
    return 0;
}
