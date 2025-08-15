#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#include <string>

struct AssignmentConstants {
    const std::string FILE_PATH = "../data/assignments.csv";
    const std::string PREFIX = "a";
    enum AssignmentFields {
        ID = 0,
        QUESTION = 1,
        ANSWER = 2,
        CORRECT = 3,
        STUDENTS_SOLVED = 4,
        STUDENTS_NOT_SOLVED = 5
    };
    const char FIELDS_SEPARATOR = ',';
    const char ANSWERS_SEPARATOR = '/';
    const char STUDENTS_SEPARATOR = '/';
};

struct InfoConstants {
    const std::string FILE_PATH = "../data/info.csv";
    enum InfoFields {
        COURSE_ID = 0,
        STUDENT_ID = 1,
        DOCTOR_ID = 2,
        ASSIGNMENT_ID = 3
    };
    const char FIELDS_SEPARATOR = ',';
};

struct StudentConstants {
    const std::string FILE_PATH = "../data/students.csv";
    const std::string PREFIX = "s";
    enum StudentFields {
        ID = 0,
        NAME = 1,
        USERNAME = 2,
        PASSWORD = 3,
        EMAIL = 4,
        COURSE_IDS = 5,
        FRIENDS_IDS = 6,
        NOTIFICATIONS = 7
    };
    const char FIELDS_SEPARATOR = ',';
    const char COURSE_IDS_SEPARATOR = '/';
    const char FRIENDS_IDS_SEPARATOR = '/';
    const char NOTIFICATIONS_SEPARATOR = '/';
};

struct DoctorConstants {
    const std::string FILE_PATH = "../data/doctors.csv";
    const std::string PREFIX = "d";
    enum DoctorFields {
        ID = 0,
        NAME = 1,
        USERNAME = 2,
        PASSWORD = 3,
        EMAIL = 4,
        COURSE_IDS = 5
    };
    const char FIELDS_SEPARATOR = ',';
    const char COURSE_IDS_SEPARATOR = '/';
};



#endif // CONSTANTS_H_INCLUDED