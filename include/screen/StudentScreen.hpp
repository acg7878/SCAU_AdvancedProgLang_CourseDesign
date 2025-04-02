#ifndef STUDENT_SCREEN_HPP
#define STUDENT_SCREEN_HPP

#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include "core/Student.hpp"

void searchStudentInfo(std::vector<Student> &students);
void ShowStudentScreen(std::vector<Student> &students);
void AddStudentScreen(std::vector<Student> &students);
#endif // STUDENT_SCREEN_HPP