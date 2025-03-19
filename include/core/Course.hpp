#ifndef COURSE_HPP
#define COURSE_HPP

#include <string>

class Course {
private:
  int id;
  std::string name;
  int credit;

public:
  // 构造函数
  Course(int id, const std::string &name, int credit);

  // 获取课程信息
  int getId() const;
  std::string getName() const;
  int getCredit() const;
  bool getIsRetake() const;
};

#endif // COURSE_HPP