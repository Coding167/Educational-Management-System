#ifndef STUDENTS_H_INCLUDED
#define STUDENTS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <iomanip>
#include "assignments.h"
#include <algorithm>

using namespace std;
students studentInfo1;
bool notificationBool;
int validateChoice(int startNum, int endNum, string msg);
vstr split(string text, char character);
string join(vstr items, char delimiter);
vector<course> GetPersonCourse(string studentID);
vector<course> GetCoursesNOTstudentCourses(string studentID);
vector<course> myCourses;
void student(string ID);
students getStudent(string studentID);

// s1 , Moaz Mohammed , m0az$3 , m0aZ1$fa , moaz@gmail.com , c1 , s2 , s3

void myCoursesList(string studentID) {
    myCourses = GetPersonCourse(studentID);
    if (myCourses.empty()) {
        cout<<"You have not registered in any course yet.\n\n";
        student(studentID);
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
    cout<<"Course "<<selectedCourse.name;
    cout<<" - Code "<<selectedCourse.code<<endl;
    cout<<"Created by Dr "<<getDoctor(selectedCourse.doctorID).name<<endl;
    cout<<"\tcourse has "<<selectedCourse.studentsIDs.size()<<" Student"<<(selectedCourse.studentsIDs.size() == 1 ? "\n" : "s\n");
    cout<<"\tand "<<selectedCourse.assignmentsIDs.size()<<" Assignment"<<(selectedCourse.assignmentsIDs.size() == 1 ? "\n" : "s\n");
    cout<<endl;
    student(studentID);
}

void reqisterInCourse(string studentID) {
    vector<course> courses = GetCoursesNOTstudentCourses(studentID);
    if (courses.empty()) {
        cout<<"You have registered in all courses.\n\n";
        student(studentID);
        return;
    }
    int size = courses.size();
    for (int i = 0 ; i < size ; i++) {
        cout<<i+1<<") Course "<<left<<setw(15)<<courses.at(i).name;
        cout<<" -  Code "<<courses.at(i).code<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1, size, "Choose a course to register [1 - " + to_string(size) + "]: ");
    cout<<endl;
    string coursePassword = courses.at(choice - 1).password;
    string password;
    cout<<endl<<"Enter the password of the course: ";
    cin>>password;
    if (password == coursePassword) {
        addStudentToCourse(courses.at(choice - 1).code, studentID);
        cout<<"You have registered in course successfully.\n\n";
    } else {
        cout << "Invalid Password.\n";
        student(studentID);
        return;
    }
    student(studentID);
    return;
}

void solveAssignment(string studentID) {
    vector<course> courses = GetPersonCourse(studentID);
    vstr solvedAssignments;
    int solved = 0;
    if (courses.empty()) {
        cout<<"You have not registered in any course yet.\n\n";
        student(studentID);
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
        student(studentID);
        return;
    }
    int assignmentSize = selectedCourse.assignmentsIDs.size();
    for (int i = 0 ; i < assignmentSize ; i++) {
        if (isStudentInAssignment(selectedCourse.assignmentsIDs.at(i), studentID)) {
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
            addStudentSolve(currentAssignment.id, studentID, true);
            solvedAssignments.push_back("true");
            solved++;
        } else {
            cout<<"Your answer is incorrect.\n";
            addStudentSolve(currentAssignment.id, studentID, false);
            solvedAssignments.push_back("false");
        }
    }
    cout<<endl;
    if (solvedAssignments.empty()) {
        student(studentID);
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
    student(studentID);
    return;
}

void assignmentReport(string studentID) {
    vector<course> courses = GetPersonCourse(studentID);
    int assignmentSolved = 0;
    if (courses.empty()) {
        cout << "You have not registered in any course yet.\n\n";
        student(studentID);
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
        student(studentID);
        return;
    }
    bool solved = false;
    int assignmentSize = selectedCourse.assignmentsIDs.size();
    for (int i = 0; i < assignmentSize; i++) {
        assignment currentAssignment = getAssignment(selectedCourse.assignmentsIDs.at(i));
        solved = isStudentSolvedAssignment(currentAssignment.id, studentID);
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
    student(studentID);
}

void listFriends(string studentID) {
    vstr friends = studentInfo1.friends;
    if (friends.empty()) {
        cout<<"you are havn't friends yet.\n";
        student(studentID);
        return;
    }
    for (int i = 0 ; i < friends.size() ; i++) {
        string name = getStudent(friends.at(i)).name;
        cout<<i+1<<". "<<name<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1,friends.size(),"Enter number of your friend to show his information: ");
    cout<<endl;
    students currentStudent = getStudent(friends.at(choice - 1));
    int coursesSize = currentStudent.courses.size();
    cout<<"Name "<<currentStudent.name<<endl;
    cout<<"He in "<<coursesSize<<" Course"<<(coursesSize == 1 ? "\n" : "s\n");
    for (int i = 0 ; i < coursesSize ; i++) {
        string courseName = getCourse(currentStudent.courses.at(i)).name;
        cout<<i+1<<". "<<courseName<<endl;
    }
    student(studentID);
}

void addFriend(string studentID) {
    vstr friends = studentInfo1.friends;
    friends.push_back(studentID);
    string line;
    vstr lines;
    vstr allStudents;
    ifstream fileIn("Data/students.csv");
    while (getline(fileIn,line)) {
        string id = split(line,',').at(0);
        if (find(friends.begin(), friends.end(), id) != friends.end()) {
            allStudents.push_back(line);
            continue;
        }
        allStudents.push_back(line);
        lines.push_back(line);
    }
    fileIn.close();
    int size = lines.size();

    if (size == 0) {
        cout<<"You have all friends.\n";
        student(studentID);
        return;
    }

    for (int i = 0 ; i < size ; i++) {
        string name = getStudent(split(lines.at(i),',').at(0)).name;
        cout<<i+1<<". "<<name<<endl;
    }
    int choice = validateChoice(1,size+1,"Enter the number of Friend to show it (if you want to exit write "+to_string(size+1)+" ): ");
    cout<<endl;
    if(choice == size+1) {
        student(studentID);
        return;
    }
    string id = split(lines.at(choice - 1),',').at(0);
    students currentStudent = getStudent(id);
    cout<<"Name "<<currentStudent.name<<endl;
    cout<<"He in "<<currentStudent.courses.size()<<" Course"<<(currentStudent.courses.size() == 1 ? "\n" : "s\n");
    cout<<"and "<<currentStudent.friends.size()<<" Friend"<<(currentStudent.friends.size() == 1 ? "\n" : "s\n");

    char yOrn;
    while (true) {
        cout<<endl<<"Do you want to add an assignment to this course? (y/n): ";
        cin>>yOrn;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
         else {
            break;
        }
    }
    if (yOrn == 'y' || yOrn == 'Y') {
        ofstream fileOut("Data/students.csv");
        for (string l : allStudents) {
            vstr parts = split(l,',');
            if (parts.at(0) == id) {
                currentStudent.notifications.push_back(studentID);
                sort(currentStudent.notifications.begin(),currentStudent.notifications.end());
                fileOut<<parts.at(0)<<','<<parts.at(1)<<','<<parts.at(2)<<','<<parts.at(3)<<','<<parts.at(4)<<','<<parts.at(5)<<','<<parts.at(6)<<','<<join(currentStudent.notifications,'/')<<endl;
            }else {
                fileOut<<l<<endl;
            }
        }
        fileOut.close();
        student(studentID);
        return;
    }else if (yOrn == 'n' || yOrn == 'N') {
        student(studentID);
        return;
    }else {
        cout << "Invalid input. Please enter 'y' or 'n'.\n";
        student(studentID);
        return;
    }
}

void removeNotification(string studentID, string notificID) {
    string line;
    vstr lines;
    ifstream fileIn("Data/students.csv");
    while (getline(fileIn,line)) {
        lines.push_back(line);
    }
    fileIn.close();

    ofstream fileOut("Data/students.csv");
    for (string l : lines) {
        vstr parts = split(l,',');
        if (parts.at(0) == studentID) {
            vstr notifications = studentInfo1.notifications;
            int idx = 0;
            for (int i = 0 ; i < notifications.size() ; i++) {
                if (notifications.at(i) == notificID) {
                    idx = i;
                    break;
                }
                idx = -1;
            }
            if (idx == -1) {
                return;
            }
            notifications.erase(notifications.begin() + idx); // to remove notificID from notifications of student
            studentInfo1.notifications.erase(studentInfo1.notifications.begin() + idx); // to remove notificID from notifications of student
            vstr friends = split(parts.at(6), '/');
            friends.push_back(notificID);
            sort(friends.begin(),friends.end());
            studentInfo1.friends = friends;
            fileOut<<parts.at(0)<<','<<parts.at(1)<<','<<parts.at(2)<<','<<parts.at(3)<<','<<parts.at(4)<<','<<parts.at(5)<<','<<join(friends,'/')<<','<<join(notifications,'/')<<endl;
        }else {
            fileOut<<l<<endl;
        }
    }
    fileOut.close();
}

void addFriendToStudent(string studentID, string notificID) {
    string line;
    vstr lines;
    ifstream fileIn("Data/students.csv");
    while (getline(fileIn,line)) {
        lines.push_back(line);
    }
    fileIn.close();

    ofstream fileOut("Data/students.csv");
    for (string l : lines) {
        vstr parts = split(l,',');
        if (parts.at(0) == notificID) {
            vstr friends = split(parts.at(6), '/');
            friends.push_back(studentID);
            sort(friends.begin(),friends.end());
            fileOut<<parts.at(0)<<','<<parts.at(1)<<','<<parts.at(2)<<','<<parts.at(3)<<','<<parts.at(4)<<','<<parts.at(5)<<','<<join(friends,'/')<<','<<parts.at(7)<<endl;
        }else {
            fileOut<<l<<endl;
        }
    }
    fileOut.close();
}

students getStudent(string studentID) {
    string line;
    students studentInfo;
    vstr parts;
    ifstream studentFile("Data/students.csv");
    bool found = false;
    while(getline(studentFile,line)) {
        parts = split(line,',');
        if (parts.size() > 0 && parts.at(0) == studentID) {
            found = true;
            break;
        }
    }
    studentFile.close();
    if (!found || parts.size() < 2) {
        return studentInfo;
    }
    studentInfo.name = parts.at(1);
    if (parts.size() <= 5) {
        studentInfo.courses = {};
    } else {
        studentInfo.courses = split(parts.at(5),'/');
    }
    if (parts.size() <= 6) {
        studentInfo.friends = {};
    } else {
        studentInfo.friends = split(parts.at(6),'/');
    }
    if (parts.size() <= 7) {
        studentInfo.notifications = {};
    } else {
        studentInfo.notifications = split(parts.at(7),'/');
    }
    return studentInfo;
}

void getNotifications(string studentID,bool msg=false) {
    vstr notifications = studentInfo1.notifications;
    if (notifications.empty()) {
        if (msg) {
            cout<<"You havn't notifications yet."<<endl;
            notificationBool = true;
            student(studentID);
            return;
        }
        notificationBool = true;
        student(studentID);
        return;
    }
    int size = notifications.size();
    cout<<"You have "<<size<<" Notification"<<(size == 1 ? "\n" : "s\n");
    for (int i = 0 ; i < size ; i++) {
        string name = getStudent(notifications.at(i)).name;
        cout<<i+1<<". "<<name<<endl;
    }
    cout<<endl;
    int choice = validateChoice(1,size+1,"Enter the number of Notification to apply it (if you want to exit write "+to_string(size+1)+" ): ");
    cout<<endl;
    if (choice == size+1) {
        notificationBool = true;
        student(studentID);
        return;
    }
    string currentID = notifications.at(choice-1);
    cout<<currentID<<endl;
    removeNotification(studentID,currentID);
    addFriendToStudent(studentID,currentID);
}

void student(string ID) {
    studentInfo1 = getStudent(ID);
    // cout<<"\nWelcome "<<studentInfo1.name<<". What are you want to do today?\n\n";
    if (notificationBool) {
        cout<<"";
    }else {
        getNotifications(ID);
    }
    cout<<"\nPlease make a choice:\n"
        <<"1- Register in Course\n"
        <<"2- List My Courses\n"
        <<"3- Solve Assignment\n"
        <<"4- Assignment Report\n"
        <<"5- Add Friend\n"
        <<"6- List my Friends\n"
        <<"7- Get Notifications\n"
        <<"8- Log out\n";
    int choice = validateChoice(1,8,"Enter Choice: ");
    cout<<endl;
    switch (choice) {
        case 1:
            reqisterInCourse(ID);
            break;
        case 2:
            myCoursesList(ID);
            break;
        case 3:
            solveAssignment(ID);
            break;
        case 4:
            assignmentReport(ID);
            break;
        case 5:
            addFriend(ID);
            break;
        case 6:
            listFriends(ID);
            break;
        case 7:
            getNotifications(ID,true);
            break;
        default:
            welcome();
            break;
    }
}

#endif // STUDENTS_H_INCLUDED