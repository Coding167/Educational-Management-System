#ifndef STUDENTS_H_INCLUDED
#define STUDENTS_H_INCLUDED

#include "user.h"
#include <vector>

class Student : public User {
private:
    std::vector<std::string> friendsIDs;
    std::vector<std::string> notificationsIDs;
public:
    Student() {
        this->id = 0;
        this->userPrefix = 's';
        this->filePath = "../data/students.csv";
    }

    Student(std::string id) { setID(id); }

    void setID(std::string id);

    std::string addStudent(std::string name, std::string username, std::string password, std::string email);
    std::vector<Student*> getFriends();
    std::vector<Student*> getNotificatoins();
    std::vector<Course*> getCourses();
    std::vector<Course*> getUnregisteredCourses();
    void addRegisterCourse(std::string courseID);
    std::vector<Student*> getNotFriends();
    void addNotification(std::string stuID);
    void addFriend(std::string friendId);
};

void studentStart(std::string personID);

#endif // STUDENTS_H_INCLUDED