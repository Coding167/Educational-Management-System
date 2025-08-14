#include "../headers/functions.h"
#include "../headers/student.h"
#include "../headers/doctor.h"
#include <fstream>
#include <regex>
#include <cctype>
// File Class
void File::setPath(std::string path) {
    this->path = path;
}

std::string File::getPath() const {
    return this->path;
} 

std::vector<std::string> File::read() {
    std::vector<std::string> lines;
    std::string line;

    std::ifstream file(path);
    while(getline(file,line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

std::string File::readLine(int lineNum) {
    std::string line;
    int i = 0;

    std::ifstream file(path);
    while(getline(file,line)) {
        i++;
        if (i == lineNum) {
            file.close();
            return line;
        }
    }
    file.close();
    return "";
}

void File::addLine(std::string line) {
    std::ofstream file(path,std::ios::app);
    file << line << std::endl;
    file.close();
}

void File::write(std::vector<std::string> lines, bool append) {
    std::ofstream file;

    if (append) {
        file.open(path, std::ios::app);
    }else {
        file.open(path);
    }

    for (std::string line: lines) {
        file << line << std::endl;
    }
    file.close();
}

void File::writeLine(std::string line) {
    std::ofstream file(path);
    file << line << std::endl;
    file.close();
}

void File::replaceLine(int lineNum, std::string line) {
    std::vector<std::string> lines = read();
    if (lineNum >= 1 && lineNum <= lines.size()) {
        lines.at(lineNum - 1) = line;
    }
    write(lines,false);
}

// some functions

std::vector<std::string> split(std::string text, char character) {
    text += character;
    std::vector<std::string> result;
    std::string word;
    for (char c: text) {
        if (c == character) {
            result.push_back(word);
            word = "";
        }else {
            word += c;
        }
    }
    return result;
}

std::string join(std::vector<std::string> words, char character) {
    std::string result;
    for (std::string word : words) {
        if (!word.empty()) {
            result += word + character;
        }
    }
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}


int stringToInt(std::string number) {
    std::string digits = "";

    for (char ch : number) {
        if (std::isdigit(ch)) {
            digits += ch;
        }
    }

    // Handle the case where no digits were found
    if (digits.empty()) {
        return 0; // or throw custom error or return -1, depending on what you want
    }

    return std::stoi(digits);
}


int validateChoice(int startNum, int endNum, std::string msg) {
    int choice;

    while (true) {
        std::cout<<msg;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        }
        else if (choice < startNum || choice > endNum) {
            std::cout << "Out of range. Please choice between " << startNum << " and " << endNum << std::endl;
        }
        else {
            break;
        }
    }

    return choice;
}

std::string userId(std::string username, std::string password, std::string path) {
    std::ifstream file(path);
    std::string line;
    while (getline(file,line)) {
        std::vector<std::string> parts = split(line,',');
        if (parts.at(2) == username && parts.at(3) == password) {
            file.close();
            return parts.at(0);
        }
    }
    file.close();
    return "none";
}

bool isValidPassword(std::string password) {
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

std::string getPassword() {
    std::string password;
    do {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        std::cout << "Enter a strong password (at least 8 chars, upper, lower, digit): ";
        std::cin >> password;

        if (!isValidPassword(password)) {
            std::cout << "Invalid password. Try again.\n";
        }

    } while (!isValidPassword(password));
    return password;
}

bool isValidEmail(std::string email) {
    std::regex pattern(R"(\w+@\w+\.\w+)");
    return std::regex_match(email, pattern);
}

bool usernameExistsInFile(std::string filename, std::string username) {
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line)) {
        std::vector<std::string> fields = split(line, ',');
        if (fields[2] == username) {
            return true;
        }
    }
    return false;
}

bool isUsernameTaken(std::string username) {
    return usernameExistsInFile("../data/doctors.csv", username) ||
           usernameExistsInFile("../data/students.csv", username);
}

std::string getUniqueUsername() {
    std::string username;

    while (true) {
        std::cout << "Enter a username: ";
        std::cin >> username;

        if (isUsernameTaken(username)) {
            std::cout << "That username already exists. Try another one.\n";
        } else {
            std::cout << "Username is available!\n";
            break;
        }
    }

    return username;
}


void welcome() {
    std::cout<<"\nPlease make a choice\n"
        <<"1- Login\n"
        <<"2- Sign up\n"
        <<"3- Shutdown system\n";
    int choice = validateChoice(1,3,"Enter Choice: ");
    std::cout<<std::endl;
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

void login() {
    std::string username, password;
    std::cout<<"Enter Username: ";
    std::cin>>username;
    std::cout<<"Enter Password: ";
    std::cin>>password;
    
    std::string personID = userId(username,password,"../data/doctors.csv");
    if (personID == "none") {
        personID = userId(username,password,"../data/students.csv");
        if (personID[0] == 's') {
            // studentStart(personID);
        }else {
            std::cout<<"You are not exist.\n";
            welcome();
        }
    } else if (personID[0] == 'd') {
        // doctorStart(personID);
    }else {
        std::cout<<"You are not exist.\n";
        welcome();
    }
}

void signUp() {
    std::string fname, lname, password, email, username;
    int sOrD;
    std::cout<<"Enter you first name: ";
    std::cin>>fname;
    std::cout<<"Enter you last name: ";
    std::cin>>lname;
    username = getUniqueUsername();
    password = getPassword();
    do {
        std::cout << "Enter your email: ";
        std::cin >> email;
        if (!isValidEmail(email)) {
            std::cout << "Invalid email format. Try again.\n";
        }
    } while (!isValidEmail(email));
    std::cout<<"1. Student\n2. Doctor\n";
    sOrD = validateChoice(1,2,"Who are you? ");
    std::string filename = (sOrD == 1 ? "../data/students.csv" : "../data/doctors.csv");
    char userPrefix = (sOrD == 1 ? 's' : 'd');

    File infoFile("../data/info.csv");
    std::vector<std::string> parts = split(infoFile.readLine(1),',');
    int id = stringToInt(parts.at(sOrD));
    id++;
    parts.at(sOrD) = userPrefix + std::to_string(id);
    infoFile.replaceLine(1,join(parts,','));
    File file(filename);
    std::string line = userPrefix + std::to_string(id) + ',' + fname + ' ' + lname + ',' + username + ',' + password + ',' + email + (userPrefix == 's' ?  ",,,":",");
    file.addLine(line);
    if (userPrefix == 's') {
        // studentStart(userPrefix + std::to_string(id));
    }else {
        // doctorStart(userPrefix + std::to_string(id));
    }
}
