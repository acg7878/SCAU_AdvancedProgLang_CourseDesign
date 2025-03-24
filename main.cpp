#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "core/Enrollment.hpp"
#include "utils/DataLoader.hpp"
#include "utils/EnrollmentUtils.hpp"
#include "utils/StudentUtils.hpp"
#include "utils/CourseUtils.hpp"

void displayMenu(const std::vector<std::string>& options) {
    std::cout << "+--------------------------------+" << std::endl;
    std::cout << "|       \033[1;34m教务信息管理系统\033[0m         |" << std::endl;
    std::cout << "+--------------------------------+" << std::endl;

    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "| " << std::setw(2) << i + 1 << ". " << std::left << std::setw(30) << options[i] << " |" << std::endl;
    }
    std::cout << "+--------------------------------+" << std::endl;
    std::cout << "|  0. 退出                       |" << std::endl;
    std::cout << "+--------------------------------+" << std::endl;
}

int main() {
    std::vector<Student> students = DataLoader::loadStudents("../data/student.json");
    std::vector<Course> courses = DataLoader::loadCourses("../data/course.json");
    std::vector<Enrollment> enrollments = DataLoader::loadEnrollments("../data/enrollment.json");
    // debug代码
    // enrollments.pop_back();
    // DataLoader::saveEnrollments("../data/enrollment.json", enrollments); 
    
    std::vector<std::string> options = {
        "学生信息",
        "选课信息",
        "课程信息"
    };

    int choice = -1;
    do {
        displayMenu(options);
        std::cout << "\n请输入您的选择：";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效输入，请输入数字。\n";
            continue;
        }

        switch (choice) {
            case 0:
                std::cout << "正在退出... 再见！" << std::endl;
                break;
            case 1:
                StudentInfo(students);
                break;
            case 2:
                EnrollmentInfo(enrollments, students, courses);
                break;
            case 3:
                CourseInfo(courses);
                break;
            default:
                std::cout << "无效选择，请选择有效的选项。" << std::endl;
        }
        std::cout << "\n";
    } while (choice != 0);

    return 0;
}