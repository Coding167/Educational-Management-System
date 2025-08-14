#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <iostream>
#include <vector>
// Some Functions
std::vector<std::string> split(std::string text, char character);
std::string join(std::vector<std::string> words, char character);
int stringToInt(std::string number);
int validateChoice(int startNum, int endNum, std::string msg);
std::string userId(std::string username, std::string password, std::string path);
bool isValidPassword(std::string password);
std::string getPassword();
bool isValidEmail(std::string email);
bool usernameExistsInFile(std::string filename, std::string username);
bool isUsernameTaken(std::string username);
std::string getUniqueUsername();

void welcome();
void signUp();
void login();

// File Class
class File {
    private:
        std::string path;

    public:
        File(): path("") {}
        File(std::string path): path(path) {}

        void setPath(std::string path);
        std::string getPath() const;

        std::vector<std::string> read();
        std::string readLine(int lineNum);

        void addLine(std::string line);
        void write(std::vector<std::string> lines, bool append);
        void writeLine(std::string line);

        void replaceLine(int lineNum, std::string line);
};

#endif // FUNCTIONS_H_INCLUDED