#ifndef DATALOADER_HPP
#define DATALOADER_HPP
#include <string>
#include <vector>
#include "core/Student.hpp"
#include "core/Course.hpp"
#include "core/Enrollment.hpp"

class DataLoader {
public:
    static std::vector<Student> loadStudents(const std::string& filePath);
    static std::vector<Course> loadCourses(const std::string& filePath);
    static std::vector<Enrollment> loadEnrollments(const std::string& filePath);
};

#endif