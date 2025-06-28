#include <iostream>
#include <vector>
#include <iomanip>  // For setprecision()
using namespace std;

// Function to calculate grade based on percentage
char calculateGrade(double percentage) {
    if (percentage >= 90) return 'A';
    else if (percentage >= 80) return 'B';
    else if (percentage >= 70) return 'C';
    else if (percentage >= 60) return 'D';
    else if (percentage >= 40) return 'E';
    else return 'F';
}

int main() {
    int numSubjects;
    vector<double> marks;
    double totalMarks = 0, percentage;
    char repeat;

    cout << "📚 STUDENT GRADE CALCULATOR 📚" << endl;
    cout << "-----------------------------" << endl;

    do {
        cout << "\nEnter number of subjects: ";
        cin >> numSubjects;

        marks.clear();
        totalMarks = 0;

        // Input marks for each subject
        for (int i = 0; i < numSubjects; ++i) {
            double mark;
            cout << "Enter marks for Subject " << i + 1 << " (out of 100): ";
            cin >> mark;

            // Input validation (0-100)
            while (mark < 0 || mark > 100) {
                cout << "Invalid marks! Enter again (0-100): ";
                cin >> mark;
            }
            marks.push_back(mark);
            totalMarks += mark;
        }

        // Calculate percentage
        percentage = (totalMarks / (numSubjects * 100)) * 100;
        char grade = calculateGrade(percentage);

        // Display results
        cout << fixed << setprecision(2);
        cout << "\n📊 RESULT SUMMARY 📊" << endl;
        cout << "Total Marks: " << totalMarks << "/" << numSubjects * 100 << endl;
        cout << "Percentage: " << percentage << "%" << endl;
        cout << "Grade: " << grade << endl;

        // Grade remarks
        if (grade == 'A') cout << "🎉 Excellent!" << endl;
        else if (grade == 'B') cout << "👍 Very Good!" << endl;
        else if (grade == 'C') cout << "😊 Good!" << endl;
        else if (grade == 'D') cout << "🤔 Needs Improvement!" << endl;
        else if (grade == 'E') cout << "⚠️ Passed, but work harder!" << endl;
        else cout << "❌ Failed! Seek help." << endl;

        // Ask to repeat
        cout << "\nCalculate another student? (y/n): ";
        cin >> repeat;
    } while (repeat == 'y' || repeat == 'Y');

    cout << "\nThank you for using the Grade Calculator! 🎓" << endl;
    return 0;
}
