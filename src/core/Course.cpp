#include "core/Course.hpp"

// 构造函数
Course::Course(int id, const std::string &name, int credit)
    : id(id), name(name),  credit(credit) {}

// 获取课程信息
int Course::getId() const { return id; }
std::string Course::getName() const { return name; }
int Course::getCredit() const { return credit; }
