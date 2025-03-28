#ifndef ENROLLMENT_SCREEN_HPP
#define ENROLLMENT_SCREEN_HPP

#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include "core/Enrollment.hpp"
#include "core/Student.hpp"
#include "core/Course.hpp"

void ShowEnrollmentScreen(ftxui::ScreenInteractive& screen,
                          std::vector<Enrollment>& enrollments,
                          std::vector<Student>& students,
                          std::vector<Course>& courses);

#endif // ENROLLMENT_SCREEN_HPP