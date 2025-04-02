#ifndef DATALOADER_HPP
#define DATALOADER_HPP
#include <string>
#include <vector>
#include "core/Student.hpp"
#include "core/Course.hpp"
#include "core/Enrollment.hpp"

class DataLoader {
public:
    static std::vector<Student> loadStudents(const std::string &filePath);
    static std::vector<Course> loadCourses(const std::string &filePath);
    static std::vector<Enrollment> loadEnrollments(const std::string &filePath);
    static void saveStudents(const std::string &filePath, const std::vector<Student> &students);
    static void saveCourses(const std::string &filePath, const std::vector<Course> &courses);
    static void saveEnrollments(const std::string &filePath,
                                const std::vector<Enrollment> &enrollments);
};

#endif