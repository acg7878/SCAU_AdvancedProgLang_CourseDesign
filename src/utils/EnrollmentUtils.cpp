#include "utils/EnrollmentUtils.hpp"
#include "utils/DataLoader.hpp"
#include <algorithm>
#include <string>
#include <vector>



std::vector<Student>
get_students_by_course_id(const std::vector<Enrollment> &enrollments,
                          const std::vector<Student> &students,
                          const std::string &courseId) {
  std::vector<Student> result;

  for (const auto &enrollment : enrollments) {
    if (enrollment.getCourseId() == courseId) {
      auto student =
          std::find_if(students.begin(), students.end(), [&](const Student &s) {
            return s.getId() == enrollment.getStudentId();
          });
      if (student != students.end()) {
        result.push_back(*student);
      }
    }
  }

  return result;
}

bool add_enrollment(std::vector<Enrollment> &enrollments,
                    const std::string &studentId, const std::string &courseId) {
  auto enrollment = std::find_if(
      enrollments.begin(), enrollments.end(), [&](const Enrollment &e) {
        return e.getStudentId() == studentId && e.getCourseId() == courseId;
      });

  if (enrollment != enrollments.end()) {
    return false; // 已存在相同的选课记录
  } else {
    enrollments.emplace_back(studentId, courseId, -1, -1, false,
                             -1); // -1代表成绩未录入
    DataLoader::saveEnrollments("../data/enrollment.json", enrollments);
    return true; // 录入成功
  }
}

int count_failing_students_by_course_id(const std::vector<Enrollment> &enrollments,
                                        const std::string &courseId) {
    int count = 0;

    for (const auto &enrollment : enrollments) {
        if (enrollment.getCourseId() == courseId && enrollment.getCompositeGrade() < 60) {
            count++;
        }
    }

    return count;
}

std::vector<std::string> get_failing_courses_by_student_id(const std::vector<Enrollment> &enrollments,
                                                           const std::vector<Course> &courses,
                                                           const std::string &studentId) {
    std::vector<std::string> result;

    for (const auto &enrollment : enrollments) {
        if (enrollment.getStudentId() == studentId && enrollment.getCompositeGrade() < 60) {
            auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
                return c.getId() == enrollment.getCourseId();
            });
            if (course != courses.end()) {
                result.push_back(course->getName());
            }
        }
    }

    return result;
}

bool update_enrollment_grade(std::vector<Enrollment> &enrollments,
                             const std::string &studentId,
                             const std::string &courseId,
                             int examGrade,
                             int usualGrade) {
    auto enrollment = std::find_if(enrollments.begin(), enrollments.end(), [&](Enrollment &e) {
        return e.getStudentId() == studentId && e.getCourseId() == courseId;
    });

    if (enrollment != enrollments.end()) {
        enrollment->setExamGrade(examGrade);
        enrollment->setUsualGrade(usualGrade);
        enrollment->setCompositeGrade((examGrade + usualGrade) / 2); // 假设综合成绩为两者平均
        DataLoader::saveEnrollments("../data/enrollment.json", enrollments);
        return true; // 成绩更新成功
    }

    return false; // 未找到对应的选课记录
}