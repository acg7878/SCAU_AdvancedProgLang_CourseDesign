#ifndef ENROLLMENTUTILS_HPP
#define ENROLLMENTUTILS_HPP

#include "core/Course.hpp"
#include "core/Enrollment.hpp"
#include "core/Student.hpp"
#include <vector>

// 获得课程的选课名单
void get_enrollment_list(const std::vector<Enrollment> &enrollments,
                         const std::vector<Student> &students,
                         const std::vector<Course> &courses);

// 计算所获学分
void cal_credits(const std::vector<Enrollment> &enrollments,
                 const std::vector<Student> &students,
                 const std::vector<Course> &courses);

// 统计重修课程
void cal_retake_course(const std::vector<Enrollment> &enrollments,
                       const std::vector<Student> &students,
                       const std::vector<Course> &courses);

void add_enrollment(std::vector<Enrollment> &enrollments);
void modify_student_grade(std::vector<Enrollment> &enrollments);

// 主界面
void EnrollmentInfo(std::vector<Enrollment> &enrollments,
                    const std::vector<Student> &students,
                    const std::vector<Course> &courses);
#endif // ENROLLMENTUTILS_HPP