#ifndef STUDENTS_H_INCLUDED
#define STUDENTS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <iomanip>
#include "functions.h"
#include "assignments.h"
// #include "courses.h"
using namespace std;
vector<course> GetPersonCourse(string personID);
vector<course> GetCoursesNOTstudentCourses(string studentID);
void addStudentToCourse(string courseCode, string studentId);
void student(string PersonID);
vector<course> myCourses;
/*
1) Menu of student             => void student();
2) List My Courses             => void myCoursesList();
3) Register in Course          => void reqisterInCourse();
4) Solve Assignment            => void solveAssignment();
5) Assignment Report           => void assignmentReport();
*/

void myCoursesList(string PersonID) {
    myCourses = GetPersonCourse(PersonID);
    if (myCourses.empty()) {
        cout<<"You have not registered in any course yet.\n\n";
        student(PersonID);
        return;
    }
    int size = myCourses.size();
    for (int i = 0 ; i < size ; i++) {
        cout<<i+1<<") Course "<<left<<setw(15)<<myCourses.at(i).name;
        cout<<" -  Code "<<myCourses.at(i).code<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1, size, "Choose a course to see more details [1 - " + to_string(size) + "]: ");
    cout<<endl;
    course selectedCourse = myCourses.at(choice - 1);
    cout<<"Course Name: "<<selectedCourse.name<<endl;
    cout<<"Course Code: "<<selectedCourse.code<<endl;
    cout<<"Doctor ID: "<<getDoctor(selectedCourse.doctorID).name<<endl;
    cout<<"\tStudents: ";
    if (selectedCourse.studentsIDs.empty()) {
        cout<<"This Course have not registered in any course yet.";
    } else {
        for (int i = 0 ; i < selectedCourse.studentsIDs.size() ; i++) {
            cout<<selectedCourse.studentsIDs.at(i)<<" ";
        }
    }
    cout<<endl;
    cout<<"\tAssignments: ";
    if (selectedCourse.assignmentsIDs.empty()) {
        cout<<"This Course have not any assignments in this course yet.\n";
    }
    else {
        for (int i = 0 ; i < selectedCourse.assignmentsIDs.size() ; i++) {
            cout<<selectedCourse.assignmentsIDs.at(i)<<" ";
        }
        cout<<endl;
    }
    student(PersonID);
}

void reqisterInCourse(string PersonID) {
    vector<course> courses = GetCoursesNOTstudentCourses(PersonID);
    if (courses.empty()) {
        cout<<"You have registered in all courses.\n\n";
        student(PersonID);
        return;
    }
    int size = courses.size();
    for (int i = 0 ; i < size ; i++) {
        cout<<i+1<<") Course "<<left<<setw(15)<<courses.at(i).name;
        cout<<" -  Code "<<courses.at(i).code<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1, size, "Choose a course to register [1 - " + to_string(size) + "]: ");
    cout<<choice;
    cout<<endl;
    addStudentToCourse(courses.at(choice - 1).code, PersonID);
    cout<<"You have registered in course successfully.\n\n";
    student(PersonID);
}

