#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <cstdlib> // exit 
#include <string>
#include <vector>  // vector
#include <fstream> // ifstream / ofstream
#include <cctype>
#include <iostream>
#include <regex>
// using std
using std::string;
using std::vector;
using namespace std;
// typedef
typedef vector<string> vstr;
// Structures and variables
struct course {
    string code;
    string name;
    string doctorID;
    vstr studentsIDs;
    vstr assignmentsIDs;
};
struct person {
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

int stringToInt(string text);
vstr split(string text, char character);
int validateChoice(int startNum, int endNum, string msg);
void welcome();
void signUp();
void login();
string join(vstr items, char delimiter);
string PersonID;
person PersonInfo;
#include "doctors.h"
#include "students.h"



/*
main functions
01) Validate any integer Choice   =>  int validateChoice(int startNum, int endNum, string msg);
02) Split any Text                =>  vstr split(string text, char character);
03) Join words by charcter        =>  string join(vstr items, char delimiter);
04) String to Integer             =>  int stringToInt(string text);
05) Is Password Valid             =>  bool isValidPassword(string password);
06) Is Email Valid                =>  bool isValidEmail(string email);
07) Did username in this file     =>  bool usernameExistsInFile(string filename, string username);
08) Is username taken             =>  bool isUsernameTaken(string username);
09) Get unique Username           =>  string getUniqueUsername();
10) Welcome section               =>  void welcome();
11) log in you account            =>  void login();
12) Get unique Password           =>  string getPassword();
13) Get user id                   =>  string getUserID(string username, string password, string fileName);
14) Get name and courses of person=>  person getInfo();
*/

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

person getInfo() {
    string fileName = PersonID[0] == 's' ? "students" : "doctors";
    ifstream file("Data/" + fileName + ".csv");
    string line;
    person PersonInfo1;
    vstr fields;

    while (getline(file, line)) {
        fields = split(line, ',');
        if (fields.at(0) == PersonID) {
            PersonInfo1.name = fields.at(1);
            if (fields.size() == 5) {
                PersonInfo1.courses = {};
            }else {
                PersonInfo1.courses = split(fields.at(5),'/');
            }
            file.close();
            break;
            return PersonInfo1;
        }
    }
    file.close();
    return PersonInfo1;
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
            PersonInfo = getInfo();
            cout<<"\nWelcome Doctor "<<PersonInfo.name<<". What are you want to do today?\n\n";
            doctor(PersonID);
        }else {
            cout<<"Your account does not exist\n";
            welcome();
        }
    }else if (PersonID[0] == 's') {
        PersonInfo = getInfo();
        cout<<"\nWelcome "<<PersonInfo.name<<". What are you want to do today?\n\n";
        // student(PersonID);
    }else {
        cout<<"Your account does not exist\n";
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
    ofstream outFile("Data/" + fileName + ".csv", ios::app);
    outFile << PersonID << "," << firstName << " " << lastName << "," << username << "," 
            << password << "," << email << ",," << endl;
    outFile.close();
    ofstream infoOut("Data/info.csv");
    infoOut << split(infoLine, ',').at(0) << ","
            << (choice == 2 ? split(infoLine, ',').at(1) : PersonID) << ","
            << (choice == 1 ? split(infoLine, ',').at(2) : PersonID) << ","
            << split(infoLine, ',').at(3) << "," << endl;
    infoOut.close();
    if (PersonID[0] == 'd') {
        cout<<"Welcome Doctor "<<firstName<<" "<<lastName<<". What are you want to do today?\n\n";
        doctor(PersonID);
    }else {
        cout<<"Welcome "<<firstName<<" "<<lastName<<". What are you want to do today?\n\n";
        // student(PersonID);
    }
}

#endif // FUNCTIONS_H_INCLUDED