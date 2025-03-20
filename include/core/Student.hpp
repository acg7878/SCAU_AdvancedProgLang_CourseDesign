#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

class Student {
private:
  std::string studentID;
  std::string name;
  int sex; // 男：0，女：1
  int age;
  int dorm_number;
  std::string phone_number;
  // std::vector<std::pair<int, int>> retake_inf; // 学期，成绩
  bool isRetake = false;

public:
  // 构造函数
  // Student(int id, const std::string &name, int sex, int age, int dorm_number,
  //         int phone_number, const std::vector<std::pair<int, int>> &retake_inf);
  Student(std::string id, const std::string &name, int sex, int age, int dorm_number,
          std::string phone_number);
  // 获取学生信息
  std::string getId() const;
  std::string getName() const;
  int getSex() const;
  int getAge() const;
  int getDormNumber() const;
  std::string getPhoneNumber() const;
  std::vector<std::pair<int, int>> getRetakeInfo() const;
};

#endif // STUDENT_H