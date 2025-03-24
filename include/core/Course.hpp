#ifndef COURSE_HPP
#define COURSE_HPP

#include <string>

class Course {
private:
  std::string courseID;
  std::string name;
  int credit;

public:
  // 构造函数
  Course(std::string courseID, const std::string &name, int credit);

  // 获取课程信息
  std::string getId() const;
  std::string getName() const;
  int getCredit() const;
  void setName(const std::string& name);
  void setCredit(int credit);
};

#endif // COURSE_HPP