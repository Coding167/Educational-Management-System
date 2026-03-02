#include "../headers/doctor.h"
#include "../headers/functions.h"
#include "../headers/course.h"
#include "../headers/assignment.h"
#include "../headers/student.h"
#include "../headers/DB.h"
#include <limits>


void Doctor::setID(std::string idStr) {
  this->id = stringToInt(idStr);

  Database db;
  if (!db.connect(connectionString)) return;

  SQLHSTMT stmt = db.executeQueryHandle(
    "SELECT id, name, username "
    "FROM Doctor "
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
    std::cout << "Failed to retrieve doctor data.\n";
  }
}

void Doctor::viewProfile() {
  std::cout << "Doctor Profile:\n";
  std::cout << "ID: " << this->id << "\n";
  std::cout << "Name: " << this->name << "\n";
  std::cout << "Username: " << this->username << "\n";

  Database db;
  if (!db.connect(connectionString)) return;

  SQLHSTMT stmt = db.executeQueryHandle(
    "SELECT COUNT(*) FROM Course WHERE doctor_id = " + std::to_string(this->id) + ";"
  );

  if (stmt != NULL && SQLFetch(stmt) == SQL_SUCCESS) {
    int courseCount;
    SQLGetData(stmt, 1, SQL_C_SLONG, &courseCount, 0, NULL);
    std::cout << "Courses Created: " << courseCount << "\n";
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
  } else {
    std::cout << "Failed to retrieve course count.\n";
  }
}

void Doctor::viewCourses() {
  Database db;
  if (!db.connect(connectionString)) return;

  SQLHSTMT stmt = db.executeQueryHandle(
    "SELECT id, name FROM Course WHERE doctor_id = " 
    + std::to_string(this->id) + ";"
  );

  if (stmt == NULL) {
    std::cout << "Failed to retrieve courses.\n";
    return;
  }

  std::vector<int> courseIDs;
  std::vector<std::string> courseNames;

  while (SQLFetch(stmt) == SQL_SUCCESS) {
    int courseId;
    char courseName[100];

    SQLGetData(stmt, 1, SQL_C_SLONG, &courseId, 0, NULL);
    SQLGetData(stmt, 2, SQL_C_CHAR, courseName, sizeof(courseName), NULL);

    courseIDs.push_back(courseId);
    courseNames.push_back(courseName);
  }

  SQLFreeHandle(SQL_HANDLE_STMT, stmt);

  if (courseIDs.empty()) {
    std::cout << "You have no courses yet.\n";
    return;
  }

  std::cout << "\nYour Courses:\n";
  for (int i = 0; i < courseIDs.size(); i++) {
    std::cout << i + 1 << ". "
              << courseNames[i]
              << " - "
              << courseIDs[i]
              << "\n";
  }

  int choice = validateChoice(1, courseIDs.size(), "Choose a Course: ");

  int selectedCourseId = courseIDs[choice - 1];

  showCourseDetails(selectedCourseId);
}

void Doctor::showCourseDetails(int courseId) {
  Database db;
  if (!db.connect(connectionString)) return;

  SQLHSTMT stmt = db.executeQueryHandle(
    "SELECT c.name, "
    "(SELECT COUNT(*) FROM Assignment WHERE course_id = c.id), "
    "(SELECT COUNT(*) FROM Student_Course WHERE course_id = c.id) "
    "FROM Course c "
    "WHERE c.id = " + std::to_string(courseId) + ";"
  );

  if (stmt != NULL && SQLFetch(stmt) == SQL_SUCCESS) {

    char name[100];
    int assignmentsCount;
    int studentsCount;

    SQLGetData(stmt, 1, SQL_C_CHAR, name, sizeof(name), NULL);
    SQLGetData(stmt, 2, SQL_C_SLONG, &assignmentsCount, 0, NULL);
    SQLGetData(stmt, 3, SQL_C_SLONG, &studentsCount, 0, NULL);

    std::cout << "\n--- Course Details ---\n";
    std::cout << "ID          : " << courseId << "\n";
    std::cout << "Name        : " << name << "\n";
    std::cout << "Assignments : " << assignmentsCount << "\n";
    std::cout << "Students    : " << studentsCount << "\n";
  }
  else {
    std::cout << "Failed to retrieve course details.\n";
  }

  SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void Doctor::addCourse(std::string name) {
  Database db;
  if (!db.connect(connectionString)) return;

  std::string query = "INSERT INTO Course (name, doctor_id) VALUES ('" + name + "', " + std::to_string(this->id) + ");";

  if (db.executeNonQuery(query)) {
    std::cout << "Course added successfully.\n";
  } else {
    std::cout << "Failed to add course.\n";
  }
}

void doctorStart(std::string personID) {
  // the doctor
  Doctor doc(personID);

  while (true) {
    // menu
    std::cout<<"\n1. View Profile\n"
            <<"2. Create a Course\n"
            <<"3. Add Assignment\n"
            <<"4. My Courses\n"
            <<"5. Log Out\n";
    int choice = validateChoice(1,5,"Please enter your choice: ");
    std::cout<<std::endl;

    if (choice == 1) {
      doc.viewProfile();
    } else if (choice == 2 ) {
      // Create a Course
      std::string name;
      std::cout<<"Enter the name of the Course: ";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::getline(std::cin,name);
      doc.addCourse(name);
    } else if (choice == 3) {
      // Add Assignment
      std::vector<Course*> courses = doc.getCourses();
      if (courses.empty()) {
        std::cout << "You have no courses.\n";
        continue;
      }
      // Show the courses
      for (int i = 0; i < courses.size(); ++i) {
        std::cout << i + 1 << ". " << courses[i]->getName() << " - " << courses[i]->getID() << std::endl;
      }
      // Select a course
      int c = validateChoice(1, courses.size(), "Enter the number of Course to view it: ");
      Course* currentCourse = courses[c - 1];

      std::string question;
      std::vector<std::string> answers;
      int n;

      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
      doc.viewCourses();
    }else {
      // Log out
      break;
    }
  }
  welcome();
}