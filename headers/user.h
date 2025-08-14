#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include <iostream>
#include <vector>

class Course;

class User {
    protected:
        int id;
        char userPrefix;
        std::string name = "";
        std::string username = "";
        std::string password;
        std::string email;
        std::vector<std::string> coursesIDs;
        std::string filePath;

    public:
        User(): id(0), userPrefix('p') {}

        std::string getID() const;
        std::string getName() const;
        std::string getUsername() const;
        std::vector<Course*> getCourses() const;

        virtual void setID(std::string id);
};

#endif // USER_H_INCLUDED