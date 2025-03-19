#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

class Student {
private:
  int studentID;
  std::string name;
  int sex; // 男：0，女：1
  int age;
  int dorm_number;
  int phone_number;
  std::vector<std::pair<int, int>> retake_inf; // 学期，成绩

public:
  // 构造函数
  Student(int id, const std::string &name, int sex, int age, int dorm_number,
          int phone_number,const std::vector<std::pair<int, int>>& retake_inf);

  // 获取学生信息
  int getId() const;
  std::string getName() const;
  int getSex() const;
  int getAge() const;
  int getDormNumber() const;
  int getPhoneNumber() const;
  std::vector<std::pair<int, int>> getRetakeInfo() const;
};

#endif // STUDENT_H