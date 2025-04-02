#include "utils/EnrollmentUtils.hpp"
#include "utils/DataLoader.hpp"
#include <algorithm>
#include <string>
#include <vector>

std::vector<Student> get_students_by_course_id(const std::vector<Enrollment> &enrollments,
                                               const std::vector<Student> &students,
                                               const std::string &courseId) {
    std::vector<Student> result;

    for (const auto &enrollment : enrollments) {
        if (enrollment.getCourseId() == courseId) {
            auto student = std::find_if(students.begin(), students.end(), [&](const Student &s) {
                return s.getId() == enrollment.getStudentId();
            });
            if (student != students.end()) {
                result.push_back(*student);
            }
        }
    }

    return result;
}

bool add_enrollment(std::vector<Enrollment> &enrollments, const std::string &studentId,
                    const std::string &courseId) {
    auto enrollment =
        std::find_if(enrollments.begin(), enrollments.end(), [&](const Enrollment &e) {
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

std::vector<std::tuple<std::string, std::string, std::string, int>>
    get_failing_students(const std::vector<Enrollment> &enrollments,
                         const std::vector<Student> &students, const std::vector<Course> &courses) {
    std::vector<std::tuple<std::string, std::string, std::string, int>> result;

    for (const auto &enrollment : enrollments) {
        if (enrollment.getCompositeGrade() < 60) { // 筛选不及格的记录
            auto student = std::find_if(students.begin(), students.end(), [&](const Student &s) {
                return s.getId() == enrollment.getStudentId();
            });

            auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
                return c.getId() == enrollment.getCourseId();
            });

            if (student != students.end() && course != courses.end()) {
                result.emplace_back(student->getId(),              // 学生ID
                                    student->getName(),            // 学生姓名
                                    course->getName(),             // 课程名称
                                    enrollment.getCompositeGrade() // 综合成绩
                );
            }
        }
    }

    return result;
}

std::vector<std::tuple<std::string, std::string, int>> get_failing_courses_by_student_id(
    const std::vector<Enrollment> &enrollments, const std::vector<Student> &students,
    const std::vector<Course> &courses, const std::string &studentID) {
    std::vector<std::tuple<std::string, std::string, int>> result;
    // 学生名、课程名、综合成绩
    auto student = std::find_if(students.begin(), students.end(),
                                [&](const Student &s) { return s.getId() == studentID; });

    if (student == students.end()) {
        return result;
    }

    std::string studentName = student->getName();

    for (const auto &enrollment : enrollments) {
        if (enrollment.getStudentId() == studentID && enrollment.getCompositeGrade() < 60 &&
            enrollment.getCompositeGrade() >= 0) {
            auto course = std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
                return c.getId() == enrollment.getCourseId();
            });
            if (course != courses.end()) {
                result.emplace_back(studentName, course->getName(), enrollment.getCompositeGrade());
            }
        }
    }

    return result;
}

bool update_enrollment_grade(std::vector<Enrollment> &enrollments, const std::string &studentId,
                             const std::string &courseId, int examGrade, int usualGrade) {
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
