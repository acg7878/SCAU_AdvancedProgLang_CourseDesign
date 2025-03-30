#ifndef COURSE_SCREEN_HPP
#define COURSE_SCREEN_HPP

#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include "core/Course.hpp"

void PrintCourseInfo(std::vector<Course> &courses);
void ShowCourseScreen(std::vector<Course> &courses);
void AddCourseScreen(std::vector<Course> &courses);
#endif // COURSE_SCREEN_HPP