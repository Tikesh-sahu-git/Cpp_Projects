#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

// Question structure
struct Question {
    string questionText;
    vector<string> options;
    int correctAnswer;
    string explanation;
};

// Quiz class
class QuizGame {
private:
    vector<Question> questions;
    int score;
    string playerName;

    // Initialize questions
    void initializeQuestions() {
        // C++ Basics
        questions.push_back({
            "What is the correct syntax to output 'Hello World' in C++?",
            {"cout << 'Hello World';", "print('Hello World');", "System.out.println('Hello World');", "console.log('Hello World');"},
            0,
            "In C++, cout with << operator is used for output."
        });

        questions.push_back({
            "Which of the following is NOT a valid C++ data type?",
            {"int", "float", "real", "double"},
            2,
            "'real' is not a C++ data type, though 'float' and 'double' are."
        });

        // OOP Concepts
        questions.push_back({
            "What is the main principle of OOP that allows creating new classes from existing ones?",
            {"Encapsulation", "Inheritance", "Polymorphism", "Abstraction"},
            1,
            "Inheritance allows creating new classes (derived) from existing ones (base)."
        });

        questions.push_back({
            "Which access specifier allows class members to be accessed only within the class?",
            {"public", "private", "protected", "static"},
            1,
            "private members are only accessible within their own class."
        });

        // STL Questions
        questions.push_back({
            "Which STL container provides constant time complexity for insertion and deletion at both ends?",
            {"vector", "list", "deque", "array"},
            2,
            "deque (double-ended queue) allows efficient insertion/deletion at both ends."
        });

        questions.push_back({
            "What does STL stand for in C++?",
            {"Standard Template Library", "Simple Type Library", "System Template Library", "Structured Type List"},
            0,
            "STL stands for Standard Template Library."
        });

        // Memory Management
        questions.push_back({
            "Which operator is used to allocate memory dynamically in C++?",
            {"malloc", "new", "alloc", "create"},
            1,
            "'new' is used for dynamic memory allocation in C++."
        });

        questions.push_back({
            "What is the purpose of a destructor in C++?",
            {"To initialize objects", "To allocate memory", "To deallocate resources", "To create copies of objects"},
            2,
            "Destructors are used to release resources before object destruction."
        });
    }

    // Display question with options
    void displayQuestion(const Question& q, int questionNumber) {
        cout << "\nQuestion #" << questionNumber << endl;
        cout << "-------------------------" << endl;
        cout << q.questionText << endl << endl;

        for (int i = 0; i < q.options.size(); i++) {
            cout << i + 1 << ". " << q.options[i] << endl;
        }
        cout << "\nYour answer (1-4): ";
    }

public:
    QuizGame() : score(0) {
        initializeQuestions();
        // Shuffle questions for variety
        srand(time(0));
        random_shuffle(questions.begin(), questions.end());
    }

    void startGame() {
        cout << "🧠 PROGRAMMING QUIZ GAME 🧠" << endl;
        cout << "--------------------------" << endl;
        cout << "Enter your name: ";
        getline(cin, playerName);

        cout << "\nWelcome, " << playerName << "!" << endl;
        cout << "You will be presented with " << questions.size() << " questions about C++ programming." << endl;
        cout << "Let's begin!\n" << endl;

        for (int i = 0; i < questions.size(); i++) {
            displayQuestion(questions[i], i + 1);

            int userAnswer;
            cin >> userAnswer;

            // Validate input
            while (userAnswer < 1 || userAnswer > 4) {
                cout << "Invalid choice! Enter a number between 1-4: ";
                cin >> userAnswer;
            }

            // Check answer
            if (userAnswer - 1 == questions[i].correctAnswer) {
                cout << "✅ Correct! ";
                score++;
            } else {
                cout << "❌ Incorrect! ";
                cout << "The correct answer was: " << questions[i].options[questions[i].correctAnswer] << endl;
            }

            cout << "Explanation: " << questions[i].explanation << endl;
        }

        showResult();
    }

    void showResult() {
        double percentage = (static_cast<double>(score) / questions.size()) * 100;

        cout << "\n🎉 QUIZ COMPLETE! 🎉" << endl;
        cout << "-------------------" << endl;
        cout << "Player: " << playerName << endl;
        cout << "Score: " << score << "/" << questions.size() << endl;
        cout << fixed << setprecision(1);
        cout << "Percentage: " << percentage << "%" << endl;

        // Performance remark
        if (percentage >= 80) {
            cout << "🌟 Excellent! You're a C++ expert!" << endl;
        } else if (percentage >= 60) {
            cout << "👍 Good job! You know your C++ well!" << endl;
        } else if (percentage >= 40) {
            cout << "😊 Not bad! Keep practicing!" << endl;
        } else {
            cout << "📚 Keep learning! Review C++ fundamentals." << endl;
        }
    }
};

int main() {
    char playAgain;
    
    do {
        QuizGame game;
        game.startGame();

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
        cin.ignore(); // Clear input buffer
    } while (tolower(playAgain) == 'y');

    cout << "\nThanks for playing! Happy coding! 💻" << endl;
    return 0;
}
