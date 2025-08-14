#ifndef DOCTORS_H_INCLUDED
#define DOCTORS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <iomanip>
#include "functions.h"
#include "assignments.h"
using namespace std;
void addCourse(string name, string docID);
vector<course> GetPersonCourse(string personID);
void doctor(string ID);


/*
1) Get Doctor Info              =>  person getDoctor(string id);
2) Menu of a doctor             =>  void doctor();
3) Create a course              =>  void createCourse();
4) List courses                 =>  void listCourses();
5) Add assignment to course     =>  void addAssignmentToCourse();
6) Show assignments from course =>  void showAssignmentsFromCourse();
*/

void createCourse(string PersonID) {
    string courseName;
    cout << "Enter Course Name: ";
    cin.clear();
    cin.ignore(10000, '\n'); 
    getline(cin, courseName);
    addCourse(courseName, PersonID);
    cout<<PersonID<<endl;
    cout << "Course created successfully.\n";
    doctor(PersonID);
}

void listCourses(string PersonID) {
    vector<course> courses = GetPersonCourse(PersonID);
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
    doctor(PersonID);
    return;
}

void addAssignmentToCourse(string PersonID) {
    vector<course> courses = GetPersonCourse(PersonID);
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
        cout << "Enrolled Students: ";
        for (const string& studentID : selectedCourse.studentsIDs) {
            cout << " " << studentID;
        }
    }
    cout<<endl;
    if (selectedCourse.assignmentsIDs.empty()) {
        cout << "No assignments for this course.\n";
    } else {
        cout << "Assignments: ";
        for (const string& assignmentID : selectedCourse.assignmentsIDs) {
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
    doctor(PersonID);
    return;
}

void showAssignmentsFromCourse(string PersonID) {
    vector<course> courses = GetPersonCourse(PersonID);
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
    doctor(PersonID);
    return;
}

person getDoctor(string id) {
    ifstream file("Data/doctors.csv");
    string line;
    person doctor;
    vstr fields;

    while (getline(file, line)) {
        fields = split(line, ',');
        if (fields.at(0) == id) {
            break;
        }
    }
    file.close();
    doctor.name = fields.at(1);
    doctor.courses = split(fields.at(5),'/');
    return doctor;
}

void doctor(string ID) {
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