#include "../headers/student.h"
#include "../headers/functions.h"
#include "../headers/course.h"
#include "../headers/assignment.h"
#include <algorithm>

void Student::setID(std::string id) {
    this->id = stringToInt(id);
    this->userPrefix = StudentConstants().PREFIX;
    this->filePath = StudentConstants().FILE_PATH;
    File file(this->filePath);
    std::string line = file.readLine(this->id);
    std::vector<std::string> parts = split(line,StudentConstants().FIELDS_SEPARATOR);

    this->name = parts.at(StudentConstants().NAME);
    this->username = parts.at(StudentConstants().USERNAME);
    this->password = parts.at(StudentConstants().PASSWORD);
    this->email = parts.at(StudentConstants().EMAIL);
    this->coursesIDs = split(parts.at(StudentConstants().COURSE_IDS),StudentConstants().COURSE_IDS_SEPARATOR);
    this->friendsIDs = split(parts.at(StudentConstants().FRIENDS_IDS),StudentConstants().FRIENDS_IDS_SEPARATOR);
    this->notificationsIDs = split(parts.at(StudentConstants().NOTIFICATIONS),StudentConstants().NOTIFICATIONS_SEPARATOR);
}

void studentStart(std::string personID) {
    // the student
    Student stu(personID);
    Notification noti(&stu);
    Friend fri(&stu);
    // Welcome message
    std::cout<<"Welcome "<<stu.getName()<<std::endl;

    while (true) {
        // menu
        std::cout<<"\n1. View Profile\n"
        <<"2. Enroll in Course\n"
        <<"3. My Courses\n"
        <<"4. Solve Assignments\n"
        <<"5. View Assignment Report\n"
        <<"6. Add a Friend\n"
        <<"7. Friends List\n"
        <<"8. Notifications\n"
        <<"9. Log Out\n";
        int choice = validateChoice(1,9,"Please enter your choice: ");
        std::cout<<std::endl;
        if (choice == 1) {
            // My Profile
            std::cout<<"ID            : "<<stu.getID()<<std::endl
                     <<"Name          : "<<stu.getName()<<std::endl
                     <<"Username      : "<<stu.getUsername()<<std::endl
                     <<"Courses       : You are enrolled in [ "
                     <<stu.getCourses().size()<<" ] Course(s)\n"
                     <<"Friends       : You have [ "
                     <<fri.getFriends().size()<<" ] Friend(s)\n"
                     <<"Notifications : [ "
                     <<noti.getNotificatoins().size()<<" ] Notification(s)\n";
        }else if (choice == 2) {
            // Register in Course
            std::vector<Course*> courses = stu.getUnregisteredCourses();
            if (courses.empty()) {
                std::cout << "\nYou are already registered in all available courses!\n";
                continue;
            }
            std::cout << "\nAvailable Courses:\n";
            for (int i = 0 ; i < courses.size() ; i++) {
                std::cout<<i+1<<". Course "<<courses.at(i)->getName()<<std::endl;
            }
            int c = validateChoice(1,courses.size(),"\nEnter the number of the course to view details: ");
            std::cout<<std::endl;
            Course* currentCourse = courses.at(c-1);
            std::cout<<"Course Code       : "<<currentCourse->getID()<<std::endl
                    <<"Course Name       : "<<currentCourse->getName()<<std::endl
                    <<"Created by        : Dr. "<<currentCourse->getDoctor()<<std::endl
                    <<"Assignments       : [ "<<currentCourse->getAssignments().size()<<" ] Assignment(s)\n"
                    <<"Enrolled Students : [ "<<currentCourse->getStudetns().size()<<" ] Student(s)\n\n";
            int yOrN = validateChoice(0,1,"Do you want to register in this course? [1 = Yes, 0 = No]: ");
            std::cout<<std::endl;
            if (yOrN) {
                currentCourse->addStudent(personID);
                stu.addRegisterCourse(currentCourse->getID());
                std::cout << "Successfully registered in \"" << currentCourse->getName() << "\"!\n";
            }
        } else if (choice == 3) {
            // List my courses
            std::vector<Course*> courses = stu.getCourses();
            if (courses.size() == 0) {
                std::cout << "\nYou are not enrolled in any courses yet.\n";
                std::cout << "Go to 'Enroll in Course' to join one!\n";
                continue;
            }
            std::cout << "\nYour Courses:\n";
            for (int i = 0 ; i < courses.size() ; i++) {
                std::cout<<i+1<<". Course "<<courses.at(i)->getName()<<" - Code "<<courses.at(i)->getID()<<std::endl;
            }
            std::cout<<std::endl;
            int c = validateChoice(1,courses.size(),"Enter the number of the course to view details: ");
            Course* currentCourse = courses.at(c-1);
            std::cout<<std::endl;
            std::cout<<"Course Code       : "<<currentCourse->getID()<<std::endl
                     <<"Course Name       : "<<currentCourse->getName()<<std::endl
                     <<"Created by        : Dr. "<<currentCourse->getDoctor()<<std::endl
                     <<"Assignments       : [ "<<currentCourse->getAssignments().size()<<" ] Assignment(s)\n"
                     <<"Enrolled Students : [ "<<currentCourse->getStudetns().size()<<" ] Student(s)\n";
        } else if (choice == 4) {
            // Solve Assignment
            std::vector<Course*> courses = stu.getCourses();
            if (courses.empty()) {
                std::cout << "\nYou are not enrolled in any courses yet.\n";
                std::cout << "Go to 'Enroll in Course' to join one!\n";
                continue;
            }
            std::cout << "\nYour Courses:\n";
            for (int i = 0 ; i < courses.size() ; i++) {
                std::cout<<i+1<<". Course "<<courses.at(i)->getName()<<" - Code "<<courses.at(i)->getID()<<std::endl;
            }
            std::cout<<std::endl;
            int c = validateChoice(1,courses.size(),"Enter the number of the course to view: ");
            Course* currentCourse = courses.at(c-1);
            std::cout<<std::endl;
            std::cout<<"Course Code       : "<<currentCourse->getID()<<std::endl
                     <<"Course Name       : "<<currentCourse->getName()<<std::endl
                     <<"Created by        : Dr. "<<currentCourse->getDoctor()<<std::endl
                     <<"Assignments       : [ "<<currentCourse->getAssignments().size()<<" ] Assignment(s)\n"
                     <<"Enrolled Students : [ "<<currentCourse->getStudetns().size()<<" ] Student(s)\n";
            int a = validateChoice(0,1,"Do you want to solve the assignments? [1 = Yes, 0 = No]: ");
            if (a) {
                std::vector<Assignment*> assignments = stu.getCourses().at(c-1)->getAssignments();
                if (assignments.empty()) {
                    std::cout << "\nNo assignments available for this course yet.\n";
                    continue;
                }
                std::cout << "\nStarting Assignments...\n";
                for (int i = 0 ; i < assignments.size() ; i++) {
                    std::cout<<"[ "<<i+1<<" ] "<<assignments.at(i)->getQuestion()<<std::endl;
                    std::vector<std::string> answers = assignments.at(i)->getAnswers();
                    for (int j = 0 ; j < answers.size() ; j++) {
                        std::cout<<"  "<<j+1<<". "<<answers.at(j)<<std::endl;
                    }
                    int s = validateChoice(1,answers.size(),"Enter the number of answer: ");
                    if (answers.at(s-1) == assignments.at(i)->getCorrectAnswer()) {
                        assignments.at(i)->addStudent(stu.getID(),true);
                        std::cout<<"Correct!\n";
                    }else {
                        assignments.at(i)->addStudent(stu.getID(),false);
                        std::cout<<"Wrong!  Correct answer was: "
                                << assignments.at(i)->getCorrectAnswer() << "\n";
                    }
                }
                int count = 0;
                for (Assignment* a: assignments) {
                    count += a->isSolved(personID);
                }
                std::cout<<"Results: You solved "<<count<<" Out of "<<assignments.size()<<" correctly."<<std::endl;
            }
        } else if (choice == 5) {
            // Assignment Report
            std::vector<Course*> courses = stu.getCourses();
            if (courses.empty()) {
                std::cout << "\nYou are not enrolled in any courses yet.\n";
                std::cout << "Go to 'Enroll in Course' to join one!\n";
                continue;
            }
            for (int i = 0 ; i < courses.size() ; i++) {
                std::cout<<i+1<<". Course "<<courses.at(i)->getName()<<" - Code "<<courses.at(i)->getID()<<std::endl;
            }
            int c = validateChoice(1,courses.size(),"Please enter the number of the course you would like to view: ");
            std::vector<Assignment*> currentAssignment = courses.at(c-1)->getAssignments();
            if (currentAssignment.empty()) {
                std::cout << "This course has no assignments yet.\n";
                continue;
            }
            std::cout<<std::endl;
            int count = 0;
            for (Assignment* a: currentAssignment) {
                count += a->isSolved(personID);
            }
            std::cout<<"You have solved "<<count<<" Out of "<<currentAssignment.size()<<" Assignment(s)"<<std::endl;
        } else if (choice == 6) {
            // Add Friend
            std::vector<Student*> notFriends = fri.getNotFriends();
            if (notFriends.empty()) {
                std::cout << "No users available to add as friends. You're connected with everyone!\n";
                continue;
            }
            std::cout << "\nAvailable Users to Add as Friend:\n";
            for (int i = 0 ; i < notFriends.size() ; i++) {
                std::cout<<i+1<<". "<<notFriends.at(i)->getName()<<std::endl;
            }
            int c = validateChoice(1,notFriends.size(),"Enter the number of the user to view details: ");
            std::cout<<std::endl;
            Student* currentFriend = notFriends.at(c-1);
            Friend currentFri(currentFriend);
            std::cout<<"ID       : "<<currentFriend->getID()<<std::endl
                     <<"Name     : "<<currentFriend->getName()<<std::endl
                     <<"Username : "<<currentFriend->getUsername()<<std::endl
                     <<"Courses  : [ "<<currentFriend->getCourses().size()<<" ] Course(s)\n"
                     <<"Friends  : [ "<<currentFri.getFriends().size()<<" ] Friend(s)\n\n";
            c = validateChoice(0,1,"Do you want to send a friend request? [1 = Yes | 0 = No]: ");
            std::cout<<std::endl;
            if (c) {
                noti.send(currentFriend);
                    std::cout << "Friend request sent to " << currentFriend->getName() << "\n";
            }
        } else if (choice == 7) {
            // List my Friends
            std::vector<Student*> friends = fri.getFriends();
            if (friends.size() == 0) {
                std::cout<<"You don't have any friends yet.\n";
                continue;
            }
            std::cout<<"You have "<<friends.size()<<" Friend(s):\n";
            for (int i = 0 ; i < friends.size() ; i++) {
                std::cout<<i+1<<". "<<friends.at(i)->getName()<<std::endl;
            }
            int c = validateChoice(1,friends.size(),"Enter your friend number: ");
            std::cout<<std::endl;
            Student* currentFriend = friends.at(c-1);
            Friend currentFri(currentFriend);
            std::cout<<"ID       : "<<currentFriend->getID()<<std::endl
                     <<"Name     : "<<currentFriend->getName()<<std::endl
                     <<"Username : "<<currentFriend->getUsername()<<std::endl
                     <<"Courses  : [ "<<currentFriend->getCourses().size()<<" ] Course(s)\n"
                     <<"Friends  : [ "<<currentFri.getFriends().size()<<" ] Friend(s)\n";
        } else if (choice == 8) {
            // Get Notifications
            std::vector<Student*> notifications = noti.getNotificatoins();
            if (notifications.size() == 0) {
                std::cout<<"You haven't Notifications.\n";
                continue;
            }
            for (int i = 0 ; i < notifications.size() ; i++) {
                std::cout<<i+1<<". "<<notifications.at(i)->getName()<<std::endl;
            }
            int c = validateChoice(1,notifications.size(),"Enter your friend number: ");
            std::cout<<std::endl;
            Student* currentNotification = notifications.at(c-1);
            Friend currentFri(currentNotification);
            std::cout<<"ID: "<<currentNotification->getID()
                    <<"\nName: "<<currentNotification->getName()
                    <<"\nUsername: "<<currentNotification->getUsername()
                    <<"\nHe Register in "<<currentNotification->getCourses().size()<<" Course(s)"
                    <<"\nHe has "<<currentFri.getFriends().size()<<" Friend(s)\n";
            c = validateChoice(0,1,"Do you want to make this user your friend [1 -> yes , 0 -> no]: ");
            if (c) {
                fri.addFriend(currentNotification);
                currentFri.addFriend(&stu);
            }
        }else {
            // Log out
            break;
        }
    }
    welcome();
}

