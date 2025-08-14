#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

// Librarys
#include <cstdlib> // exit 
#include <string>
#include <vector>  // vector
#include <fstream> // ifstream / ofstream
#include <cctype>
#include <iostream>
#include <regex>
using std::string;
using std::vector;
using namespace std;
// typedef
typedef vector<string> vstr;
string PersonID;
// Structures and variables
struct course {
    string code;
    string name;
    string doctorID;
    vstr studentsIDs;
    vstr assignmentsIDs;
    string password;
};
struct students {
    string name;
    vstr courses;
    vstr friends;
    vstr notifications;
};
struct doctors {
    string name;
    vstr courses;
};
struct assignment{
    string id;
    string question;
    vstr answers;
    int correctAnswer;
    vstr studentsSolve;
};
#include "doctors.h"
#include "students.h"
void welcome();

int validateChoice(int startNum, int endNum, string msg) {
    int choice;

    while (true) {
        cout<<msg;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
        }
        else if (choice < startNum || choice > endNum) {
            cout << "Out of range. Please choice between " << startNum << " and " << endNum << endl;
        }
        else {
            break;
        }
    }

    return choice;
}

vstr split(string text, char character) {
    vstr arr;

    if (text.empty() || text == " ") {
        return arr;
    }

    string currentPart = "";
    text += character;

    for (int i = 0 ; i < text.length() ; i++) {
        if (text[i] == character) {
            arr.push_back(currentPart);
            currentPart = "";
        } else {
            currentPart += text[i];
        }
    }
    return arr;
}

string join(vstr items, char delimiter) {
    if (items.size() == 0) {
        return "";
    }
    string result;
    for (int i = 0; i < items.size(); i++) {
        result += items[i];
        if (i < items.size() - 1)
            result += delimiter;
    }
    return result;
}

int stringToInt(string text) {
    string numStr = "";

    for (char ch : text) {
        if (isdigit(ch)) {
            numStr += ch;
        }
    }

    return numStr.empty() ? 0 : stoi(numStr);
}

bool isValidPassword(string password) {
    if (password.length() < 8) return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;

    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
    }

    return hasUpper && hasLower && hasDigit;
}

string getPassword() {
    string password;
    do {
        cin.clear();
        cin.ignore(10000,'\n');
        cout << "Enter a strong password (at least 8 chars, upper, lower, digit): ";
        cin >> password;

        if (!isValidPassword(password)) {
            cout << "Invalid password. Try again.\n";
        }

    } while (!isValidPassword(password));
    return password;
}

bool isValidEmail(string email) {
    regex pattern(R"(\w+@\w+\.\w+)");
    return regex_match(email, pattern);
}

bool usernameExistsInFile(string filename, string username) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        vstr fields = split(line, ',');
        if (fields[2] == username) {
            return true;
        }
    }
    return false;
}

bool isUsernameTaken(string username) {
    return usernameExistsInFile("Data/doctors.csv", username) ||
           usernameExistsInFile("Data/students.csv", username);
}

string getUniqueUsername() {
    string username;

    while (true) {
        cout << "Enter a username: ";
        cin >> username;

        if (isUsernameTaken(username)) {
            cout << "That username already exists. Try another one.\n";
        } else {
            cout << "Username is available!\n";
            break;
        }
    }

    return username;
}

string getUserID(string username, string password, string fileName) {
    ifstream file("Data/" + fileName + ".csv");
    string line;
    vstr fields;
    while (getline(file, line)) {
        fields = split(line, ',');
        if (line == "") continue;
        if (fields.at(2) == username && fields.at(3) == password) {
            file.close();
            return fields.at(0);
        }
    }
    file.close();
    return "";
}

void login() {
    string username;
    string password;

    cout<<"Username: ";
    cin>>username;

    cin.clear();
    cin.ignore(10000,'\n');
    cout<<"Password: ";
    cin>>password;
    
    PersonID = getUserID(username, password, "students");
    if (PersonID == "") {
        PersonID = getUserID(username, password, "doctors");
        if (PersonID[0] == 'd') {
            cout<<PersonID<<endl;
            doctor(PersonID);
        }else {
            cout<<"Your account does not exist\n";
            cout<<PersonID<<endl;
            welcome();
        }
    }else if (PersonID[0] == 's') {
        cout<<PersonID<<endl;
        student(PersonID);
    }else {
        cout<<"Your account does not exist\n";
        cout<<PersonID<<endl;
        welcome();
    }
}

void signUp() {
    string firstName, lastName;
    cout << "Enter your first name: ";
    cin >> firstName;
    cout << "Enter your last name: ";
    cin >> lastName;
    string username = getUniqueUsername();
    string password = getPassword();
    string email;
    do {
        cout << "Enter your email: ";
        cin >> email;
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Try again.\n";
        }
    } while (!isValidEmail(email));
    cout<<"1- Student\n"
        <<"2- Doctor\n";
    int choice = validateChoice(1, 2, "Enter your choice: ");
    cout<<endl;
    string fileName = (choice == 2) ? "doctors" : "students";
    string idPrefix = (choice == 2) ? "d" : "s";
    string infoLine;
    ifstream infoFile("Data/info.csv");
    getline(infoFile, infoLine);
    infoFile.close();
    string ID1 = split(infoLine, ',').at(choice);
    int IDNum = stringToInt(ID1);
    IDNum++;
    string PersonID = idPrefix + to_string(IDNum);

    string line = PersonID + "," + firstName + " " + lastName + "," + username + "," + password + "," + email + (idPrefix == "s"? ",,,": ",");


    ofstream outFile("Data/" + fileName + ".csv", ios::app);
    outFile << line << endl;
    outFile.close();
    
    ofstream infoOut("Data/info.csv");
    infoOut << split(infoLine, ',').at(0) << ","
            << (choice == 2 ? split(infoLine, ',').at(1) : PersonID) << ","
            << (choice == 1 ? split(infoLine, ',').at(2) : PersonID) << ","
            << split(infoLine, ',').at(3) << endl;
    infoOut.close();
    if (idPrefix == "d") {
        cout<<"Welcome Doctor "<<firstName<<" "<<lastName<<". What are you want to do today?\n\n";
        doctor(PersonID);
    }else {
        cout<<"Welcome "<<firstName<<" "<<lastName<<". What are you want to do today?\n\n";
        student(PersonID);
    }
}

void welcome() {
    cout<<"\nPlease make a choice\n"
        <<"1- Login\n"
        <<"2- Sign up\n"
        <<"3- Shutdown system\n";
    int choice = validateChoice(1,3,"Enter Choice: ");
    cout<<endl;
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            signUp();
            break;
        default:
            exit(0);
            break;
    }
}

#endif // FUNCTIONS_H_INCLUDED