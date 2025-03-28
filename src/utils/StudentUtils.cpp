#include "utils/StudentUtils.hpp"
#include "core/Student.hpp"
#include "utils/DataLoader.hpp"
#include <string>
#include <vector>


// 验证id唯一性
bool isStudentIdUnique(const std::vector<Student> &students,
                       const std::string &id) {
  for (const auto &student : students) {
    if (student.getId() == id) {
      return false;
    }
  }
  return true;
}

Student createStudent(const std::string &id, const std::string &name, int sex, int age, int dormNumber, const std::string &phoneNumber) {
  return Student(id, name, sex, age, dormNumber, phoneNumber);
}

bool addStudent(std::vector<Student> &students, const Student &newStudent) {
    // 检查学生ID是否唯一
    if (!isStudentIdUnique(students, newStudent.getId())) {
        return false; // 学生ID已存在，添加失败
    }
    students.push_back(newStudent);
    DataLoader::saveStudents("../data/student.json", students);
    return true; // 添加成功
}

Student findStudentById(const std::vector<Student> &students,
                        std::string studentId) {
  for (const auto &student : students) {
    if (student.getId() == studentId) {
      return student;
    }
  }
  throw std::runtime_error("未找到该学生");
}

// 没什么用
// std::string displayStudentInfo(const Student &student) {
//     std::ostringstream oss;
//     oss << "学生ID: " << student.getId() << "\n";
//     oss << "姓名: " << student.getName() << "\n";
//     oss << "性别: " << (student.getSex() == 0 ? "男" : "女") << "\n";
//     oss << "年龄: " << student.getAge() << "\n";
//     oss << "宿舍号: " << student.getDormNumber() << "\n";
//     oss << "电话号码: " << student.getPhoneNumber() << "\n";
//     return oss.str();
// }

