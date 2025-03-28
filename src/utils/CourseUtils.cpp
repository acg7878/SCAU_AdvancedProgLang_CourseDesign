#include "utils/CourseUtils.hpp"
#include "utils/DataLoader.hpp"
#include <algorithm>
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
  auto course =
      std::find_if(courses.begin(), courses.end(),
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
std::string modify_course(std::vector<Course> &courses,
                          const std::string &courseId,
                          const std::string &newName, int newCredit) {
  auto course =
      std::find_if(courses.begin(), courses.end(),
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

// 搜索课程信息（按名字关键字）
std::vector<std::vector<std::string>>
search_course(const std::vector<Course> &courses,
              const std::vector<std::string> &keywords) {
  std::vector<std::vector<std::string>> results;

  for (const auto &course : courses) {
    bool match = true;
    for (const auto &kw : keywords) {
      if (course.getName().find(kw) == std::string::npos) {
        match = false;
        break;
      }
    }
    if (match) {
      results.push_back({course.getId(), course.getName(),
                         std::to_string(course.getCredit())});
    }
  }

  return results;
}
