#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <string>
//  #include "Student.hpp"
//  #include "Course.hpp"

class Enrollment {
public:
  Enrollment(const std::string &studentId, const std::string &courseId,
             int exam_grade, int usual_grade, bool is_retake,
             int composite_grade = -1);
  // 默认值需要放最后
  std::string getStudentId() const;
  std::string getCourseId() const;

private:
  std::string studentId;
  std::string courseId;
  int exam_grade;
  int usual_grade;
  int composite_grade;
  bool is_retake;
};

#endif // ENROLLMENT_H
