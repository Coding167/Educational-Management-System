#include "../headers/course.h"
#include "../headers/functions.h"
#include "../headers/student.h"
#include "../headers/doctor.h"
#include "../headers/assignment.h"

void Course::setID(int id) {
    this->id = id;
    File file(this->filePath);
    std::string line = file.readLine(id);
    std::vector<std::string> parts = split(line,',');

    this->name = parts.at(1);
    this->docID = parts.at(2);
    this->studentsIDs = split(parts.at(3),'/');
    this->assignmentsIDs = split(parts.at(4), '/');
}

std::string Course::addCourse(std::string name, std::string docID) {
    File infoFile("../data/info.csv");
    std::string infoLine = infoFile.readLine(1);
    std::vector<std::string> parts = split(infoLine,',');
    int id = stringToInt(parts.at(0));
    id++;
    parts.at(0) = 'c' + std::to_string(id);
    infoFile.replaceLine(1,join(parts,','));
    File file("../data/courses.csv");
    std::string line = 'c' + std::to_string(id) + ',' + name + ',' + docID + ",,\n";
    file.addLine(line);
    return ('c' + std::to_string(id));
}

std::string Course::getName() const {
    return this->name;
}
std::string Course::getID() const {
    return ( coursePrefix + std::to_string(this->id) );
}
std::vector<Student*> Course::getStudetns() const {
    if (studentsIDs.empty()) return {};
    if (studentsIDs.size() == 1) {
        if (studentsIDs.at(0).empty()) {
            return {};
        }
    }
    std::vector<Student*> studs;
    for (std::string s: this->studentsIDs) {
        Student* stud = new Student(s);
        studs.push_back(stud);
    }
    return studs;
}
std::vector<Assignment*> Course::getAssignments() const {
    if (assignmentsIDs.empty()) return {};
    if (assignmentsIDs.size() == 1) {
        if (assignmentsIDs.at(0).empty()) {
            return {};
        }
    }
    std::vector<Assignment*> ass;
    for (std::string s: this->assignmentsIDs) {
        Assignment* a = new Assignment(stringToInt(s));
        ass.push_back(a);
    }
    return ass;
}
std::string Course::getDoctor() {
    Doctor* d = new Doctor(this->docID);
    return d->getName();
}

void Course::addStudent(std::string stuID) {
    File file(filePath);
    this->studentsIDs.push_back(stuID);
    std::string line = (this->coursePrefix + std::to_string(id)) + ',' + this->name + ',' + this->docID + ',' + join(this->studentsIDs,'/') + ',' + join(this->assignmentsIDs,'/');
    file.replaceLine(this->id,line);
}