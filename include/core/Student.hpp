#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
private:
    std::string studentID;
    std::string name;
    int sex; // 男：0，女：1
    int age;
    int dorm_number;
    std::string phone_number;
    bool isRetake = false;

public:
    Student(const std::string id, const std::string &name, int sex, int age, int dorm_number,
            std::string phone_number);

    // 获取学生信息
    std::string getId() const;
    std::string getName() const;
    int getSex() const;
    int getAge() const;
    int getDormNumber() const;
    std::string getPhoneNumber() const;

    // 设置学生信息
    void setName(const std::string &newName);
    void setSex(int newSex);
    void setAge(int newAge);
    void setDormNumber(int newDormNumber);
    void setPhoneNumber(const std::string &newPhoneNumber);
};

#endif // STUDENT_H