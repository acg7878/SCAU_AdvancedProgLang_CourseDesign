#include "utils/StudentUtils.hpp"
#include "core/Student.hpp"
#include "utils/DataLoader.hpp"
#include <iomanip>
#include <iostream>
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

Student inputStudentInfo(const std::vector<Student> &students) {
  int sex, age, dormNumber;
  std::string id, name, phoneNumber;

  do {
    std::cout << "请输入学生ID: ";
    std::cin >> id;
    if (!isStudentIdUnique(students, id)) {
      std::cout << "学生ID已存在，请输入一个唯一的ID。\n";
    }
  } while (!isStudentIdUnique(students, id));

  std::cout << "请输入姓名: ";
  std::cin >> name;

  do {
    std::cout << "请输入性别 (0: 男, 1: 女): ";
    if (!(std::cin >> sex)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "无效输入，请输入数字。\n";
      continue;
    }
    if (sex != 0 && sex != 1) {
      std::cout << "输入无效，请输入 0 或 1。\n";
    }
  } while (sex != 0 && sex != 1);

  std::cout << "请输入年龄: ";
  std::cin >> age;

  std::cout << "请输入宿舍号: ";
  std::cin >> dormNumber;

  do {
    std::cout << "请输入电话号码 (11位): ";
    std::cin >> phoneNumber;
    if (phoneNumber.length() != 11) {
      std::cout << "电话号码必须为11位，请重新输入。\n";
    }
  } while (phoneNumber.length() != 11);

  return Student(id, name, sex, age, dormNumber, phoneNumber);
}

void addStudent(std::vector<Student> &students) {
  Student newStudent = inputStudentInfo(students);
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
  throw std::runtime_error("未找到该学生");
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
    std::cout << "\n+------------------------------+" << std::endl;
    std::cout << "|         学生信息菜单         |" << std::endl;
    std::cout << "+------------------------------+" << std::endl;
    for (size_t i = 0; i < options.size(); ++i) {
      std::cout << "| " << std::setw(2) << i << ". " << std::left
                << std::setw(30) << options[i] << " |" << std::endl;
    }
    std::cout << "+------------------------------+" << std::endl;
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
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           查询学生信息              |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      std::cout << "请输入学生ID：";
      std::string studentID;
      std::cin >> studentID;
      try {
        displayStudentInfo(findStudentById(students, studentID));
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
      break;
    }
    case 2:
      std::cout << "\n+------------------------------------+" << std::endl;
      std::cout << "|           录入学生信息              |" << std::endl;
      std::cout << "+------------------------------------+" << std::endl;
      addStudent(students);
      break;
    default:
      std::cout << "无效选择，请选择有效的选项。" << std::endl;
    }
  } while (choice != 0);
}