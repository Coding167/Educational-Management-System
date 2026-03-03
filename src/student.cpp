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

void Student::viewCourses() {

    Database db;
    if (!db.connect(connectionString)) return;

    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT c.id, c.name "
        "FROM Course c "
        "JOIN Student_Course sc ON sc.course_id = c.id "
        "WHERE sc.student_id = " + std::to_string(this->id) + ";"
    );

    if (stmt == NULL) {
        std::cout << "Failed to retrieve courses.\n";
        return;
    }

    std::vector<int> courseIDs;
    std::vector<std::string> courseNames;

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        int id;
        char name[100];

        SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

        courseIDs.push_back(id);
        courseNames.push_back(name);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    if (courseIDs.empty()) {
        std::cout << "You are not enrolled in any courses.\n";
        return;
    }

    std::cout << "\nYour Courses:\n";

    for (int i = 0; i < courseIDs.size(); i++) {
        std::cout << i + 1 << ". "
                  << courseNames[i]
                  << " - "
                  << courseIDs[i] << "\n";
    }

    int choice = validateChoice(1, courseIDs.size(), "Choose a Course: ");
    int selectedCourse = courseIDs[choice - 1];

    // Show course details
    SQLHSTMT stmt2 = db.executeQueryHandle(
        "SELECT c.id, c.name, d.name, "
        "(SELECT COUNT(*) FROM Assignment WHERE course_id = c.id), "
        "(SELECT COUNT(*) FROM Student_Course WHERE course_id = c.id) "
        "FROM Course c "
        "JOIN Doctor d ON d.id = c.doctor_id "
        "WHERE c.id = " + std::to_string(selectedCourse) + ";"
    );

    if (stmt2 != NULL && SQLFetch(stmt2) == SQL_SUCCESS) {

        int id;
        char courseName[100];
        char doctorName[100];
        int assignments;
        int students;

        SQLGetData(stmt2, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt2, 2, SQL_C_CHAR, courseName, sizeof(courseName), NULL);
        SQLGetData(stmt2, 3, SQL_C_CHAR, doctorName, sizeof(doctorName), NULL);
        SQLGetData(stmt2, 4, SQL_C_SLONG, &assignments, 0, NULL);
        SQLGetData(stmt2, 5, SQL_C_SLONG, &students, 0, NULL);

        std::cout << "\n--- Course Details ---\n";
        std::cout << "ID          : " << id << "\n";
        std::cout << "Name        : " << courseName << "\n";
        std::cout << "Doctor      : " << doctorName << "\n";
        std::cout << "Assignments : " << assignments << "\n";
        std::cout << "Students    : " << students << "\n";
    }else {
        std::cout << "Failed to retrieve course details.\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
}

