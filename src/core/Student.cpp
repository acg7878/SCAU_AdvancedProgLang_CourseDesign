#include "core/Student.hpp"
#include <string>

// 构造函数
// Student::Student(int id, const std::string &name, int sex, int age,
//                  int dorm_number, int phone_number,
//                  const std::vector<std::pair<int, int>> &retake_inf)
//     : studentID(id), name(name), sex(sex), age(age), dorm_number(dorm_number),
//       phone_number(phone_number), retake_inf(retake_inf) {}

Student::Student(std::string id, const std::string &name, int sex, int age,
                 int dorm_number, std::string phone_number)
    : studentID(id), name(name), sex(sex), age(age), dorm_number(dorm_number),
      phone_number(phone_number) {}
// 获取学生信息
std::string Student::getId() const { return studentID; }
std::string Student::getName() const { return name; }
int Student::getSex() const { return sex; }
int Student::getAge() const { return age; }
int Student::getDormNumber() const { return dorm_number; }
std::string Student::getPhoneNumber() const { return phone_number; }
// std::vector<std::pair<int, int>> Student::getRetakeInfo() const {
//   return retake_inf;
// }