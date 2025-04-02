#include "core/Student.hpp"
#include <string>

// 构造函数
Student::Student(const std::string id, const std::string &name, int sex, int age, int dorm_number,
                 std::string phone_number)
    : studentID(id), name(name), sex(sex), age(age), dorm_number(dorm_number),
      phone_number(phone_number) {
}

// 获取学生信息
std::string Student::getId() const {
    return studentID;
}
std::string Student::getName() const {
    return name;
}
int Student::getSex() const {
    return sex;
}
int Student::getAge() const {
    return age;
}
int Student::getDormNumber() const {
    return dorm_number;
}
std::string Student::getPhoneNumber() const {
    return phone_number;
}

// 设置学生信息
void Student::setName(const std::string &newName) {
    name = newName;
}
void Student::setSex(int newSex) {
    sex = newSex;
}
void Student::setAge(int newAge) {
    age = newAge;
}
void Student::setDormNumber(int newDormNumber) {
    dorm_number = newDormNumber;
}
void Student::setPhoneNumber(const std::string &newPhoneNumber) {
    phone_number = newPhoneNumber;
}