void Student::viewFriends() {

    Database db;
    if (!db.connect(connectionString)) return;

    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT s.id, s.name "
        "FROM Student s "
        "JOIN Friend f ON "
        "(f.student_id = " + std::to_string(this->id) + " AND s.id = f.friend_id) "
        "OR "
        "(f.friend_id = " + std::to_string(this->id) + " AND s.id = f.student_id);"
    );

    if (stmt == NULL) {
        std::cout << "Failed to retrieve friends.\n";
        return;
    }

    std::vector<int> friendIDs;
    std::vector<std::string> friendNames;

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        int id;
        char name[100];

        SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

        friendIDs.push_back(id);
        friendNames.push_back(name);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    if (friendIDs.empty()) {
        std::cout << "You have no friends yet.\n";
        return;
    }

    std::cout << "\nYour Friends:\n";
    for (int i = 0; i < friendIDs.size(); i++) {
        std::cout << i + 1 << ". " << friendNames[i] << "\n";
    }

    int choice = validateChoice(1, friendIDs.size(), "Choose friend: ");
    int selectedFriend = friendIDs[choice - 1];

    // 🔥 Now show friend details
    SQLHSTMT stmt2 = db.executeQueryHandle(
        "SELECT s.id, s.name, s.username, "
        "(SELECT COUNT(*) FROM Student_Course sc WHERE sc.student_id = s.id), "
        "(SELECT COUNT(*) FROM Friend fr WHERE (fr.student_id = s.id OR fr.friend_id = s.id)) "
        "FROM Student s "
        "WHERE s.id = " + std::to_string(selectedFriend) + ";"
    );

    if (stmt2 != NULL && SQLFetch(stmt2) == SQL_SUCCESS) {

        int id;
        char name[100];
        char username[100];
        int coursesCount;
        int friendsCount;

        SQLGetData(stmt2, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt2, 2, SQL_C_CHAR, name, sizeof(name), NULL);
        SQLGetData(stmt2, 3, SQL_C_CHAR, username, sizeof(username), NULL);
        SQLGetData(stmt2, 4, SQL_C_SLONG, &coursesCount, 0, NULL);
        SQLGetData(stmt2, 5, SQL_C_SLONG, &friendsCount, 0, NULL);

        std::cout << "\n--- Friend Profile ---\n";
        std::cout << "ID: " << id << "\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Username: " << username << "\n";
        std::cout << "Courses: " << coursesCount << "\n";
        std::cout << "Friends: " << friendsCount << "\n";
    } else {
        std::cout << "Failed to retrieve friend details.\n";
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
}

