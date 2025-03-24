#ifndef COURSEUTILS_HPP
#define COURSEUTILS_HPP

#include "core/Course.hpp"
#include <vector>


void get_course_info(const std::vector<Course> &courses);
void add_course(std::vector<Course> &courses);
void modify_course(std::vector<Course> &courses);
void search_course(const std::vector<Course> &courses);
void CourseInfo(std::vector<Course> &courses);
#endif // COURSEUTILS_HPP