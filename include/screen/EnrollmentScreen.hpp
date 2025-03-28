#ifndef ENROLLMENT_SCREEN_HPP
#define ENROLLMENT_SCREEN_HPP

#include "core/Course.hpp"
#include "core/Enrollment.hpp"
#include "core/Student.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <vector>

void ShowEnrollmentScreen(std::vector<Enrollment> &enrollments,
                          std::vector<Student> &students,
                          std::vector<Course> &courses);

#endif // ENROLLMENT_SCREEN_HPP