#ifndef STUDENTUTILS_HPP
#define STUDENTUTILS_HPP

#include "core/Student.hpp"
#include <vector>

bool addStudent(std::vector<Student> &students, const Student &newStudent);
Student findStudentById(const std::vector<Student> &students,
                        std::string studentId);
std::string displayStudentInfo(const Student &student);
Student createStudent(const std::string &id, const std::string &name, int sex,
                      int age, int dormNumber, const std::string &phoneNumber);
#endif // STUDENTUTILS_HPP