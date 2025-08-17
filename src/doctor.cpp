#include "../headers/doctor.h"
#include "../headers/functions.h"
#include "../headers/course.h"
#include "../headers/assignment.h"
#include "../headers/student.h"
#include <limits>


void Doctor::setID(std::string id) {
    this->id = stringToInt(id);
    this->userPrefix = DoctorConstants().PREFIX;
    this->filePath = DoctorConstants().FILE_PATH;
    File file(filePath);
    std::string line = file.readLine(this->id);
    std::vector<std::string> parts = split(line,',');

    this->name = parts.at(DoctorConstants().NAME);
    this->username = parts.at(DoctorConstants().USERNAME);
    this->password = parts.at(DoctorConstants().PASSWORD);
    this->email = parts.at(DoctorConstants().EMAIL);
    this->coursesIDs = split(parts.at(DoctorConstants().COURSE_IDS),DoctorConstants().COURSE_IDS_SEPARATOR);
}

void doctorStart(std::string personID) {
    // the doctor
    Doctor doc(personID);
    while (true) {
        // menu
        std::cout<<"\nPlease Enter a Choice:\n"
                <<"1- My Profile\n"
                <<"2- Create a Course\n"
                <<"3- Add Assignment\n"
                <<"4- List my Courses\n"
                <<"5- Log out\n";
        int choice = validateChoice(1,5,"Enter Choice: ");
        std::cout<<std::endl;

        if (choice == 1) {
            // See his profile
            std::cout<<"ID: "<<doc.getID()
                    <<"\nName: "<<doc.getName()
                    <<"\nUsername: "<<doc.getUsername()
                    <<"\nYou have created "<<doc.getCourses().size()<<" Course(s)\n";
        } else if (choice == 2 ) {
            // Create a Course
            std::string name;
            std::cout<<"Enter the name of the Course: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin,name);

            std::string courseID = Course::addCourse(name,doc.getID());
            File file(DoctorConstants().FILE_PATH);
            std::vector<std::string> parts = split(file.readLine(stringToInt(doc.getID())),DoctorConstants().FIELDS_SEPARATOR);
            std::vector<std::string> cs = split(parts.at(DoctorConstants().COURSE_IDS),DoctorConstants().COURSE_IDS_SEPARATOR);

            cs.push_back(courseID);
            parts.at(5) = join(cs,DoctorConstants().COURSE_IDS_SEPARATOR);
            file.replaceLine(stringToInt(doc.getID()),join(parts,DoctorConstants().FIELDS_SEPARATOR));
        } else if (choice == 3) {
            // Add Assignment
            std::vector<Course*> courses = doc.getCourses();
            if (courses.empty()) {
                std::cout << "You have no courses.\n";
                continue;
            }

            for (int i = 0; i < courses.size(); ++i) {
                std::cout << i + 1 << ". " << courses[i]->getName() << " - " << courses[i]->getID() << std::endl;
            }

            int c = validateChoice(1, courses.size(), "Enter the number of Course to view it: ");
            Course* currentCourse = courses[c - 1];

            std::string question;
            std::vector<std::string> answers;
            int n;

            std::cin.ignore(10000, '\n'); // ignore leftover newline ONCE

            std::cout << "Enter the question: ";
            std::getline(std::cin, question);

            n = validateChoice(1, 6, "Enter the number of answers: ");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            for (int i = 0; i < n; ++i) {
                std::string answer;
                std::cout << i + 1 << ". ";
                std::getline(std::cin, answer);
                answers.push_back(answer);
            }

            int correctAnswer = validateChoice(1, n, "Enter the number of correct answer: ");
            correctAnswer--;

            Assignment::addAssignment(question, answers, correctAnswer, currentCourse->getID());
        }else if (choice == 4) {
            // List Courses
            std::vector<Course*> courses = doc.getCourses();
            if (courses.size() == 0) {
                std::cout<<"You have not a courses.\n";
                continue;
            }

            for (int i = 0 ; i < courses.size() ; i++) {
                std::cout<<i+1<<". "<<courses.at(i)->getName()<<" - "<<courses.at(i)->getID()<<std::endl;
            }
            int c = validateChoice(1,courses.size(),"Enter the number of Course to view it: ");
            Course* currentCourse = courses.at(c-1);

            std::cout<<"Code: "<<currentCourse->getID()
                    <<"\nName: "<<currentCourse->getName()
                    <<"\nThe Course has "<<currentCourse->getAssignments().size()<<" Assignment(s) and "<<currentCourse->getStudetns().size()<<" Student(s)\n";
            c = validateChoice(1,3,"If you want to see assignments or students Enter [1,2,3]: ");

            if (c == 1) {
                // Show Assignments
                std::vector<Assignment*> assignments = currentCourse->getAssignments();
                for (int i = 0 ; i < assignments.size() ; i++) {
                    std::cout<<"[ "<<i+1<<" ] "<<assignments.at(i)->getQuestion()<<std::endl;
                    std::vector<std::string> answers = assignments.at(i)->getAnswers();
                    for (int j = 0 ; j < answers.size() ; j++) {
                        std::cout<<j+1<<". "<<answers.at(j)<<std::endl;
                    }
                    std::cout<<"The correct Answer is "<<assignments.at(i)->getCorrectAnswer()<<std::endl;
                }
            }else if (c == 2) {
                // Show Students
                std::vector<Student*> students = currentCourse->getStudetns();
                std::cout<<"The Course has "<<students.size()<<" Student(s)\n";
                for (int i = 0 ; i < students.size() ; i++) {
                    std::cout<<i+1<<". "<<students.at(i)->getName()<<std::endl;
                }
            }
        }else {
            // Log out
            break;
        }
    }
    welcome();
}