std::vector<Course*> Student::getCourses() {
    if (this->coursesIDs.empty()) return {};
    if (this->coursesIDs.size() == 1) {
        if (this->coursesIDs.at(0).empty()) {
            return {};
        }
    }
    std::vector<Course*> courses;
    for (std::string s: this->coursesIDs) {
        int id = stringToInt(s);
        Course* c = new Course(id);
        courses.push_back(c);
    }
    return courses;
}
std::vector<Course*> Student::getUnregisteredCourses() {
    std::vector<int> studentCourseIDs;
    for (std::string id : this->coursesIDs) {
        studentCourseIDs.push_back(stringToInt(id));
    }

    File file(courseConstants().FILE_PATH);
    int numCourses = file.read().size(); // total number of courses
    std::vector<Course*> unregisteredCourses;
    
    for (int i = 1; i < numCourses; i++) {
        // std::cout<<numCourses<<"hiiiiiii\n";
        if (std::find(studentCourseIDs.begin(), studentCourseIDs.end(), i) == studentCourseIDs.end()) {
            Course* c = new Course(i);
            unregisteredCourses.push_back(c);
        }
    }

    return unregisteredCourses;
}

void Student::addRegisterCourse(std::string courseID) {
    File file(this->filePath);
    this->coursesIDs.push_back(courseID);
    std::string line = (this->userPrefix + std::to_string(this->id)) + StudentConstants().FIELDS_SEPARATOR + this->name + StudentConstants().FIELDS_SEPARATOR + this->username + StudentConstants().FIELDS_SEPARATOR + this->password + StudentConstants().FIELDS_SEPARATOR + this->email + StudentConstants().FIELDS_SEPARATOR + join(this->coursesIDs,StudentConstants().COURSE_IDS_SEPARATOR) + StudentConstants().FIELDS_SEPARATOR + join(this->friendsIDs,StudentConstants().FRIENDS_IDS_SEPARATOR) + StudentConstants().FIELDS_SEPARATOR + join(this->notificationsIDs,StudentConstants().NOTIFICATIONS_SEPARATOR);
    file.replaceLine(this->id,line);
}


