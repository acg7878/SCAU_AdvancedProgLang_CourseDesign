#include "utils/StudentUtils.hpp"
#include "core/Student.hpp"
#include "utils/DataLoader.hpp"
#include <iostream>
#include <string>
#include <vector>

Student inputStudentInfo() {
  int sex, age, dormNumber, phoneNumber;
  std::string id, name;

  std::cout << "请输入学生ID: ";
  std::cin >> id;

  std::cout << "请输入姓名: ";
  std::cin >> name;

  std::cout << "请输入性别 (0: 男, 1: 女): ";
  std::cin >> sex;

  std::cout << "请输入年龄: ";
  std::cin >> age;

  std::cout << "请输入宿舍号: ";
  std::cin >> dormNumber;

  std::cout << "请输入电话号码: ";
  std::cin >> phoneNumber;

  return Student(id, name, sex, age, dormNumber, phoneNumber);
  // std::cout << "是否有重修记录？(1为需要录入重修信息，0为未曾重修)" <<
  // std::endl; std::cout << "请输入："; if(isRetake == 0) return Student(id,
  // name, sex, age, dormNumber, phoneNumber); else if(isRetake == 1){
  //     std::cout << "请输入重修记录数量：";
  //     int num;
  //     std::cin >> num;
  //     std::vector<std::pair<int, int>> retake_inf(num);
  //     for(int i = 0;i < num; i++) {
  //         std::cout << "请输入第" << i << "条记录（学期 ）"
  //     }

  // }
}

void addStudent(std::vector<Student> &students) {
  Student newStudent = inputStudentInfo();
  students.push_back(newStudent);
  DataLoader::saveStudents("../data/student.json", students);
  std::cout << "学生信息录入成功！" << std::endl;
}

Student findStudentById(const std::vector<Student> &students,
                        std::string studentId) {
  for (const auto &student : students) {
    if (student.getId() == studentId) {
      return student;
    }
  }
  throw std::runtime_error("Student not found");
}

void displayStudentInfo(const Student &student) {
  std::cout << "学生ID: " << student.getId() << std::endl;
  std::cout << "姓名: " << student.getName() << std::endl;
  std::cout << "性别: " << (student.getSex() == 0 ? "男" : "女") << std::endl;
  std::cout << "年龄: " << student.getAge() << std::endl;
  std::cout << "宿舍号: " << student.getDormNumber() << std::endl;
  std::cout << "电话号码: " << student.getPhoneNumber() << std::endl;
}

void StudentInfo(std::vector<Student> &students) {
  int choice = -1;
  do {
    std::vector<std::string> options = {"返回上一级", "查询学生信息",
                                        "录入学生信息"};
    std::cout << "\n学生信息菜单：" << std::endl;
    for (size_t i = 0; i < options.size(); ++i) {
      std::cout << i << ". " << options[i] << std::endl;
    }
    std::cout << "请输入您的选择：";
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "无效输入，请输入数字。\n";
      continue;
    }

    switch (choice) {
    case 0:
      std::cout << "返回上一级..." << std::endl;
      break;
    case 1: {
      std::cout << "请输入学生ID：";
      std::string studentID;
      std::cin >> studentID;
      displayStudentInfo(findStudentById(students, studentID));
      break;
    }
    case 2:
      addStudent(students);
      break;
    default:
      std::cout << "无效选择，请选择有效的选项。" << std::endl;
    }
  } while (choice != 0);
}