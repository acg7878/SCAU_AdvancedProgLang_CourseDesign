#include "utils/CourseUtils.hpp"
#include "core/Course.hpp"
#include "utils/DataLoader.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
// 查询课程信息（按ID）
#include <stdexcept> // 用于抛出异常

Course findCourseById(const std::vector<Course> &courses, const std::string &courseId) {
    auto course = std::find_if(courses.begin(), courses.end(),
                               [&](const Course &c) { return c.getId() == courseId; });

    if (course != courses.end()) {
        return *course; // 返回找到的课程对象
    } else {
        throw std::runtime_error("未找到该课程信息。"); // 抛出异常
    }
}

// 录入课程信息
bool add_course(std::vector<Course> &courses, const std::string &courseId,
                const std::string &courseName, int credit) {
    auto course = std::find_if(courses.begin(), courses.end(),
                               [&](const Course &c) { return c.getId() == courseId; });

    if (course != courses.end()) {
        return false;
    } else {
        courses.emplace_back(courseId, courseName, credit);
        DataLoader::saveCourses("../data/course.json", courses);
        return true;
    }
}

// 修改课程信息
std::string modify_course(std::vector<Course> &courses, const std::string &courseId,
                          const std::string &newName, int newCredit) {
    auto course = std::find_if(courses.begin(), courses.end(),
                               [&](const Course &c) { return c.getId() == courseId; });

    if (course != courses.end()) {
        course->setName(newName);
        course->setCredit(newCredit);
        DataLoader::saveCourses("../data/course.json", courses);
        return "课程信息修改成功！";
    } else {
        return "未找到该课程信息。";
    }
}

bool delete_course(std::vector<Course> &courses, const std::string &courseId) {
    auto course = std::find_if(courses.begin(), courses.end(),
                               [&](const Course &c) { return c.getId() == courseId; });

    if (course != courses.end()) {
        courses.erase(course); // 删除找到的课程
        DataLoader::saveCourses("../data/course.json",
                                courses); // 保存更新后的课程列表
        return true;                      // 删除成功
    }

    return false; // 未找到课程，删除失败
}

std::unordered_set<Course> search_course(const std::vector<Course> &courses,
                                         const std::string &keywords) {
    std::unordered_set<Course> results;

    // 将关键词通过空格分割
    std::istringstream iss(keywords);
    std::vector<std::string> keyword_list{std::istream_iterator<std::string>{iss},
                                          std::istream_iterator<std::string>{}};

    // 遍历课程列表
    for (const auto &course : courses) {
        bool match = true;
        // 检查每个关键词是否都在课程名称中
        for (const auto &keyword : keyword_list) {
            if (course.getName().find(keyword) == std::string::npos) {
                match = false;
                break;
            }
        }
        // 如果所有关键词都匹配，则将课程加入结果集合
        if (match) {
            results.insert(course);
        }
    }
    return results;
}