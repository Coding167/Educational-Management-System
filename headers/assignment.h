#ifndef ASSIGNMENTS_H_INCLUDED
#define ASSIGNMENTS_H_INCLUDED

#include <vector>
#include <iostream>
#include "student.h"

class Assignment {
    private:
        int id;
        char assignmentPrefix = 'a';
        std::string question;
        std::vector<std::string> answers;
        int correctAnswer;
        std::vector<std::string> students_s_IDs;  // Solved students
        std::vector<std::string> students_ns_IDs; // Not solved students
        std::string filePath = "../data/assignments.csv";

    public:
        Assignment(): id(0), correctAnswer(-1) {}
        Assignment(int id) { setID(id); }

        void setID(int id);

        std::string getQuestion() const;
        std::vector<std::string> getAnswers() const;
        std::string getCorrectAnswer() const;

        int isSolved(std::string idStu);

        void addStudent(std::string stuID, bool solved);

        static std::string addAssignment(std::string question, std::vector<std::string> answers, int correctAnswres, std::string courseID);
};

#endif // ASSIGNMENTS_H_INCLUDED