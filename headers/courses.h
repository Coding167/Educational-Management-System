#ifndef COURSES_H_INCLUDED
#define COURSES_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "functions.h"
string join(vstr items, char delimiter);


/*
1) Get Course                =>   course getCourse(string courseCode);
2) Get Coursse of Person     =>   vector<course> GetStudentCourse(string personID)
3) Add Course                =>   void addCourse(string name, string docID)
4) Add Student To Course     =>   void addStudentToCourse(string courseCode, string studentId)
5) Add Assignment To Course  =>   void addAssignmentToCourse(string courseCode, string assignmentId)
*/

course getCourse(string courseCode) {
    string line;
    course Info;
    bool found = false;
    ifstream file("Data/courses.csv");

    while (getline(file, line)) {
        vstr parts = split(line, ',');
        if (parts[0] == courseCode) {
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Course not found.\n";
        return Info; // empty course
    }

    vstr parts = split(line, ',');
    if (parts.size() < 5) {
        cout << "Invalid course format.\n";
        return Info;
    }

    Info.code = parts.at(0);
    Info.name = parts.at(1);
    Info.doctorID = parts.at(2);

    // Process students
    vstr students = split(parts.at(3), '/');
    for (string studentID : students) {
        if (!studentID.empty()) {
            Info.studentsIDs.push_back(studentID);
        }
    }

    // Process assignments
    vstr assignments = split(parts.at(4), '/');
    for (string assignmentID : assignments) {
        if (!assignmentID.empty()) {
            Info.assignmentsIDs.push_back(assignmentID);
        }
    }

    return Info;
}

void addStudentToCourse(string courseCode, string studentId) {
    string line;
    vstr lines;

    ifstream inFile("Data/courses.csv");

    // Step 1: Read all lines and edit the target one
    while (getline(inFile, line)) {
        vstr parts = split(line, ',');
        if (parts[0] == courseCode) {
            vstr students = split(parts[3], '/');

            // Only add if not already in list
            if (find(students.begin(), students.end(), studentId) == students.end()) {
                students.push_back(studentId);
                sort(students.begin(), students.end());
                parts[3] = join(students, '/');

                // Rebuild updated line
                line = parts[0] + "," + parts[1] + "," + parts[2] + "," + parts[3] + "," + parts[4];
            }
        }
        lines.push_back(line); // Add (edited or original) line to vector
    }
    inFile.close();
    // Step 2: Overwrite the same file with updated lines
    ofstream outFile("Data/courses.csv");
    for (string l : lines) {
        outFile << l << endl;
    }
    outFile.close();

    string line1;
    vstr lines1;
    string studentline;

    ifstream inFile1("Data/students.csv");

    // Step 1: Read all lines and edit the target one
    while (getline(inFile1, line1)) {
        vstr parts = split(line1, ',');
        if (parts[0] == studentId) {
            vstr courses = split(parts[5], '/');

            // Only add if not already in list
            if (find(courses.begin(), courses.end(), courseCode) == courses.end()) {
                courses.push_back(courseCode);
                sort(courses.begin(), courses.end());
                parts[5] = join(courses, '/');
            }

            // Rebuild updated line
            line1 = parts[0] + "," + parts[1] + "," + parts[2] + "," + parts[3] + "," + parts[4] + "," + parts[5];
        }
        lines1.push_back(line1); // Add (edited or original) line to vector
    }
    inFile1.close();
    // Step 2: Overwrite the same file with updated lines
    ofstream outFile1("Data/students.csv");
    for (string l : lines1) {
        string id0 = split(l, ',').at(0);
        if (id0 == studentId) {
            outFile1 << id0 << "," << split(l, ',').at(1) << "," << split(l, ',').at(2) << "," 
                   << split(l, ',').at(3) << "," << split(l, ',').at(4) << "," 
                   << join(split(split(l, ',').at(5),'/'), '/') << endl;
        } else {
            outFile1 << l << endl;
        }
    }
    outFile1.close();
}

void addAssignmentToCourse(string courseCode, string assignmentId) {
    string line;
    vstr lines;

    ifstream inFile("Data/courses.csv");

    while (getline(inFile, line)) {
        vstr parts = split(line, ',');
        if (parts[0] == courseCode) {
            vstr assignments = split(parts[4], '/');

            // Add assignment if it doesn't already exist
            if (find(assignments.begin(), assignments.end(), assignmentId) == assignments.end()) {
                assignments.push_back(assignmentId);
                sort(assignments.begin(), assignments.end()); // keep it sorted
                parts[4] = join(assignments, '/');

                // rebuild the full updated line
                line = parts[0] + "," + parts[1] + "," + parts[2] + "," + parts[3] + "," + parts[4];
            }
        }
        lines.push_back(line);
    }

    inFile.close();

    ofstream outFile("Data/courses.csv");

    for (const string& l : lines) {
        outFile << l << endl;
    }

    outFile.close();
    
}

void addCourse(string name, string docID) {
    // Step 1: Read current info from info.csv
    ifstream infoFile("Data/info.csv");
    string infoLine;
    getline(infoFile, infoLine);
    infoFile.close();

    vstr infoParts = split(infoLine, ',');
    string lastCourseCode = infoParts[0];  // example: c5

    // Step 2: Generate new course code
    int courseNum = stringToInt(lastCourseCode); // 5
    courseNum++; // 6
    string newCourseCode = "c" + to_string(courseNum); // c6

    // Step 3: Update info.csv with new course code
    infoParts[0] = newCourseCode;
    ofstream infoOut("Data/info.csv");
    infoOut << join(infoParts, ',') << endl;
    infoOut.close();

    // Step 4: Add new course to courses.csv
    ofstream coursesFile("Data/courses.csv", ios::app); // append mode
    coursesFile << newCourseCode << "," << name << "," << docID << "," << "," << endl;
    coursesFile.close();

    string line1;
    vstr lines1;
    string studentline;

    ifstream inFile1("Data/doctors.csv");

    // Step 1: Read all lines and edit the target one
    while (getline(inFile1, line1)) {
        vstr parts = split(line1, ',');
        if (parts[0] == PersonID) {
            vstr courses = split(parts[5], '/');

            courses.push_back(newCourseCode);
            sort(courses.begin(), courses.end());
            parts[5] = join(courses, '/');

            // Rebuild updated line
            line1 = parts[0] + "," + parts[1] + "," + parts[2] + "," + parts[3] + "," + parts[4] + "," + parts[5];
        }
        lines1.push_back(line1); // Add (edited or original) line to vector
    }
    inFile1.close();
    // Step 2: Overwrite the same file with updated lines
    ofstream outFile1("Data/doctors.csv");
    for (string l : lines1) {
        string id0 = split(l, ',').at(0);
        if (id0 == PersonID) {
            outFile1 << id0 << "," << split(l, ',').at(1) << "," << split(l, ',').at(2) << "," 
                   << split(l, ',').at(3) << "," << split(l, ',').at(4) << "," 
                   << join(split(split(l, ',').at(5),'/'), '/') << endl;
        } else {
            outFile1 << l << endl;
        }
    }
    outFile1.close();

    cout << "Course added successfully: " << newCourseCode << " - " << name << endl;
}

vector<course> GetPersonCourse(string personID) {
    string fileName = personID[0] == 's' ? "students" : "doctors";
    string infoLine;
    vstr coursesCodes;
    vector<course> Courses;
    ifstream file("Data/" + fileName + ".csv");

    // Find the student line
    while (getline(file, infoLine)) {
        string id = split(infoLine, ',').at(0);
        if (id == personID) {
            // Get the 6th field (course codes)
            vstr parts = split(infoLine, ',');
            if (parts.size() == 4) {
                return {};
            }else {
                coursesCodes = split(parts.at(5), '/');
                sort(coursesCodes.begin(), coursesCodes.end());
            }
            
            course myCourse;
            for (int i = 0 ; i < coursesCodes.size() ; i++) {
                myCourse = getCourse(coursesCodes.at(i));
                Courses.push_back(myCourse);
            }

            break;
            return Courses;
        }
    }
    file.close();
    return Courses;
    
}

vector<course> GetCoursesNOTstudentCourses(string studentID) {
    vector<course> filteredCourses;
    vector<course> studentCoursesCodes = GetPersonCourse(studentID);
    vstr studentCoursesIDs;
    for (int i = 0 ; i < studentCoursesCodes.size() ; i++) {
        studentCoursesIDs.push_back(studentCoursesCodes.at(i).code);
    }
    ifstream file("Data/courses.csv");
    string line;

    // Read all courses
    while (getline(file, line)) {
        string code = split(line, ',').at(0);
        if (find(studentCoursesIDs.begin(), studentCoursesIDs.end(), code) == studentCoursesIDs.end()) {
            course c;
            vstr parts = split(line, ',');
            c.code = parts.at(0);
            c.name = parts.at(1);
            c.doctorID = parts.at(2);
            c.studentsIDs = split(parts.at(3), '/');
            c.assignmentsIDs = split(parts.at(4), '/');
            filteredCourses.push_back(c);
        }
    }

    file.close();

    return filteredCourses;
}

#endif // COURSES_H_INCLUDED