// Notifications
Notification::Notification(Student* from) {
    setStudents(from);
}

void Notification::setStudents(Student* from) {
    this->from = from;
    File file(StudentConstants().FILE_PATH);
    std::string line1 = file.readLine(stringToInt(this->from->getID()));
    this->stu1notificationsIDs = split(split(line1,StudentConstants().FIELDS_SEPARATOR).at(StudentConstants().NOTIFICATIONS),StudentConstants().NOTIFICATIONS_SEPARATOR);
}

std::vector<Student*> Notification::getNotificatoins() {
    if (this->stu1notificationsIDs.empty()) return {};
    std::vector<Student*> studs;
    for (std::string id: this->stu1notificationsIDs) {
        Student* stud = new Student(id);
        studs.push_back(stud);
    }
    return studs;
}

void Notification::send(Student* to) {
    File file(StudentConstants().FILE_PATH);
    std::string line = file.readLine(stringToInt(to->getID()));
    std::vector<std::string> parts = split(line,StudentConstants().FIELDS_SEPARATOR);
    std::vector<std::string> NotificationsIDs = split(parts.at(StudentConstants().NOTIFICATIONS),StudentConstants().NOTIFICATIONS_SEPARATOR);
    NotificationsIDs.push_back(this->from->getID());
    parts.at(StudentConstants().NOTIFICATIONS) = join(NotificationsIDs,StudentConstants().NOTIFICATIONS_SEPARATOR);
    line = join(parts,StudentConstants().FIELDS_SEPARATOR);
    file.replaceLine(stringToInt(to->getID()),line);
}

