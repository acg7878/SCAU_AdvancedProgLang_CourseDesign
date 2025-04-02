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
    void setName(const std::string &name);
    void setCredit(int credit);

    bool operator==(const Course &other) const {
        return courseID == other.courseID; // 只比较课程ID是否相等
    }
};

namespace std {
    template <>
    struct hash<Course> {
        std::size_t operator()(const Course &course) const {
            return std::hash<std::string>()(course.getId()); // 使用课程ID的哈希值
        }
    };
} // namespace std

#endif // COURSE_HPP