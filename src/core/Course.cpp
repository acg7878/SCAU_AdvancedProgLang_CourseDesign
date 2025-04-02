#include "core/Course.hpp"
#include <string>

// 构造函数
Course::Course(std::string courseID, const std::string &name, int credit)
    : courseID(courseID), name(name), credit(credit) {
}

// 获取课程信息
std::string Course::getId() const {
    return courseID;
}
std::string Course::getName() const {
    return name;
}
void Course::setName(const std::string &name) {
    this->name = name;
}
void Course::setCredit(int credit) {
    this->credit = credit;
}

int Course::getCredit() const {
    return credit;
}
