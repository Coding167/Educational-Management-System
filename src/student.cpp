#include "../headers/student.h"
#include "../headers/functions.h"
#include "../headers/course.h"
#include "../headers/assignment.h"
#include "../headers/DB.h"
#include <algorithm>

void Student::setID(std::string idStr) {
  this->id = stringToInt(idStr);

  Database db;
  if (!db.connect(connectionString)) return;

  SQLHSTMT stmt = db.executeQueryHandle(
    "SELECT id, name, username "
    "FROM Student "
    "WHERE id = " + idStr + ";"
  );

  if (stmt != NULL && SQLFetch(stmt) == SQL_SUCCESS) {
    // Get ID
    int id1;
    SQLGetData(stmt, 1, SQL_C_SLONG, &id1, 0, NULL);
    this->id = id1;

    // Get name and username
    char name1[100], username1[100];
    SQLGetData(stmt, 2, SQL_C_CHAR, name1, sizeof(name1), NULL);
    SQLGetData(stmt, 3, SQL_C_CHAR, username1, sizeof(username1), NULL);

    this->name = name1;
    this->username = username1;

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  } else {
    std::cout << "Failed to retrieve student data.\n";
  }
}

void Student::viewProfile() {

    Database db;
    if (!db.connect(connectionString)) return;

    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT s.id, s.name, s.username, "
        "(SELECT COUNT(*) FROM Student_Course sc WHERE sc.student_id = s.id), "
        "(SELECT COUNT(*) FROM Friend fr WHERE (fr.student_id = s.id OR fr.friend_id = s.id)), "
        "(SELECT COUNT(*) FROM Notification n WHERE n.[to] = s.id) "
        "FROM Student s "
        "WHERE s.id = " + std::to_string(this->id) + ";"
    );

    if (stmt != NULL && SQLFetch(stmt) == SQL_SUCCESS) {

        int id;
        char name[100];
        char username[100];
        int coursesCount;
        int friendsCount;
        int notificationsCount;

        SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
        SQLGetData(stmt, 3, SQL_C_CHAR, username, sizeof(username), NULL);
        SQLGetData(stmt, 4, SQL_C_SLONG, &coursesCount, 0, NULL);
        SQLGetData(stmt, 5, SQL_C_SLONG, &friendsCount, 0, NULL);
        SQLGetData(stmt, 6, SQL_C_SLONG, &notificationsCount, 0, NULL);

        std::cout << "Student Profile:\n";
        std::cout << "ID: " << id << "\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Username: " << username << "\n";
        std::cout << "Courses: " << coursesCount << "\n";
        std::cout << "Friends: " << friendsCount << "\n";
        std::cout << "Notifications: " << notificationsCount << "\n";
    }
    else {
        std::cout << "Failed to retrieve profile.\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void studentStart(std::string personID) {
    // the student
    Student stu(personID);
    Notification noti(&stu);
    Friend fri(&stu);

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
            stu.viewProfile();
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
                std::cout<<"You don't have any new notifications.\n";
                continue;
            }
            std::cout << "\nYou have [ " << notifications.size() << " ] new notification(s):\n";
            for (int i = 0 ; i < notifications.size() ; i++) {
                std::cout<<i+1<<". "<<notifications.at(i)->getName()<<std::endl;
            }
            int c = validateChoice(1,notifications.size(),"Enter the number to view details: ");
            std::cout<<std::endl;
            Student* currentNotification = notifications.at(c-1);
            Friend currentFri(currentNotification);
            std::cout<<"ID       : "<<currentNotification->getID()<<std::endl
                     <<"Name     : "<<currentNotification->getName()<<std::endl
                     <<"Username : "<<currentNotification->getUsername()<<std::endl
                     <<"Courses  : [ "<<currentNotification->getCourses().size()<<" ] Course(s)"<<std::endl
                     <<"Friends  : [ "<<currentFri.getFriends().size()<<" ] Friend(s)\n";
            c = validateChoice(0,1,"Do you want to accept this friend request? [1 -> Yes | 0 -> No]: ");
            if (c) {
                fri.addFriend(currentNotification);
                currentFri.addFriend(&stu);
                std::cout << "You are now friends with " << currentNotification->getName() << "!\n";
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