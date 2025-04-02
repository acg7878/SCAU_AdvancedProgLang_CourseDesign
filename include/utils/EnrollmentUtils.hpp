#ifndef ENROLLMENT_UTILS_HPP
#define ENROLLMENT_UTILS_HPP

#include "core/Course.hpp"
#include "core/Enrollment.hpp"
#include "core/Student.hpp"
#include <string>
#include <vector>

// 根据课程ID获取学生列表
std::vector<Student> get_students_by_course_id(const std::vector<Enrollment> &enrollments,
                                               const std::vector<Student> &students,
                                               const std::string &courseId);

// 增加选课记录
bool add_enrollment(std::vector<Enrollment> &enrollments, const std::string &studentId,
                    const std::string &courseId);

// 按课程ID统计不及格学生人数
int count_failing_students_by_course_id(const std::vector<Enrollment> &enrollments,
                                        const std::string &courseId);

// 按学生ID获取不及格课程列表
std::vector<std::tuple<std::string, std::string, int>> get_failing_courses_by_student_id(
    const std::vector<Enrollment> &enrollments, const std::vector<Student> &students,
    const std::vector<Course> &courses, const std::string &studentID);

// 修改选课记录的成绩
bool update_enrollment_grade(std::vector<Enrollment> &enrollments, const std::string &studentId,
                             const std::string &courseId, int examGrade, int usualGrade);

std::vector<std::tuple<std::string, std::string, std::string, int>>
    get_failing_students(const std::vector<Enrollment> &enrollments,
                         const std::vector<Student> &students, const std::vector<Course> &courses);
#endif // ENROLLMENT_UTILS_HPP