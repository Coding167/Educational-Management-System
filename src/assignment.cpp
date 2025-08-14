#include "../headers/assignment.h"
#include "../headers/functions.h"
#include "../headers/student.h"
#include <algorithm>

void Assignment::setID(int id) {
    this->id = id;
    File file(filePath);
    std::string line = file.readLine(id);
    std::vector<std::string> parts = split(line, ',');

    this->question = parts.at(1);
    this->answers = split(parts.at(2),'/');
    this->correctAnswer = stringToInt(parts.at(3));
    this->students_s_IDs = split(parts.at(4),'/');
    this->students_ns_IDs = split(parts.at(4),'/');
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
    if (solved) {
        this->students_s_IDs.push_back(stuID);
    }else {
        this->students_ns_IDs.push_back(stuID);
    }
    File file(this->filePath);
    std::string line = (this->assignmentPrefix + std::to_string(id)) + ',' + this->question + ',' + join(answers,'/') + ',' + std::to_string(this->correctAnswer) + ',' + join(this->students_s_IDs,'/') + ',' + join(this->students_ns_IDs,'/');
    file.replaceLine(this->id,line);
}

std::string Assignment::addAssignment(std::string question, std::vector<std::string> answers, int correctAnswer, std::string courseID) {
    File infoFile("../data/info.csv");
    std::string infoLine = infoFile.readLine(1);
    std::vector<std::string> parts = split(infoLine,',');
    int id = stringToInt(parts.at(3));
    id++;
    parts.at(3) = 'a' + std::to_string(id);
    infoFile.replaceLine(1,join(parts,','));
    std::string line = ('a' + std::to_string(id)) + ',' + question + ',' + join(answers,'/') + ',' + std::to_string(correctAnswer) + ",,\n";
    File file("../data/assignments.csv");
    file.addLine(line);
    file.setPath("../data/courses.csv");
    line = file.readLine(stringToInt(courseID));
    parts = split(line,',');
    std::vector<std::string> a = split(parts.at(4),'/');
    a.push_back('a' + std::to_string(id));
    parts.at(4) = join(a,'/');
    file.replaceLine(stringToInt(courseID),join(parts,','));
    return ('a' + std::to_string(id));
}
