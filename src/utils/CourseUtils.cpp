#include "utils/CourseUtils.hpp"
#include "utils/DataLoader.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

void get_course_info(const std::vector<Course> &courses) {
    std::string courseId;
    std::cout << "请输入课程ID: ";
    std::cin >> courseId;

    auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
        return c.getId() == courseId;
    });

    if (course != courses.end()) {
        std::cout << "课程ID: " << course->getId() << std::endl;
        std::cout << "课程名称: " << course->getName() << std::endl;
        std::cout << "课程学分: " << course->getCredit() << std::endl;
    } else {
        std::cout << "未找到该课程信息。" << std::endl;
    }
}

void add_course(std::vector<Course> &courses) {
    std::string courseId, courseName;
    int credit;

    std::cout << "请输入课程ID: ";
    std::cin >> courseId;
    std::cout << "请输入课程名称: ";
    std::cin >> courseName;
    std::cout << "请输入课程学分: ";
    std::cin >> credit;

    // 检查是否已经存在相同的课程ID
    auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
        return c.getId() == courseId;
    });

    if (course != courses.end()) {
        std::cout << "该课程ID已存在，不能重复录入。" << std::endl;
    } else {
        courses.emplace_back(courseId, courseName, credit);
        DataLoader::saveCourses("../data/course.json", courses);
        std::cout << "课程信息录入成功！" << std::endl;
    }
}

void modify_course(std::vector<Course> &courses) {
    std::string courseId;
    std::cout << "请输入要修改的课程ID: ";
    std::cin >> courseId;

    auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
        return c.getId() == courseId;
    });

    if (course != courses.end()) {
        std::string courseName;
        int credit;

        std::cout << "请输入新的课程名称: ";
        std::cin >> courseName;
        std::cout << "请输入新的课程学分: ";
        std::cin >> credit;

        course->setName(courseName);
        course->setCredit(credit);

        DataLoader::saveCourses("../data/course.json", courses);
        std::cout << "课程信息修改成功！" << std::endl;
    } else {
        std::cout << "未找到该课程信息。" << std::endl;
    }
}

void search_course(const std::vector<Course> &courses) {
    std::string input;
    std::cout << "请输入查询关键字（使用'+'分隔多个关键字）: ";
    std::cin.ignore();
    std::getline(std::cin, input);

    std::istringstream iss(input);
    std::vector<std::string> keywords;
    std::string keyword;
    while (std::getline(iss, keyword, '+')) {
        keywords.push_back(keyword);
    }

    std::vector<Course> results;
    for (const auto &course : courses) {
        bool match = true;
        for (const auto &kw : keywords) {
            if (course.getName().find(kw) == std::string::npos) {
                match = false;
                break;
            }
        }
        if (match) {
            results.push_back(course);
        }
    }

    if (!results.empty()) {
        std::cout << "查询结果：" << std::endl;
        for (const auto &course : results) {
            std::cout << "课程ID: " << course.getId() << std::endl;
            std::cout << "课程名称: " << course.getName() << std::endl;
            std::cout << "课程学分: " << course.getCredit() << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    } else {
        std::cout << "未找到匹配的课程。" << std::endl;
    }
}

void CourseInfo(std::vector<Course> &courses) {
    int choice = -1;
    do {
        std::vector<std::string> options = {"返回上一级", "查询课程信息（按id）",
                                            "录入课程信息", "修改课程信息",
                                            "搜索课程信息（按名字）"};
        std::cout << "\n+------------------------------------+" << std::endl;
        std::cout << "|            课程信息菜单             |" << std::endl;
        std::cout << "+------------------------------------+" << std::endl;
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << "| " << std::setw(2) << i << ". " << std::left
                      << std::setw(30) << options[i] << " |" << std::endl;
        }
        std::cout << "+------------------------------------+" << std::endl;
        std::cout << "请输入您的选择：";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效输入，请输入数字。\n";
            continue;
        }

        switch (choice) {
        case 0:
            std::cout << "返回上一级..." << std::endl;
            break;
        case 1: {
            std::cout << "\n+------------------------------------+" << std::endl;
            std::cout << "|           查询课程信息              |" << std::endl;
            std::cout << "+------------------------------------+" << std::endl;
            get_course_info(courses);
            break;
        }
        case 2:
            std::cout << "\n+------------------------------------+" << std::endl;
            std::cout << "|           录入课程信息              |" << std::endl;
            std::cout << "+------------------------------------+" << std::endl;
            add_course(courses);
            break;
        case 3:
            std::cout << "\n+------------------------------------+" << std::endl;
            std::cout << "|           修改课程信息              |" << std::endl;
            std::cout << "+------------------------------------+" << std::endl;
            modify_course(courses);
            break;
        case 4:
            std::cout << "\n+------------------------------------+" << std::endl;
            std::cout << "|            搜索课程信息              |" << std::endl;
            std::cout << "+------------------------------------+" << std::endl;
            search_course(courses);
            break;
        default:
            std::cout << "无效选择，请选择有效的选项。" << std::endl;
        }
    } while (choice != 0);
}