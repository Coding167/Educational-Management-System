#ifndef ASSIGNMENTS_H_INCLUDED
#define ASSIGNMENTS_H_INCLUDED

#include <fstream>
#include <iostream>
#include <algorithm>
#include "functions.h"
#include "courses.h"
int stringToInt(string text);
vstr split(string text, char character);
using namespace std;

/*
1) Get Assignment Info          =>  assignment getAssignment(string id);
2) Get Assignments by Course    =>  vector<assignment> getAssignmentsByCourse(string courseID);
3) Add Assignment               =>  void addAssignment(assignment newAssignment, string courseCode);
*/

assignment getAssignment(string id) {
    ifstream file("Data/assignments.csv");
    string line;
    assignment assignmentInfo;

    while (getline(file, line)) {
        vstr fields = split(line, ',');
        if (fields.at(0) == id) {
            assignmentInfo.id = fields.at(0);
            assignmentInfo.question = fields.at(1);
            assignmentInfo.answers = split(fields.at(2), '/');
            assignmentInfo.correctAnswer = stringToInt(fields.at(3));
            assignmentInfo.studentsSolve = split(fields.at(4), '/');
            break;
        }
    }
    file.close();
    return assignmentInfo;
}

vector<assignment> getAssignmentsByCourse(string courseID) {
    vstr assignments = getCourse(courseID).assignmentsIDs;
    vector<assignment> assignmentList;
    if (assignments.empty()) {
        cout << "No assignments found for this course.\n";
        return {};
    }
    for (int i = 0 ; i < assignments.size() ; i++) {
        assignmentList.push_back(getAssignment(assignments.at(i)));
    }
    return assignmentList;
}

void addAssignment(assignment newAssignment, string courseCode) {
    ifstream infoFile("Data/info.csv");
    string infoLine;
    getline(infoFile, infoLine);
    infoFile.close();
    vstr infoParts = split(infoLine, ',');
    int assignmentID = stringToInt(infoParts.at(3)) + 1; // Increment the ID
    newAssignment.id = "a" + to_string(assignmentID); // Assign new ID
    ofstream infoOut("Data/info.csv"); 
    infoOut << infoParts.at(0) << "," 
            << infoParts.at(1) << "," 
            << infoParts.at(2) << "," 
            << newAssignment.id <<"\n";
    infoOut.close();

    ofstream file("Data/assignments.csv", ios::app);
    file << newAssignment.id << "," 
         << newAssignment.question << "," 
         << join(newAssignment.answers, '/') << "," 
         << newAssignment.correctAnswer << "," 
         << join(newAssignment.studentsSolve, '/') << "\n";
    file.close();

    string line;
    vstr lines;
    ifstream inFile("Data/courses.csv");
    while (getline(inFile, line)) {
        vstr parts = split(line, ',');
        if (parts[0] == courseCode) {
            vstr assignments = split(parts[5], '/');
            if (find(assignments.begin(), assignments.end(), newAssignment.id) == assignments.end()) {
                assignments.push_back(newAssignment.id);
                sort(assignments.begin(), assignments.end()); // keep it sorted
                parts[5] = join(assignments, '/');
                line = parts[0] + "," + parts[1] + "," + parts[2] + "," + parts[3] + "," + parts[4] + "," + parts[5];
            }
        }
        lines.push_back(line);
    }
    inFile.close();
    ofstream outFile("Data/courses.csv");
    for (const string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
}

void addStudentSolve(string assignmentID, string studentID, bool solved) {
    assignment currentAssignment = getAssignment(assignmentID);
    vstr students = currentAssignment.studentsSolve;
    string studentInfo = studentID + "=" + (solved ? "1" : "0");
    students.push_back(studentInfo);
    sort(students.begin(), students.end());
    ifstream file("Data/assignments.csv");
    vstr lines;
    string line1;
    while(getline(file, line1)) {
        lines.push_back(line1);
    }
    file.close();
    string line = currentAssignment.id + "," 
                    + currentAssignment.question + "," 
                    + join(currentAssignment.answers, '/') + "," 
                    + to_string(currentAssignment.correctAnswer) + "," 
                    + join(students, '/');
    
    ofstream outFile("Data/assignments.csv");
    for (int i = 0 ; i < lines.size() ; i++) {
        string id = split(lines.at(i), ',').at(0);
        if (id == assignmentID) {
            outFile << line << "\n";
        } else {
            outFile << lines.at(i) << "\n";
        }
    }
    outFile.close();
}

bool isStudentSolvedAssignment(string assignmentID, string studentID) {
    assignment currentAssignment = getAssignment(assignmentID);
    vstr students = currentAssignment.studentsSolve;
    for (int i = 0; i < students.size(); i++) {
        vstr parts = split(students.at(i), '=');
        if (parts.at(0) == studentID && parts.at(1) == "1") {
            return true; // Return true if solved, false if not solved
        }
    }
    return false; // Student has not solved this assignment
}

bool isStudentInAssignment(string assignmentID, string studentID) {
    assignment currentAssignment = getAssignment(assignmentID);
    vstr students = currentAssignment.studentsSolve;
    for (int i = 0; i < students.size(); i++) {
        vstr parts = split(students.at(i), '=');
        if (parts.at(0) == studentID) {
            return true; // Return true if solved, false if not solved
        }
    }
    return false; // Student has not solved this assignment
}

#endif // ASSIGNMENTS_H_INCLUDED