void solveAssignment(string PersonID) {
    vector<course> courses = GetPersonCourse(PersonID);
    vstr solvedAssignments;
    int solved = 0;
    if (courses.empty()) {
        cout<<"You have not registered in any course yet.\n\n";
        student(PersonID);
        return;
    }
    int size = courses.size();
    for (int i = 0 ; i < size ; i++) {
        cout<<i+1<<") Course "<<left<<setw(15)<<courses.at(i).name;
        cout<<" -  Code "<<courses.at(i).code;
        cout<<" -  Assignments Number "<<courses.at(i).assignmentsIDs.size()<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1, size, "Choose a course to solve assignment [1 - " + to_string(size) + "]: ");
    cout<<endl;
    course selectedCourse = courses.at(choice - 1);
    if (selectedCourse.assignmentsIDs.empty()) {
        cout<<"This course has no assignments yet.\n\n";
        student(PersonID);
        return;
    }
    int assignmentSize = selectedCourse.assignmentsIDs.size();
    for (int i = 0 ; i < assignmentSize ; i++) {
        if (isStudentSolvedAssignment(selectedCourse.assignmentsIDs.at(i), PersonID)) {
            cout<<"You have already solved this assignment.\n\n";
            continue;
        }
        assignment currentAssignment = getAssignment(selectedCourse.assignmentsIDs.at(i));
        int answer;
        cout<<"Start Your Assignment:"<<endl;
        cout<<i+1<<") Assignment "<<currentAssignment.question<<endl;
        cout<<"Answers: \n";
        for (int j = 0 ; j < currentAssignment.answers.size() ; j++) {
            cout<<j+1<<") "<<currentAssignment.answers.at(j)<<endl;
        }
        answer = validateChoice(1, currentAssignment.answers.size(), "Choose your answer [1 - " + to_string(currentAssignment.answers.size()) + "]: ");
        cout<<endl;
        if (answer == currentAssignment.correctAnswer + 1) {
            cout<<"Your answer is correct.\n";
            addStudentSolve(currentAssignment.id, PersonID, true);
            solvedAssignments.push_back("true");
            solved++;
        } else {
            cout<<"Your answer is incorrect.\n";
            addStudentSolve(currentAssignment.id, PersonID, false);
            solvedAssignments.push_back("false");
        }
    }
    cout<<endl;
    if (solvedAssignments.empty()) {
        student(PersonID);
        return;
    }
    for (int i = 0 ; i < solvedAssignments.size() ; i++) {
        if (solvedAssignments.at(i) == "true") {
            cout<<"Assignment "<<i+1<<" solved successfully.\n";
        } else {
            cout<<"Assignment "<<i+1<<" not solved successfully.\n";
        }
    }
    cout<<"You have solved "<<solved<<" assignments successfully out of "<<solvedAssignments.size()<<".\n\n";
    student(PersonID);
    return;
}

void assignmentReport(string PersonID) {
    vector<course> courses = GetPersonCourse(PersonID);
    int assignmentSolved = 0;
    if (courses.empty()) {
        cout << "You have not registered in any course yet.\n\n";
        student(PersonID);
        return;
    }
    int size = courses.size();
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ") Course " << left << setw(15) << courses.at(i).name;
        cout << " -  Code " << courses.at(i).code;
        cout << " -  Assignments Number " << courses.at(i).assignmentsIDs.size() << endl;
    }
    cout << endl;
    int choice = validateChoice(1, size, "Choose a course to see assignment report [1 - " + to_string(size) + "]: ");
    cout << endl;
    course selectedCourse = courses.at(choice - 1);
    if (selectedCourse.assignmentsIDs.empty()) {
        cout << "This course has no assignments yet.\n\n";
        student(PersonID);
        return;
    }
    bool solved = false;
    int assignmentSize = selectedCourse.assignmentsIDs.size();
    for (int i = 0; i < assignmentSize; i++) {
        assignment currentAssignment = getAssignment(selectedCourse.assignmentsIDs.at(i));
        solved = isStudentSolvedAssignment(currentAssignment.id, PersonID);
        // cout<<(solved ? "Yes" : "no")<<endl;
        
        cout << i + 1 << ") Assignment\n" << "Question: " << currentAssignment.question << endl;
        cout << "\tYour Answer: ";
        if (solved) {
            cout << "Solved Successfully.\n";
            assignmentSolved++;
        } else {
            cout << "Not Solved Yet.\n";
        }
        cout << endl;
    }
    cout << "You have solved " << assignmentSolved << " assignments successfully out of " << assignmentSize << ".\n";
    cout << endl;
    student(PersonID);
}

void student(string PersonID) {
    cout<<"\nPlease make a choice:\n"
        <<"1- Register in Course\n"
        <<"2- List My Courses\n"
        <<"3- Solve Assignment\n"
        <<"4- Assignment Report\n"
        <<"5- Log out\n";
    int choice = validateChoice(1,5,"Enter Choice: ");
    cout<<endl;
    switch (choice) {
        case 1:
            reqisterInCourse(PersonID);
            break;
        case 2:
            myCoursesList(PersonID);
            break;
        case 3:
            solveAssignment(PersonID);
            break;
        case 4:
            assignmentReport(PersonID);
            break;
        default:
            welcome();
            break;
    }
}

#endif // STUDENTS_H_INCLUDED