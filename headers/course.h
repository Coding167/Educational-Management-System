#ifndef COURSES_H_INCLUDED
#define COURSES_H_INCLUDED

#include <iostream>
#include <vector>

class Student;
class Assignment;

class Course {
    private:
        int id;
        std::string coursePrefix;
        std::string name;
        std::string docID;
        std::vector<std::string> studentsIDs;
        std::vector<std::string> assignmentsIDs;
        std::string filePath;

    public:
        Course(): id(0) {}
        Course(int id) { this->setID(id); }

        void setID(int id);

        static std::string addCourse(std::string name, std::string docID);

        std::string getName() const;
        std::string getID() const;
        std::vector<Student*> getStudetns() const;
        std::vector<Assignment*> getAssignments() const;
        std::string getDoctor();
        void addStudent(std::string stuID);
};

#endif // COURSES_H_INCLUDED