void Student::enrollInCourse() {

    Database db;
    if (!db.connect(connectionString)) return;

    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT id, name "
        "FROM Course "
        "WHERE id NOT IN ( "
        "   SELECT course_id "
        "   FROM Student_Course "
        "   WHERE student_id = " + std::to_string(this->id) +
        ");"
    );

    if (stmt == NULL) {
        std::cout << "Failed to retrieve courses.\n";
        return;
    }

    std::vector<int> courseIDs;
    std::vector<std::string> courseNames;

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        int id;
        char name[100];

        SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

        courseIDs.push_back(id);
        courseNames.push_back(name);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    if (courseIDs.empty()) {
        std::cout << "No courses available.\n";
        return;
    }

    std::cout << "\nAvailable Courses:\n";
    for (int i = 0; i < courseIDs.size(); i++) {
        std::cout << i + 1 << ". " << courseNames[i] << "\n";
    }

    int choice = validateChoice(1, courseIDs.size(), "Choose course: ");
    int selectedCourse = courseIDs[choice - 1];

    SQLHSTMT stmt2 = db.executeQueryHandle(
        "SELECT c.id, c.name, d.name, "
        "(SELECT COUNT(*) FROM Assignment WHERE course_id = c.id), "
        "(SELECT COUNT(*) FROM Student_Course WHERE course_id = c.id) "
        "FROM Course c "
        "JOIN Doctor d ON d.id = c.doctor_id "
        "WHERE c.id = " + std::to_string(selectedCourse) + ";"
    );

    if (stmt2 != NULL && SQLFetch(stmt2) == SQL_SUCCESS) {

        int id;
        char courseName[100];
        char doctorName[100];
        int assignments;
        int students;

        SQLGetData(stmt2, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt2, 2, SQL_C_CHAR, courseName, sizeof(courseName), NULL);
        SQLGetData(stmt2, 3, SQL_C_CHAR, doctorName, sizeof(doctorName), NULL);
        SQLGetData(stmt2, 4, SQL_C_SLONG, &assignments, 0, NULL);
        SQLGetData(stmt2, 5, SQL_C_SLONG, &students, 0, NULL);

        std::cout << "\n--- Course Details ---\n";
        std::cout << "ID          : " << id << "\n";
        std::cout << "Name        : " << courseName << "\n";
        std::cout << "Doctor      : " << doctorName << "\n";
        std::cout << "Assignments : " << assignments << "\n";
        std::cout << "Students    : " << students << "\n";

        int confirm = validateChoice(0, 1, 
            "Do you want to enroll in this course? [yes=1 / no=0]: ");

        if (confirm == 1) {

            std::string insertQuery =
                "INSERT INTO Student_Course (student_id, course_id) VALUES ("
                + std::to_string(this->id) + ", "
                + std::to_string(id) + ");";

            if (db.executeNonQuery(insertQuery)) {
                std::cout << "Enrolled successfully.\n";
            } else {
                std::cout << "Failed to enroll.\n";
            }
        }
        else {
            std::cout << "Enrollment cancelled.\n";
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
}

void Student::viewAssignmentReport() {

    Database db;
    if (!db.connect(connectionString)) return;

    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT c.id, c.name "
        "FROM Course c "
        "JOIN Student_Course sc ON sc.course_id = c.id "
        "WHERE sc.student_id = " + std::to_string(this->id) + ";"
    );

    if (stmt == NULL) {
        std::cout << "Failed to retrieve courses.\n";
        return;
    }

    std::vector<int> courseIDs;
    std::vector<std::string> courseNames;

    while (SQLFetch(stmt) == SQL_SUCCESS) {
        int id;
        char name[100];

        SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
        SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

        courseIDs.push_back(id);
        courseNames.push_back(name);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    if (courseIDs.empty()) {
        std::cout << "You are not enrolled in any courses.\n";
        return;
    }

    std::cout << "\nYour Courses:\n";
    for (int i = 0; i < courseIDs.size(); i++) {
        std::cout << i + 1 << ". " << courseNames[i] << "\n";
    }

    int choice = validateChoice(1, courseIDs.size(), "Choose course: ");
    int selectedCourse = courseIDs[choice - 1];

    SQLHSTMT stmt2 = db.executeQueryHandle(
        "SELECT a.id, sa.status "
        "FROM Assignment a "
        "LEFT JOIN Student_Assignment sa "
        "ON sa.assignment_id = a.id "
        "AND sa.student_id = " + std::to_string(this->id) + " "
        "WHERE a.course_id = " + std::to_string(selectedCourse) + ";"
    );

    if (stmt2 == NULL) {
        std::cout << "Failed to retrieve assignments.\n";
        return;
    }

    std::cout << "\n--- Assignment Report ---\n";

    bool hasAssignments = false;

    while (SQLFetch(stmt2) == SQL_SUCCESS) {
        hasAssignments = true;

        int assignmentId;
        char status[50];

        SQLGetData(stmt2, 1, SQL_C_SLONG, &assignmentId, 0, NULL);

        // status may be NULL
        SQLLEN indicator;
        SQLGetData(stmt2, 2, SQL_C_CHAR, status, sizeof(status), &indicator);

        std::cout << "Assignment ID: " << assignmentId << " | Status: ";

        if (indicator == SQL_NULL_DATA) {
            std::cout << "Not Attempted";
        } else {
            std::cout << status;
        }

        std::cout << "\n";
    }

    if (!hasAssignments) {
        std::cout << "No assignments in this course.\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
}

void Student::solveAssignment() {
    Database db;
    if (!db.connect(connectionString)) return;

    while (true) {
        // Get courses student is in that have assignments
        SQLHSTMT stmt = db.executeQueryHandle(
            "SELECT c.id, c.name, COUNT(a.id) AS total_assignments "
            "FROM Course c "
            "JOIN Student_Course sc ON sc.course_id = c.id "
            "JOIN Assignment a ON a.course_id = c.id "
            "WHERE sc.student_id = " + std::to_string(this->id) + " "
            "GROUP BY c.id, c.name;"
        );

        if (!stmt) {
            std::cout << "Failed to retrieve courses.\n";
            return;
        }

        std::vector<int> courseIDs;
        std::vector<std::string> courseNames;
        std::vector<int> courseAssignments;

        while (SQLFetch(stmt) == SQL_SUCCESS) {
            int id, totalAssignments;
            char name[100];

            SQLGetData(stmt, 1, SQL_C_SLONG, &id, 0, NULL);
            SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
            SQLGetData(stmt, 3, SQL_C_SLONG, &totalAssignments, 0, NULL);

            courseIDs.push_back(id);
            courseNames.push_back(name);
            courseAssignments.push_back(totalAssignments);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);

        if (courseIDs.empty()) {
            std::cout << "No courses with assignments found.\n";
            return;
        }

        // Show courses
        std::cout << "\nCourses with Assignments:\n";
        for (size_t i = 0; i < courseIDs.size(); i++) {
            std::cout << i + 1 << ". " << courseNames[i] << " - have " << courseAssignments[i] << " assignments\n";
        }
        std::cout << courseIDs.size() + 1 << ". Exit\n";

        int choice = validateChoice(1, courseIDs.size() + 1, "Choose a course: ");
        if (choice == courseIDs.size() + 1) break;

        int selectedCourse = courseIDs[choice - 1];

        // Get unsolved assignments with correct answer
        SQLHSTMT stmt2 = db.executeQueryHandle(
            "SELECT a.id, a.question, a.answer1, a.answer2, a.answer3, a.answer4, a.correct_answer "
            "FROM Assignment a "
            "LEFT JOIN Student_Assignment sa ON sa.assignment_id = a.id AND sa.student_id = " + std::to_string(this->id) + " "
            "WHERE a.course_id = " + std::to_string(selectedCourse) + " AND (sa.status IS NULL OR sa.status = '');"
        );

        if (!stmt2) {
            std::cout << "Failed to retrieve assignments.\n";
            continue;
        }

        std::vector<int> unsolvedIDs;
        std::vector<std::string> questions;
        std::vector<std::vector<std::string>> answers;
        std::vector<int> correctAnswers;

        while (SQLFetch(stmt2) == SQL_SUCCESS) {
            int id, correct;
            char question[256], ans1[100], ans2[100], ans3[100], ans4[100];

            SQLGetData(stmt2, 1, SQL_C_SLONG, &id, 0, NULL);
            SQLGetData(stmt2, 2, SQL_C_CHAR, question, sizeof(question), NULL);
            SQLGetData(stmt2, 3, SQL_C_CHAR, ans1, sizeof(ans1), NULL);
            SQLGetData(stmt2, 4, SQL_C_CHAR, ans2, sizeof(ans2), NULL);
            SQLGetData(stmt2, 5, SQL_C_CHAR, ans3, sizeof(ans3), NULL);
            SQLGetData(stmt2, 6, SQL_C_CHAR, ans4, sizeof(ans4), NULL);
            SQLGetData(stmt2, 7, SQL_C_SLONG, &correct, 0, NULL); // 1-based index

            unsolvedIDs.push_back(id);
            questions.push_back(question);
            answers.push_back({ans1, ans2, ans3, ans4});
            correctAnswers.push_back(correct - 1); // convert to 0-based index
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt2);

        if (unsolvedIDs.empty()) {
            std::cout << "You have solved all assignments in this course.\n";
            continue;
        }

        // Solve assignments one by one
        for (size_t i = 0; i < unsolvedIDs.size(); i++) {
            std::cout << "\nAssignment: " << questions[i] << "\n";
            for (size_t j = 0; j < answers[i].size(); j++) {
                std::cout << j + 1 << ". " << answers[i][j] << "\n";
            }

            int ans = validateChoice(1, 4, "Enter your answer number: ") - 1;

            // Save answer in DB
            std::string status = (ans == correctAnswers[i]) ? "Correct" : "Wrong";
            db.executeNonQuery(
                "INSERT INTO Student_Assignment (student_id, assignment_id, status) VALUES (" +
                std::to_string(this->id) + ", " + std::to_string(unsolvedIDs[i]) + ", '" + status + "');"
            );

            // Show feedback
            if (ans == correctAnswers[i]) {
                std::cout << "Correct! Your answer is saved.\n";
            } else {
                std::cout << "Wrong! The correct answer is: "
                          << correctAnswers[i] + 1 << ". " << answers[i][correctAnswers[i]] << "\n";
            }

            // Ask if want to continue
            int cont;
            std::cout << "Do you want to continue solving assignments? [yes=1 / no=0]: ";
            std::cin >> cont;
            if (cont != 1) break;
        }
    }
}

void Student::addFriend() {
    Database db;
    if (!db.connect(connectionString)) return;

    std::string username;
    std::cout << "Enter the username of the friend: ";
    std::cin >> username;

    // Get friend info + counts
    SQLHSTMT stmt = db.executeQueryHandle(
        "SELECT s.id, s.name, s.username, "
        "(SELECT COUNT(*) FROM Student_Course sc WHERE sc.student_id = s.id), "
        "(SELECT COUNT(*) FROM Friend f WHERE f.student_id = s.id OR f.friend_id = s.id) "
        "FROM Student s "
        "WHERE s.username = '" + username + "';"
    );

    if (!stmt) {
        std::cout << "Query failed!\n";
        return;
    }

    int friendId;
    char name[100];
    char user[100];
    int courseCount;
    int friendCount;

    if (SQLFetch(stmt) != SQL_SUCCESS) {
        std::cout << "User not found.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return;
    }

    SQLGetData(stmt, 1, SQL_C_SLONG, &friendId, 0, NULL);
    SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
    SQLGetData(stmt, 3, SQL_C_CHAR, user, sizeof(user), NULL);
    SQLGetData(stmt, 4, SQL_C_SLONG, &courseCount, 0, NULL);
    SQLGetData(stmt, 5, SQL_C_SLONG, &friendCount, 0, NULL);

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    // Can't add yourself
    if (friendId == this->id) {
        std::cout << "You cannot add yourself.\n";
        return;
    }

    // Check if already friends
    SQLHSTMT checkStmt = db.executeQueryHandle(
        "SELECT * FROM Friend "
        "WHERE (student_id = " + std::to_string(this->id) +
        " AND friend_id = " + std::to_string(friendId) + ") "
        "OR (student_id = " + std::to_string(friendId) +
        " AND friend_id = " + std::to_string(this->id) + ");"
    );

    if (checkStmt && SQLFetch(checkStmt) == SQL_SUCCESS) {
        std::cout << "You are already friends.\n";
        SQLFreeHandle(SQL_HANDLE_STMT, checkStmt);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, checkStmt);

    // Show friend details
    std::cout << "\n--- Friend Details ---\n";
    std::cout << "ID: " << friendId << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Username: " << user << "\n";
    std::cout << "Courses: " << courseCount << "\n";
    std::cout << "Friends: " << friendCount << "\n";

    int confirm;
    std::cout << "\nDo you want to be her friend? [yes=1 / no=0]: ";
    std::cin >> confirm;

    if (confirm != 1) {
        std::cout << "Cancelled.\n";
        return;
    }

    // Send notification instead of directly adding friend
    db.executeNonQuery(
        "INSERT INTO Notification ([from], [to]) VALUES (" +
        std::to_string(this->id) + ", " +
        std::to_string(friendId) + ");"
    );

    std::cout << "Friend request sent successfully!\n";
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
            stu.enrollInCourse();
        } else if (choice == 3) {
            // List my courses
            stu.viewCourses();
        } else if (choice == 4) {
            // Solve Assignment
            stu.solveAssignment();
        } else if (choice == 5) {
            // Assignment Report
            stu.viewAssignmentReport();
        } else if (choice == 6) {
            // Add Friend
            stu.addFriend();
        } else if (choice == 7) {
            // List my Friends
            stu.viewFriends();
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