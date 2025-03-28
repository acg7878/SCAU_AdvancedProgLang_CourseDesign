#include "core/Student.hpp"
#include <string>

Student::Student(const std::string id, const std::string &name, int sex, int age,
                 int dorm_number, std::string phone_number)
    : studentID(id), name(name), sex(sex), age(age), dorm_number(dorm_number),
      phone_number(phone_number) {}

std::string Student::getId() const { return studentID; }
std::string Student::getName() const { return name; }
int Student::getSex() const { return sex; }
int Student::getAge() const { return age; }
int Student::getDormNumber() const { return dorm_number; }
std::string Student::getPhoneNumber() const { return phone_number; }
