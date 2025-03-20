#include "utils/EnrollmentUtils.hpp"
#include "utils/DataLoader.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_map>

struct Grade {
  int exam_grade;
  int usual_grade;
  int composite_grade;
};

void get_enrollment_list(const std::vector<Enrollment> &enrollments,
                         const std::vector<Student> &students,
                         const std::vector<Course> &courses) {
  std::unordered_map<std::string, std::vector<std::pair<std::string, Grade>>>
      enrollment_list;
  // courseID 映射 pair（studentID、成绩）

  for (const auto &enrollment : enrollments) {
    Grade grade = {enrollment.getExamGrade(), enrollment.getUsualGrade(),
                   enrollment.getCompositeGrade()};
    enrollment_list[enrollment.getCourseId()].emplace_back(
        enrollment.getStudentId(), grade);
  }

  for (const auto &course : courses) {
    std::cout << "课程ID: " << course.getId()
              << ", 课程名称: " << course.getName() << std::endl;
    if (enrollment_list.find(course.getId()) != enrollment_list.end()) {
      for (const auto &studentGrade : enrollment_list[course.getId()]) {
        auto student = std::find_if(
            students.begin(), students.end(),
            [&](const Student &s) { return s.getId() == studentGrade.first; });
        if (student != students.end()) {
          std::cout << "学生ID: " << student->getId()
                    << ", 姓名: " << student->getName()
                    << ", 考试成绩: " << studentGrade.second.exam_grade
                    << ", 平时成绩: " << studentGrade.second.usual_grade
                    << ", 综合成绩: " << studentGrade.second.composite_grade
                    << std::endl;
        }
      }
    } else {
      std::cout << "没有学生选修此课程。" << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
  }
}

void cal_credits(const std::vector<Enrollment> &enrollments,
                 const std::vector<Student> &students,
                 const std::vector<Course> &courses) {
  std::unordered_map<std::string, int> studentCredits;

  for (const auto &enrollment : enrollments) {
    auto course =
        std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
          return c.getId() == enrollment.getCourseId();
        });
    if (course != courses.end() && enrollment.getCompositeGrade() >= 60) {
      studentCredits[enrollment.getStudentId()] += course->getCredit();
    }
  }

  for (const auto &student : students) {
    std::cout << "学生ID: " << student.getId()
              << ", 姓名: " << student.getName() << std::endl;
    std::cout << "已修学分: " << studentCredits[student.getId()] << std::endl;
    std::cout << "----------------------------------------" << std::endl;
  }
}

void cal_retake_course(const std::vector<Enrollment> &enrollments,
                       const std::vector<Student> &students,
                       const std::vector<Course> &courses) {
  std::unordered_map<std::string, std::vector<std::string>> studentFailures;

  for (const auto &enrollment : enrollments) {
    auto course =
        std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
          return c.getId() == enrollment.getCourseId();
        });
    if (course != courses.end() && enrollment.getExamGrade() < 60) {
      studentFailures[enrollment.getStudentId()].push_back(course->getName());
    }
  }

  for (const auto &student : students) {
    std::cout << "学生ID: " << student.getId()
              << ", 姓名: " << student.getName() << std::endl;
    std::cout << "不及格课程: ";
    if (studentFailures.find(student.getId()) != studentFailures.end()) {
      for (const auto &courseName : studentFailures[student.getId()]) {
        std::cout << courseName << " ";
      }
    } else {
      std::cout << "无";
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
  }
}

void add_enrollment(std::vector<Enrollment> &enrollments) {
  std::string studentId, courseId;

  std::cout << "请输入学生ID: ";
  std::cin >> studentId;
  std::cout << "请输入课程ID: ";
  std::cin >> courseId;

  // 检查是否已经存在相同的学生ID和课程ID
  auto enrollment = std::find_if(
      enrollments.begin(), enrollments.end(), [&](const Enrollment &e) {
        return e.getStudentId() == studentId && e.getCourseId() == courseId;
      });

  if (enrollment != enrollments.end()) {
    std::cout << "该学生已经选修了该课程，不能重复录入。" << std::endl;
  } else {
    enrollments.emplace_back(studentId, courseId, -1, -1, false,
                             -1); // -1代表没出成绩
    DataLoader::saveEnrollments("../data/enrollment.json", enrollments);
    std::cout << "选课信息录入成功！" << std::endl;
  }
}

void modify_student_grade(std::vector<Enrollment> &enrollments) {
  std::string studentId, courseId;
  int examGrade, usualGrade, compositeGrade;

  std::cout << "请输入学生ID: ";
  std::cin >> studentId;
  std::cout << "请输入课程ID: ";
  std::cin >> courseId;

  auto enrollment = std::find_if(
      enrollments.begin(), enrollments.end(), [&](const Enrollment &e) {
        return e.getStudentId() == studentId && e.getCourseId() == courseId;
      });

  if (enrollment != enrollments.end()) {
    std::cout << "请输入考试成绩: ";
    std::cin >> examGrade;
    std::cout << "请输入平时成绩: ";
    std::cin >> usualGrade;

    enrollment->setExamGrade(examGrade);
    enrollment->setUsualGrade(usualGrade);
    enrollment->setCompositeGrade((examGrade + usualGrade) / 2);

    int is_retake;
    do {
      std::cout << "是否为重修成绩？(0为否，1为是): ";
      std::cin >> is_retake;
      if (std::cin.fail() || (is_retake != 0 && is_retake != 1)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "无效输入，请输入0或1。\n";
      }
    } while (is_retake != 0 && is_retake != 1);
    enrollment->set_is_retake(is_retake == 1);
    DataLoader::saveEnrollments("../data/enrollment.json", enrollments);
    std::cout << "学生成绩录入成功！" << std::endl;
  } else {
    std::cout << "未找到该学生的选课信息。" << std::endl;
  }
}

void EnrollmentInfo(std::vector<Enrollment> &enrollments,
                    const std::vector<Student> &students,
                    const std::vector<Course> &courses) {
  int choice = -1;
  do {
    std::vector<std::string> options = {
        "返回上一级",   "查询选课信息",     "录入选课信息",
        "录入学生成绩", "查询学生所获学分", "查询学生不及格课程"};
    std::cout << "\n+------------------------------------+" << std::endl;
    std::cout << "|            选课信息菜单             |" << std::endl;
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
      std::cout << "|           查询选课信息              |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      get_enrollment_list(enrollments, students, courses);
      break;
    }
    case 2:
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           录入选课信息              |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      add_enrollment(enrollments);
      break;
    case 3:
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           录入学生成绩              |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      modify_student_grade(enrollments);
      break;
    case 4:
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           查询学生所获学分          |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      cal_credits(enrollments, students, courses);
      break;
    case 5:
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           查询学生不及格课程        |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      cal_retake_course(enrollments, students, courses);
      break;
    default:
      std::cout << "无效选择，请选择有效的选项。" << std::endl;
    }
  } while (choice != 0);
}