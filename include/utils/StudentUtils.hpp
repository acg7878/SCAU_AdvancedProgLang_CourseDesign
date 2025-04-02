#ifndef STUDENTUTILS_HPP
#define STUDENTUTILS_HPP

#include "core/Student.hpp"
#include <vector>

bool addStudent(std::vector<Student> &students, const Student &newStudent);
Student findStudentById(const std::vector<Student> &students, std::string studentId);
Student createStudent(const std::string &id, const std::string &name, int sex, int age,
                      int dormNumber, const std::string &phoneNumber);
bool updateStudent(std::vector<Student> &students, const std::string &studentId,
                   const std::string &newName, int newSex, int newAge, int newDormNumber,
                   const std::string &newPhoneNumber);
bool deleteStudent(std::vector<Student> &students, const std::string &studentId);
#endif // STUDENTUTILS_HPP