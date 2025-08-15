#include "../headers/user.h"
#include "../headers/functions.h"
#include "../headers/course.h"

std::string User::getID() const {
    return (this->userPrefix + std::to_string(this->id));
}
std::string User::getName() const {
    return this->name;
}
std::string User::getUsername() const {
    return this->username;
}
std::vector<Course*> User::getCourses() const {
    if (this->coursesIDs.empty()) return {};
    if (this->coursesIDs.size() == 1) {
        if (this->coursesIDs.at(0).empty()) {
            return {};
        }
    }
    std::vector<Course*> courses;
    for (std::string s: this->coursesIDs) {
        int id = stringToInt(s);
        Course* c = new Course(id);
        courses.push_back(c);
    }
    return courses;
}
void User::setID(std::string id) {
    this->id = 0;
    this->userPrefix = "u";
}