// Friend
Friend::Friend(Student* stu) {setStudent(stu);}

void Friend::setStudent(Student* stu) {
    this->stu = stu;
    File file(StudentConstants().FILE_PATH);
    std::vector<std::string> parts = split(file.readLine(stringToInt(this->stu->getID())), StudentConstants().FIELDS_SEPARATOR);
    this->friends = split(parts.at(StudentConstants().FRIENDS_IDS), StudentConstants().FRIENDS_IDS_SEPARATOR);
}

std::vector<Student*> Friend::getFriends() {
    if (this->friends.empty()) return {};
    std::vector<Student*> studs;
    for (std::string id: this->friends) {
        Student* stu = new Student(id);
        studs.push_back(stu);
    }
    return studs;
}

std::vector<Student*> Friend::getNotFriends() {
    std::vector<int> friendsIDs;
    for (std::string id : this->friends) {
        friendsIDs.push_back(stringToInt(id));
    }
    friendsIDs.push_back(stringToInt(this->stu->getID()));

    File file(StudentConstants().FILE_PATH);
    int numCourses = file.read().size(); // total number of courses
    std::vector<Student*> notFriends;

    
    for (int i = 1; i <= numCourses; i++) {
        if (std::find(friendsIDs.begin(), friendsIDs.end(), i) == friendsIDs.end()) {
            std::string id = (StudentConstants().PREFIX + std::to_string(i));
            Student* c = new Student(id);
            notFriends.push_back(c);
        }
    }

    return notFriends;
}

void Friend::addFriend(Student* fri) {
    this->friends.push_back(fri->getID());
    File file(StudentConstants().FILE_PATH);
    std::string line = file.readLine(stringToInt(this->stu->getID()));
    std::vector<std::string> parts = split(line, StudentConstants().FIELDS_SEPARATOR);
    std::vector<std::string> n = split(parts.at(StudentConstants().NOTIFICATIONS), StudentConstants().NOTIFICATIONS_SEPARATOR);
    n.erase(remove(n.begin(), n.end(), fri->getID()), n.end());
    parts.at(StudentConstants().NOTIFICATIONS) = join(n, StudentConstants().NOTIFICATIONS_SEPARATOR);
    parts.at(StudentConstants().FRIENDS_IDS) = join(this->friends, StudentConstants().FRIENDS_IDS_SEPARATOR);
    line = join(parts, StudentConstants().FIELDS_SEPARATOR);
    file.replaceLine(stringToInt(this->stu->getID()),line);
}