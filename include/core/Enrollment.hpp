#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <string>
//  #include "Student.hpp"
//  #include "Course.hpp"

class Enrollment {
public:
    Enrollment(const std::string &studentId, const std::string &courseId, int exam_grade,
               int usual_grade, bool is_retake = false, int composite_grade = -1);
    // 默认值需要放最后

    std::string getStudentId() const;
    std::string getCourseId() const;
    int getExamGrade() const;
    int getUsualGrade() const;
    int getCompositeGrade() const;
    bool isRetake() const;

    void setExamGrade(int grade);
    void setUsualGrade(int grade);
    void setCompositeGrade(int grade);
    void set_is_retake(bool flag);

private:
    std::string studentId;
    std::string courseId;
    int exam_grade;
    int usual_grade;
    int composite_grade;
    bool is_retake;
};

#endif // ENROLLMENT_H
