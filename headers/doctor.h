#ifndef DOCTORS_H_INCLUDED
#define DOCTORS_H_INCLUDED

#include "user.h"

class Doctor: public User {
    public:
        Doctor() {
            this->id = 0;
            this->userPrefix = 'd';
            this->filePath = "../data/doctors.csv";
        }
        Doctor(std::string id) { this->setID(id); }

        void setID(std::string id);

        std::string addDoctor(std::string name, std::string username, std::string password, std::string email);
};

void doctorStart(std::string personID);

#endif // DOCTORS_H_INCLUDED