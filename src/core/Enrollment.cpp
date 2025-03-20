#include "core/Enrollment.hpp"

Enrollment::Enrollment(const std::string &studentId,
                       const std::string &courseId, int exam_grade,
                       int usual_grade, bool is_retake, int composite_grade)
    : studentId(studentId), courseId(courseId), exam_grade(exam_grade),
      usual_grade(usual_grade), composite_grade(composite_grade),
      is_retake(is_retake) {}

std::string Enrollment::getStudentId() const { return studentId; }
std::string Enrollment::getCourseId() const { return courseId; }
int Enrollment::getExamGrade() const { return exam_grade; }
int Enrollment::getUsualGrade() const { return usual_grade; }
int Enrollment::getCompositeGrade() const { return composite_grade; }
bool Enrollment::isRetake() const { return is_retake; }