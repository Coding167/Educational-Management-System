#ifndef DOCTORS_H_INCLUDED
#define DOCTORS_H_INCLUDED

#include "functions.h"
#include <iomanip>
int validateChoice(int startNum, int endNum, string msg);
vector<course> GetPersonCourse(string studentID);
assignment getAssignment(string id);
void addAssignment(assignment newAssignment, string courseCode);
void welcome();
void doctor(string ID);
vstr split(string text, char character);
string getPassword();
void addCourse(string name, string docID, string password);

void createCourse(string doctorID) {
    string courseName, password;
    cin.clear();
    cin.ignore(10000, '\n'); 
    cout << "Enter Course Name: ";
    getline(cin, courseName);
    cout<<"Enter the password: ";
    password = getPassword();
    addCourse(courseName, doctorID, password);
    cout << "Course created successfully.\n";
    doctor(doctorID);
}

void listCourses(string doctorID) {
    vector<course> courses = GetPersonCourse(doctorID);
    if (courses.empty()) {
        cout << "No courses found.\n";
        return;
    }
    for (int i = 0 ; i < courses.size() ; i++) {
        cout<<i+1<<") Course "<<left<<setw(10)<<courses.at(i).name;
        cout<<" -  Code: "<<courses.at(i).code<<endl;
    }
    int choice = validateChoice(1, courses.size(), "Enter Course Number To View it: ");
    cout<<endl;
    course selectedCourse = courses.at(choice - 1);
    cout<<"Course "<<selectedCourse.name<<" - Code: "<<selectedCourse.code<<endl;
    if (selectedCourse.studentsIDs.empty()) {
        cout << "No students enrolled in this course.\n";
    } else {
        cout << "Enrolled Students:\n";
        for (const string& studentID : selectedCourse.studentsIDs) {
            cout << " " << studentID;
        }
    }
    cout<<endl;
    if (selectedCourse.assignmentsIDs.empty()) {
        cout << "No assignments for this course.\n";
    } else {
        cout << "Assignments:\n";
        for (const string& assignmentID : selectedCourse.assignmentsIDs) {
            cout << " " << assignmentID;
        }
    }
    cout<<endl;
    doctor(doctorID);
    return;
}

void addAssignmentToCourse(string doctorID) {
    vector<course> courses = GetPersonCourse(doctorID);
    char yOrn;
    if (courses.empty()) {
        cout << "No courses found.\n";
        return;
    }
    for (int i = 0 ; i < courses.size() ; i++) {
        cout<<i+1<<") Course "<<left<<setw(10)<<courses.at(i).name;
        cout<<" -  Code: "<<courses.at(i).code<<endl;
    }
    int choice = validateChoice(1, courses.size(), "Enter Course Number To View it: ");
    cout<<endl;
    course selectedCourse = courses.at(choice - 1);
    cout<<"Course "<<selectedCourse.name<<" - Code: "<<selectedCourse.code<<endl;
    if (selectedCourse.studentsIDs.empty()) {
        cout << "No students enrolled in this course.\n";
    } else {
        cout << "Students: ";
        for (string studentID : selectedCourse.studentsIDs) {
            cout << " " << studentID;
        }
    }
    cout<<endl;
    if (selectedCourse.assignmentsIDs.empty()) {
        cout << "No assignments for this course.\n";
    } else {
        cout << "Assignments: ";
        for (string assignmentID : selectedCourse.assignmentsIDs) {
            cout << " " << assignmentID;
        }
    }
    while (true) {
        cout<<endl<<"Do you want to add an assignment to this course? (y/n): ";
        cin>>yOrn;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        } else {
            break;
        }
    }
    if (yOrn == 'y' || yOrn == 'Y') {
        assignment newAssignment;
        string question;
        vstr answers;
        int correctAnswer;
        newAssignment.id = "";
        cout << "Enter Assignment Question: ";
        cin.clear();
        cin.ignore(10000, '\n');
        getline(cin, question);
        newAssignment.question = question;
        cout << "Enter Answers (separated by '/'): ";
        string answersInput;
        getline(cin, answersInput);
        answers = split(answersInput, '/');
        newAssignment.answers = answers;
        for (int i = 0; i < answers.size(); i++) {
            cout << i + 1 << ") " << answers.at(i) << endl;
        }
        correctAnswer = validateChoice(1, answers.size(), "Enter Correct Answer Number: ");
        newAssignment.correctAnswer = correctAnswer - 1; // Convert to zero-based index
        newAssignment.studentsSolve = {};
        addAssignment(newAssignment, selectedCourse.code);
        cout << "Assignment added successfully.\n";
    } else if (yOrn == 'n' || yOrn == 'N') {
        cout << "No assignment added.\n";
    } else {
        cout << "Invalid input. Please enter 'y' or 'n'.\n";
    }
    doctor(doctorID);
    return;
}

