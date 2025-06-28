#include <iostream>
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
using namespace std;

int main() {
    srand(time(0)); // Seed random number generator

    int secretNumber = rand() % 100 + 1; // Random number between 1-100
    int guess;
    int attempts = 0;
    int maxAttempts = 8; // Adjust difficulty here
    bool hasWon = false;

    cout << "🎮 NUMBER GUESSING GAME 🎮" << endl;
    cout << "-------------------------" << endl;
    cout << "Guess the number (between 1 and 100)!" << endl;
    cout << "You have " << maxAttempts << " attempts." << endl << endl;

    while (attempts < maxAttempts) {
        cout << "Attempt #" << attempts + 1 << ": Enter your guess: ";
        cin >> guess;
        attempts++;

        if (guess == secretNumber) {
            hasWon = true;
            break;
        } else if (guess < secretNumber) {
            cout << "Too low! Try higher." << endl;
        } else {
            cout << "Too high! Try lower." << endl;
        }
    }

    if (hasWon) {
        cout << "\n🎉 Congratulations! You guessed it in " << attempts << " attempts!" << endl;
    } else {
        cout << "\n😢 Game Over! The number was: " << secretNumber << endl;
    }

    // Play again option
    char playAgain;
    cout << "\nDo you want to play again? (y/n): ";
    cin >> playAgain;

    if (playAgain == 'y' || playAgain == 'Y') {
        main(); // Restart the game
    } else {
        cout << "Thanks for playing! 👋" << endl;
    }

    return 0;
}
