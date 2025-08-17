#ifndef DOCTORS_H_INCLUDED
#define DOCTORS_H_INCLUDED

#include "user.h"
#include "constants.h"

class Doctor: public User {
    public:
        Doctor() {
            this->id = 0;
            this->userPrefix = DoctorConstants().PREFIX;
            this->filePath = DoctorConstants().FILE_PATH;
        }
        Doctor(std::string id) { this->setID(id); }

        void setID(std::string id);

        void addCourse(std::string courseID);
};

void doctorStart(std::string personID);

#endif // DOCTORS_H_INCLUDED