void showAssignmentsFromCourse(string doctorID) {
    vector<course> courses = GetPersonCourse(doctorID);
    char yOrn;
    if (courses.empty()) {
        cout << "No courses found.\n";
        return;
    }
    for (int i = 0 ; i < courses.size() ; i++) {
        cout<<i+1<<") Course "<<left<<setw(10)<<courses.at(i).name;
        cout<<" -  Code: "<<courses.at(i).code<<endl;
    }
    int choice = validateChoice(1, courses.size(), "Enter Course Number To View it: ");
    cout<<endl;
    course selectedCourse = courses.at(choice - 1);
    if (selectedCourse.assignmentsIDs.empty()) {
        cout << "No assignments for this course.\n";
    } else {
        cout << "Assignments:\n";
        for (int i = 0; i < selectedCourse.assignmentsIDs.size(); i++) {
            assignment currentAssignment = getAssignment(selectedCourse.assignmentsIDs.at(i));
            cout << i + 1 << ") " << currentAssignment.question << endl;
            cout << "Answers:\n";
            for (int j = 0; j < currentAssignment.answers.size(); j++) {
                cout << " " << j + 1 << ") " << currentAssignment.answers.at(j) << endl;
            }
            cout << "Correct Answer: " << currentAssignment.answers.at(currentAssignment.correctAnswer) << endl;
            cout << "Students who solved this assignment: ";
            if (currentAssignment.studentsSolve.empty()) {
                cout << "No students solved this assignment.\n\n";
            } else {
                for (string studentID : currentAssignment.studentsSolve) {
                    vstr parts = split(studentID, '=');
                    if (parts.at(1) == "1") {
                        cout << parts.at(0) << " (Solved) ";
                    } else {
                        cout << parts.at(0) << " (Not Solved) ";
                    }
                }
                cout<<endl<<endl;
            }
        }
    }
    doctor(doctorID);
    return;
}

doctors getDoctor(string doctorID) {
    string line;
    doctors doctor;
    vstr fields;
    ifstream file("Data/doctors.csv");
    while (getline(file, line)) {
        fields = split(line, ',');
        if (fields.at(0) == doctorID) {
            break;
        }
    }
    file.close();
    doctor.name = fields.at(1);
    doctor.courses = split(fields.at(5),'/');
    return doctor;
}

void doctor(string ID) {
    cout<<"\nWelcome Doctor "<<ID<<". What are you want to do today?\n\n";
    cout<<"1- Create a Course\n"
        <<"2- Add Assignment\n"
        <<"3- List my Courses\n"
        <<"4- Show Assignments\n"
        <<"5- Log out\n";
    int choice = validateChoice(1,5,"Enter Choice: ");
    cout<<endl;
    switch (choice) {
        case 1:
            createCourse(ID);
            break;
        case 2:
            addAssignmentToCourse(ID);
            break;
        case 3:
            listCourses(ID);
            break;
        case 4:
            showAssignmentsFromCourse(ID);
            break;
        default:
            welcome();
            break;
    }
}

#endif // DOCTORS_H_INCLUDED