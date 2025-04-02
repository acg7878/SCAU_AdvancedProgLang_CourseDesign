#ifndef COURSEUTILS_HPP
#define COURSEUTILS_HPP

#include "core/Course.hpp"
#include <unordered_set>
#include <vector>

Course findCourseById(const std::vector<Course> &courses, const std::string &courseId);

bool add_course(std::vector<Course> &courses, const std::string &courseId,
                const std::string &courseName, int credit);

std::string modify_course(std::vector<Course> &courses, const std::string &courseId,
                          const std::string &newName, int newCredit);

std::vector<std::vector<std::string>> search_course(const std::vector<Course> &courses,
                                                    const std::vector<std::string> &keywords);

std::string modify_course(std::vector<Course> &courses, const std::string &courseId,
                          const std::string &newName, int newCredit);

bool delete_course(std::vector<Course> &courses, const std::string &courseId);

std::unordered_set<Course> search_course(const std::vector<Course> &courses,
                                         const std::string &keywords);
#endif // COURSEUTILS_HPP