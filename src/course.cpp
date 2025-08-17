#include "../headers/course.h"
#include "../headers/functions.h"
#include "../headers/student.h"
#include "../headers/doctor.h"
#include "../headers/assignment.h"
#include "../headers/constants.h"

void Course::setID(int id) {
    this->id = id;
    this->coursePrefix = courseConstants().PREFIX;
    this->filePath = courseConstants().FILE_PATH;

    // Read course data from file
    File file(this->filePath);
    std::string line = file.readLine(id);
    std::vector<std::string> parts = split(line,courseConstants().FIELDS_SEPARATOR);

    // Store the data in the object
    this->name = parts.at(courseConstants().NAME);
    this->docID = parts.at(courseConstants().DOC_ID);
    this->studentsIDs = split(parts.at(courseConstants().STUDENTS_IDS),courseConstants().STUDENTS_IDS_SEPARATOR);
    this->assignmentsIDs = split(parts.at(courseConstants().ASSIGNMENTS_IDS), courseConstants().ASSIGNMENTS_IDS_SEPARATOR);
}

std::string Course::addCourse(std::string name, std::string docID) {
    // Get the last course ID and increment it
    File infoFile(InfoConstants().FILE_PATH);
    std::string infoLine = infoFile.readLine(1);
    std::vector<std::string> parts = split(infoLine,InfoConstants().FIELDS_SEPARATOR);
    int id = stringToInt(parts.at(InfoConstants().COURSE_ID));
    id++;
    // Update the info file with the new course ID
    parts.at(InfoConstants().COURSE_ID) = courseConstants().PREFIX + std::to_string(id);
    infoFile.replaceLine(1,join(parts,InfoConstants().FIELDS_SEPARATOR));
    // Store the new course in the file
    File file(courseConstants().FILE_PATH);
    std::string line = courseConstants().PREFIX + std::to_string(id) + courseConstants().FIELDS_SEPARATOR + name + courseConstants().FIELDS_SEPARATOR + docID + courseConstants().FIELDS_SEPARATOR + courseConstants().FIELDS_SEPARATOR + "\n";
    file.addLine(line);
    return (courseConstants().PREFIX + std::to_string(id));
}

std::string Course::getName() const {
    return this->name;
}
std::string Course::getID() const {
    return ( coursePrefix + std::to_string(this->id) );
}
std::vector<Student*> Course::getStudetns() const {
    // Check if there are no students
    if (studentsIDs.empty()) return {};
    if (studentsIDs.size() == 1) {
        if (studentsIDs.at(0).empty()) {
            return {};
        }
    }
    // Create a vector of Student pointers
    std::vector<Student*> studs;
    for (std::string s: this->studentsIDs) {
        Student* stud = new Student(s);
        studs.push_back(stud);
    }
    return studs;
}
std::vector<Assignment*> Course::getAssignments() const {
    // Check if there are no assignments
    if (assignmentsIDs.empty()) return {};
    if (assignmentsIDs.size() == 1) {
        if (assignmentsIDs.at(0).empty()) {
            return {};
        }
    }
    // Create a vector of Assignment pointers
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
    File file(this->filePath);
    this->studentsIDs.push_back(stuID);
    std::string line = (this->coursePrefix + std::to_string(id)) + courseConstants().FIELDS_SEPARATOR + this->name + courseConstants().FIELDS_SEPARATOR + this->docID + courseConstants().FIELDS_SEPARATOR + join(this->studentsIDs,courseConstants().STUDENTS_IDS_SEPARATOR) + courseConstants().FIELDS_SEPARATOR + join(this->assignmentsIDs,courseConstants().ASSIGNMENTS_IDS_SEPARATOR);
    file.replaceLine(this->id,line);
}