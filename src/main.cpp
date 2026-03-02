#include "../headers/functions.h"
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

void CleanLogs() {
    std::string folderPath = "F:/Educational-Management-System/Logs"; // path logs folder
    int maxFiles = 5; // maximum number of log files

    std::vector<fs::directory_entry> files;

    if (!fs::exists(folderPath)) {
        std::cout << "Logs folder not found!\n";
        return;
    }

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        }
    }

    if (files.size() >= maxFiles) {

        std::sort(files.begin(), files.end(),
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                return fs::last_write_time(a) < fs::last_write_time(b);
            });

        std::cout << "Deleting: " << files[0].path() << std::endl;
        fs::remove(files[0].path());
    }
}

int main() {
    // Welcome message
    std::cout<<"\tWelcome to the Educational Management System!\n\n";

    // Clean old log files
    CleanLogs();

    // Start the application
    welcome();
    
    return 0;
}

// Running the program
// g++ assignment.cpp course.cpp DB.cpp doctor.cpp functions.cpp main.cpp student.cpp user.cpp -o main.exe -lodbc32