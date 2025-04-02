#include "core/Enrollment.hpp"
#include "screen/CourseScreen.hpp"
#include "screen/StudentScreen.hpp"
#include "screen/EnrollmentScreen.hpp"
#include "utils/DataLoader.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

void LoadData(std::vector<Student> &students, std::vector<Course> &courses,
              std::vector<Enrollment> &enrollments) {
    students = DataLoader::loadStudents("../data/student.json");
    courses = DataLoader::loadCourses("../data/course.json");
    enrollments = DataLoader::loadEnrollments("../data/enrollment.json");
}

// 菜单函数
void menu(std::vector<Student> students, std::vector<Course> courses,
          std::vector<Enrollment> enrollments) {
    auto screen = ScreenInteractive::TerminalOutput();

    // 菜单选项
    int selected = 0;
    std::vector<std::string> entries = {"学生信息管理", "选课信息管理", "课程信息管理"};

    // "进入" 按钮
    auto enter = Button("进入", [&] {
        switch (selected) {
            case 0:
                // 进入学生信息管理界面
                ShowStudentScreen(students);
                break;
            case 1:
                // 进入选课信息管理界面
                ShowEnrollmentScreen(enrollments, students, courses);
                break;
            case 2:
                // 进入课程信息管理界面
                ShowCourseScreen(courses);
                break;
        }
    });

    auto quit = Button("退出", screen.ExitLoopClosure());
    auto menu = Menu(&entries, &selected);

    // 容器组件
    auto comp = Container::Horizontal({
        menu,
        enter,
        quit,
    });

    // 渲染器
    auto renderer = Renderer(comp, [&] {
        return vbox({
                   text("教务信息管理系统") | bold | center,
                   separator(),
                   menu->Render() | center,
                   hbox(enter->Render(), separatorEmpty(), quit->Render()) | center,
               }) |
               border | size(WIDTH, EQUAL, 50) | center;
    });

    // 启动界面
    screen.Loop(renderer);
}

int main() {
    std::vector<Student> students;
    std::vector<Course> courses;
    std::vector<Enrollment> enrollments;
    LoadData(students, courses, enrollments);
    menu(students, courses, enrollments);
    return 0;
}
