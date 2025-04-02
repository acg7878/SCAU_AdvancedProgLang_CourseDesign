#include "utils/DataLoader.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<Student> DataLoader::loadStudents(const std::string &filePath) {
    std::vector<Student> students;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << filePath << std::endl;
        return students;
    }

    json data;
    file >> data;

    for (const auto &item : data) {
        if (item.contains("studentID") && item.contains("name") && item.contains("sex") &&
            item.contains("age") && item.contains("dorm_number") && item.contains("phone_number")) {
            students.emplace_back(item["studentID"], item["name"], item["sex"], item["age"],
                                  item["dorm_number"], item["phone_number"]);
        } else {
            std::cerr << "Missing keys in student data" << std::endl;
        }
    }

    return students;
}

std::vector<Course> DataLoader::loadCourses(const std::string &filePath) {
    std::vector<Course> courses;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << filePath << std::endl;
        return courses;
    }

    json data;
    file >> data;

    for (const auto &item : data) {
        if (item.contains("courseID") && item.contains("name") && item.contains("credit")) {
            courses.emplace_back(item["courseID"], item["name"], item["credit"]);
        } else {
            std::cerr << "Missing keys in course data" << std::endl;
        }
    }

    return courses;
}

std::vector<Enrollment> DataLoader::loadEnrollments(const std::string &filePath) {
    std::vector<Enrollment> enrollments;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << filePath << std::endl;
        return enrollments;
    }

    json data;
    file >> data;

    for (const auto &item : data) {
        if (item.contains("studentID") && item.contains("courseID") &&
            item.contains("exam_grade") && item.contains("usual_grade") &&
            item.contains("is_retake") && item.contains("composite_grade")) {
            enrollments.emplace_back(item["studentID"], item["courseID"], item["exam_grade"],
                                     item["usual_grade"], item["is_retake"],
                                     item["composite_grade"]);
        } else {
            std::cerr << "Missing keys in enrollment data" << std::endl;
        }
    }

    return enrollments;
}

void DataLoader::saveStudents(const std::string &filePath, const std::vector<Student> &students) {
    std::vector<Student> sorted_students = students;
    std::sort(
        sorted_students.begin(), sorted_students.end(), [](const Student &a, const Student &b) {
            return std::stoll(a.getId()) < std::stoll(b.getId()); // 按 studentID 的数值顺序排序
        });
    json j;
    for (const auto &student : sorted_students) {
        j.push_back({
            {"studentID",    student.getId()         },
            {"name",         student.getName()       },
            {"sex",          student.getSex()        },
            {"age",          student.getAge()        },
            {"dorm_number",  student.getDormNumber() },
            {"phone_number", student.getPhoneNumber()}
        });
    }
    std::ofstream file(filePath);
    file << j.dump(4);
}

void DataLoader::saveCourses(const std::string &filePath, const std::vector<Course> &courses) {
    // 创建一个副本并排序
    std::vector<Course> sorted_courses = courses;
    std::sort(sorted_courses.begin(), sorted_courses.end(), [](const Course &a, const Course &b) {
        return std::stoll(a.getId()) < std::stoll(b.getId()); // 按 courseID 的数值顺序排序
    });
    json j;
    for (const auto &course : sorted_courses) {
        j.push_back({
            {"courseID", course.getId()    },
            {"name",     course.getName()  },
            {"credit",   course.getCredit()}
        });
    }
    std::ofstream file(filePath);
    file << j.dump(4);
}

void DataLoader::saveEnrollments(const std::string &filePath,
                                 const std::vector<Enrollment> &enrollments) {
    std::vector<Enrollment> sorted_enrollments = enrollments;
    std::sort(
        sorted_enrollments.begin(), sorted_enrollments.end(),
        [](const Enrollment &a, const Enrollment &b) {
            int studentIdA = std::stoll(a.getStudentId());
            int studentIdB = std::stoll(b.getStudentId());
            if (studentIdA == studentIdB) {
                return std::stoll(a.getCourseId()) <
                       std::stoll(
                           b.getCourseId()); // 如果 studentID 相同，按 courseID 的数值顺序排序
            }
            return studentIdA < studentIdB; // 按 studentID 的数值顺序排序
        });
    json j;
    for (const auto &enrollment : sorted_enrollments) {
        j.push_back({
            {"studentID",       enrollment.getStudentId()     },
            {"courseID",        enrollment.getCourseId()      },
            {"exam_grade",      enrollment.getExamGrade()     },
            {"usual_grade",     enrollment.getUsualGrade()    },
            {"is_retake",       enrollment.isRetake()         },
            {"composite_grade", enrollment.getCompositeGrade()}
        });
    }
    std::ofstream file(filePath);
    file << j.dump(4);
}