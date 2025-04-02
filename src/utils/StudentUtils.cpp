#include "utils/StudentUtils.hpp"
#include "core/Student.hpp"
#include "utils/DataLoader.hpp"
#include <string>
#include <vector>


// 验证id唯一性
bool isStudentIdUnique(const std::vector<Student> &students, const std::string &id) {
    for (const auto &student : students) {
        if (student.getId() == id) {
            return false;
        }
    }
    return true;
}

Student createStudent(const std::string &id, const std::string &name, int sex, int age,
                      int dormNumber, const std::string &phoneNumber) {
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

Student findStudentById(const std::vector<Student> &students, std::string studentId) {
    for (const auto &student : students) {
        if (student.getId() == studentId) {
            return student;
        }
    }
    throw std::runtime_error("未找到该学生");
}

bool updateStudent(std::vector<Student> &students, const std::string &studentId,
                   const std::string &newName, int newSex, int newAge, int newDormNumber,
                   const std::string &newPhoneNumber) {
    for (auto &student : students) {
        if (student.getId() == studentId) {
            // 更新学生信息
            student.setName(newName);
            student.setSex(newSex);
            student.setAge(newAge);
            student.setDormNumber(newDormNumber);
            student.setPhoneNumber(newPhoneNumber);

            DataLoader::saveStudents("../data/student.json", students);
            return true;
        }
    }
    return false;
}

bool deleteStudent(std::vector<Student> &students, const std::string &studentId) {
    auto it = std::find_if(students.begin(), students.end(),
                           [&](const Student &student) { return student.getId() == studentId; });

    if (it != students.end()) {
        students.erase(it);                                         // 删除找到的学生
        DataLoader::saveStudents("../data/student.json", students); // 保存更新后的学生列表
        return true;                                                // 删除成功
    }

    return false; // 未找到该学生，删除失败
}