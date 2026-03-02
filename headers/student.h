#ifndef STUDENTS_H_INCLUDED
#define STUDENTS_H_INCLUDED

#include "user.h"
#include "constants.h"
#include <vector>

class Student : public User {
private:
    std::vector<std::string> friendsIDs;
    std::vector<std::string> notificationsIDs;
public:
    Student() {
        this->id = 0;
        this->userPrefix = StudentConstants().PREFIX;
        this->filePath = StudentConstants().FILE_PATH;
    }

    Student(std::string id) { setID(id); }

    void setID(std::string id);
    void viewProfile();

    std::vector<Course*> getCourses();
    std::vector<Course*> getUnregisteredCourses();
    void addRegisterCourse(std::string courseID);
};

void studentStart(std::string personID);

class Notification {
    private:
        Student* from;
        std::vector<std::string> stu1notificationsIDs;

    public:
        Notification() {}
        Notification(Student* from);

        void setStudents(Student* from);

        std::vector<Student*> getNotificatoins();
        void send(Student* to);
};

class Friend {
    private:
        Student* stu;
        std::vector<std::string> friends;

    public:
        Friend() {}
        Friend(Student* stu);

        void setStudent(Student* stu);

        std::vector<Student*> getFriends();
        std::vector<Student*> getNotFriends();

        void addFriend(Student* fri);
};
#endif // STUDENTS_H_INCLUDED