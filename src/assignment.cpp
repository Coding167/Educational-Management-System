#include "../headers/assignment.h"
#include "../headers/functions.h"
#include "../headers/student.h"
#include "../headers/constants.h"
#include <algorithm>

void Assignment::setID(int id) {
    this->id = id;
    this->assignmentPrefix = AssignmentConstants().PREFIX;
    this->filePath = AssignmentConstants().FILE_PATH;
    // Read the data from the file
    File file(filePath);
    std::string line = file.readLine(id);
    std::vector<std::string> parts = split(line, AssignmentConstants().FIELDS_SEPARATOR);
    // store the data in the object
    this->question = parts.at(AssignmentConstants().QUESTION);
    this->answers = split(parts.at(AssignmentConstants().ANSWER),AssignmentConstants().ANSWERS_SEPARATOR);
    this->correctAnswer = stringToInt(parts.at(AssignmentConstants().CORRECT));
    this->students_s_IDs = split(parts.at(AssignmentConstants().STUDENTS_SOLVED),AssignmentConstants().STUDENTS_SEPARATOR);
    this->students_ns_IDs = split(parts.at(AssignmentConstants().STUDENTS_NOT_SOLVED),AssignmentConstants().STUDENTS_SEPARATOR);
}

std::string Assignment::getQuestion() const {
    return this->question;
}
std::vector<std::string> Assignment::getAnswers() const {
    return this->answers;
}
std::string Assignment::getCorrectAnswer() const {
    return this->answers.at(this->correctAnswer);
}

int Assignment::isSolved(std::string idStu) {
    if (std::find(students_s_IDs.begin(), students_s_IDs.end(), idStu) != students_s_IDs.end()) {
        return 1;
    }
    return 0;
}

void Assignment::addStudent(std::string stuID, bool solved) {
    // Check if the student is already in the solved or not solved lists
    if (solved) 
        this->students_s_IDs.push_back(stuID);
    else 
        this->students_ns_IDs.push_back(stuID);
    // store the updated data in the file
    File file(this->filePath);
    std::string line = (this->assignmentPrefix + std::to_string(id)) + AssignmentConstants().FIELDS_SEPARATOR + this->question + AssignmentConstants().FIELDS_SEPARATOR + join(answers,AssignmentConstants().ANSWERS_SEPARATOR) + AssignmentConstants().FIELDS_SEPARATOR + std::to_string(this->correctAnswer) + AssignmentConstants().FIELDS_SEPARATOR + join(this->students_s_IDs,AssignmentConstants().STUDENTS_SOLVED) + AssignmentConstants().FIELDS_SEPARATOR + join(this->students_ns_IDs,AssignmentConstants().STUDENTS_NOT_SOLVED);
    file.replaceLine(this->id,line);
}

std::string Assignment::addAssignment(std::string question, std::vector<std::string> answers, int correctAnswer, std::string courseID) {
    // get the last assignment ID and increment it
    File infoFile(InfoConstants().FILE_PATH);
    std::string infoLine = infoFile.readLine(1);
    std::vector<std::string> parts = split(infoLine,InfoConstants().FIELDS_SEPARATOR);
    int id = stringToInt(parts.at(InfoConstants().ASSIGNMENT_ID));
    id++;
    // update the info file with the new assignment ID
    parts.at(InfoConstants().ASSIGNMENT_ID) = AssignmentConstants().PREFIX + std::to_string(id);
    infoFile.replaceLine(1,join(parts,InfoConstants().FIELDS_SEPARATOR));
    // store the new assignment in the file
    std::string line = parts.at(InfoConstants().ASSIGNMENT_ID) + AssignmentConstants().FIELDS_SEPARATOR + question + AssignmentConstants().FIELDS_SEPARATOR + join(answers,AssignmentConstants().ANSWER) + AssignmentConstants().FIELDS_SEPARATOR + std::to_string(correctAnswer) + AssignmentConstants().FIELDS_SEPARATOR + AssignmentConstants().FIELDS_SEPARATOR + "\n";
    File file(AssignmentConstants().FILE_PATH);
    file.addLine(line);
    // update the course file with the new assignment ID
    file.setPath(courseConstants().FILE_PATH);
    line = file.readLine(stringToInt(courseID));
    parts = split(line,courseConstants().FIELDS_SEPARATOR);
    std::vector<std::string> a = split(parts.at(courseConstants().ASSIGNMENTS_IDS),courseConstants().ASSIGNMENTS_IDS_SEPARATOR);
    a.push_back(AssignmentConstants().PREFIX + std::to_string(id));
    parts.at(courseConstants().ASSIGNMENTS_IDS) = join(a,courseConstants().ASSIGNMENTS_IDS_SEPARATOR);
    file.replaceLine(stringToInt(courseID),join(parts,courseConstants().FIELDS_SEPARATOR));
    return (AssignmentConstants().PREFIX + std::to_string(id));
}
