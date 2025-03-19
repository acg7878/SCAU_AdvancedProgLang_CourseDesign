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
    if (item.contains("studentID") && item.contains("name") &&
        item.contains("sex") && item.contains("age") &&
        item.contains("dorm_number") && item.contains("phone_number") &&
        item.contains("retake_inf")) {
      students.emplace_back(
          item["studentID"], item["name"], item["sex"], item["age"],
          item["dorm_number"], item["phone_number"],
          item["retake_inf"].get<std::vector<std::pair<int, int>>>());
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
    if (item.contains("courseID") && item.contains("name") &&
        item.contains("credit")) {
      courses.emplace_back(item["courseID"], item["name"], item["credit"]);
    } else {
      std::cerr << "Missing keys in course data" << std::endl;
    }
  }

  return courses;
}

std::vector<Enrollment>
DataLoader::loadEnrollments(const std::string &filePath) {
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
      enrollments.emplace_back(item["studentID"], item["courseID"],
                               item["exam_grade"], item["usual_grade"],
                               item["is_retake"], item["composite_grade"]);
    } else {
      std::cerr << "Missing keys in enrollment data" << std::endl;
    }
  }
  //   {
  //     "studentId": "1",
  //     "courseId": "1",
  //     "exam_grade": 85,
  //     "usual_grade": 90,
  //     "composite_grade": 87,
  //     "is_retake": false
  // },
  return enrollments;
}