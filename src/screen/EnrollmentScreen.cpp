#include "screen/EnrollmentScreen.hpp"
#include "core/Course.hpp"
#include "core/Student.hpp"
#include "ftxui/dom/elements.hpp"
#include "utils/DataLoader.hpp"
#include "utils/EnrollmentUtils.hpp"
#include <exception>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <vector>
using namespace ftxui;

void AddEnrollmentScreen(std::vector<Enrollment> &enrollments) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentId, courseId;
    std::string message;

    // 输入框组件
    auto student_input = Input(&studentId, "学生ID");
    auto course_input = Input(&courseId, "课程ID");

    // 按钮组件
    auto add_button = Button("添加", [&] {
        if (studentId.empty() || courseId.empty()) {
            message = "学生ID和课程ID不能为空！";
            return;
        }
        bool success = add_enrollment(enrollments, studentId, courseId);
        message = success ? "选课信息添加成功！" : "该学生已选修该课程！";
    });

    auto back_button = Button("返回", screen.ExitLoopClosure());

    // 容器组件
    auto container = Container::Vertical({
        student_input,
        course_input,
        add_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("添加选课信息") | bold | center,
                   separator(),
                   student_input->Render() | center,
                   course_input->Render() | center,
                   separator(),
                   hbox(add_button->Render() | center, back_button->Render() | center) | center,
                   separator(),
                   text(message) | color(Color::Red) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}

void ModifyEnrollmentScreen(std::vector<Enrollment> &enrollments) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentId, courseId, examGradeStr, usualGradeStr;
    std::string message;

    // 输入框组件
    auto student_input = Input(&studentId, "学生ID");
    auto course_input = Input(&courseId, "课程ID");
    auto exam_input = Input(&examGradeStr, "考试成绩");
    auto usual_input = Input(&usualGradeStr, "平时成绩");

    // 按钮组件
    auto modify_button = Button("修改", [&] {
        try {
            int examGrade = std::stoi(examGradeStr);
            int usualGrade = std::stoi(usualGradeStr);
            bool success =
                update_enrollment_grade(enrollments, studentId, courseId, examGrade, usualGrade);
            message = success ? "选课信息修改成功！" : "未找到对应的选课记录！";
        } catch (...) {
            message = "成绩输入无效，请输入数字！";
        }
    });

    auto back_button = Button("返回", screen.ExitLoopClosure());

    // 容器组件
    auto container = Container::Vertical({
        student_input,
        course_input,
        exam_input,
        usual_input,
        modify_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("修改选课信息") | bold | center,
                   separator(),
                   student_input->Render() | center,
                   course_input->Render() | center,
                   exam_input->Render() | center,
                   usual_input->Render() | center,
                   separator(),
                   hbox(modify_button->Render() | center, back_button->Render() | center) | center,
                   separator(),
                   text(message) | color(Color::Red) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}

void DeleteEnrollmentScreen(std::vector<Enrollment> &enrollments) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentId, courseId;
    std::string message;

    // 输入框组件
    auto student_input = Input(&studentId, "学生ID");
    auto course_input = Input(&courseId, "课程ID");

    // 按钮组件
    auto delete_button = Button("删除", [&] {
        auto it = std::remove_if(enrollments.begin(), enrollments.end(), [&](const Enrollment &e) {
            return e.getStudentId() == studentId && e.getCourseId() == courseId;
        });
        if (it != enrollments.end()) {
            enrollments.erase(it, enrollments.end());
            DataLoader::saveEnrollments("../data/enrollment.json", enrollments);
            message = "选课信息删除成功！";
        } else {
            message = "未找到对应的选课记录！";
        }
    });

    auto back_button = Button("返回", screen.ExitLoopClosure());

    // 容器组件
    auto container = Container::Vertical({
        student_input,
        course_input,
        delete_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("删除选课信息") | bold | center,
                   separator(),
                   student_input->Render() | center,
                   course_input->Render() | center,
                   hbox(delete_button->Render() | center, back_button->Render() | center) | center,
                   separator(),
                   text(message) | color(Color::Red) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}

void QueryFailingListScreen(const std::vector<Enrollment> &enrollments,
                            const std::vector<Student> &students,
                            const std::vector<Course> &courses) {
    std::string message = "", test = "";
    int selected = 1;
    int page = 0, max_all = 0;
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentID;
    bool isFound = true;
    Elements el_by_id[4];
    auto choose_all = Button("全部不及格信息", [&] {
        selected = 1;
        page = 0;
    });
    auto choose_by_id = Button("按ID查询", [&] {
        selected = 2;
        page = 0;
        // 清空按ID查询的元素
        for (auto &el : el_by_id) {
            el.clear();
        }
        isFound = true;
    });
    auto quit = Button("退出", screen.ExitLoopClosure());
    auto page_last = Button("上一页", [&] {
        if (page > 0)
            page--;
    });
    auto page_next = Button("下一页", [&] {
        if (page < max_all)
            page++;
    });
    auto input_box = Input(&studentID, "请输入学生ID") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);
    auto submit_button =
        Button("查询",
               [&] {
                   try {
                       auto failing_courses = get_failing_courses_by_student_id(
                           enrollments, students, courses, studentID);
                       for (auto &el : el_by_id) {
                           el.clear();
                       }
                       for (const auto &[studentName, courseName, grade] : failing_courses) {
                           el_by_id[0].push_back(text(studentName) | center);
                           el_by_id[1].push_back(text(courseName) | center);
                           el_by_id[2].push_back(text(std::to_string(grade)) | center);
                       }
                       isFound = !failing_courses.empty();
                   } catch (const std::exception &e) {
                       for (auto &el : el_by_id)
                           el.clear();
                       isFound = false;
                   }
               }) |
        size(ftxui::HEIGHT, ftxui::EQUAL, 3);

    auto comp = Container::Horizontal({
        choose_all,
        choose_by_id,
        quit,
        page_last,
        page_next,
        input_box,
        submit_button,
    });
    // 获取全部数据
    auto fail_all = get_failing_students(enrollments, students, courses);
    auto renderer = Renderer(comp, [&] {
        Elements el_all[4];
        if (selected == 1) {
            int now = 0;
            for (const auto &[student_id, studentName, courseName, grade] : fail_all) {
                if (now / 8 == page) {
                    el_all[0].push_back(text(student_id) | center);
                    el_all[1].push_back(text(studentName) | center);
                    el_all[2].push_back(text(courseName) | center);
                    el_all[3].push_back(text(std::to_string(grade)) | center);
                }
            }
            now++;
            max_all = (now - 1) / 8;
            test = "第 " + std::to_string(page + 1) + " 页 / 共 " + std::to_string(max_all + 1) +
                   " 页";
        }

        if (selected == 1) {
            return vbox({
                       hbox(choose_all->Render() | flex, choose_by_id->Render() | flex),
                       text("全部不及格信息") | bold | center,
                       separator(),
                       hbox({
                           vbox(text("学生ID") | center, el_all[0]) | flex,
                           separator(),
                           vbox(text("姓名") | center, el_all[1]) | flex,
                           separator(),
                           vbox(text("课程") | center, el_all[2]) | flex,
                           separator(),
                           vbox(text("成绩") | center, el_all[3]) | flex,
                       }),
                       separator(),
                       hbox(page_last->Render(), separatorEmpty(), quit->Render(), separatorEmpty(),
                            page_next->Render()) |
                           center,
                       text(test) | align_right,
                   }) |
                   border | size(WIDTH, EQUAL, 80) | center;
        } else if (selected == 2) {
            Elements content;
            content.push_back(hbox(choose_all->Render() | flex, choose_by_id->Render() | flex));
            content.push_back(text("按学生ID查询不及格信息") | bold | center);
            content.push_back(separator());
            content.push_back(hbox({
                                  text("输入学生ID: "),
                                  input_box->Render(),
                                  submit_button->Render(),
                              }) |
                              center);
            content.push_back(separator());
            content.push_back(hbox({
                vbox(text("姓名") | center, el_by_id[0]) | flex,
                separator(),
                vbox(text("课程") | center, el_by_id[1]) | flex,
                separator(),
                vbox(text("成绩") | center, el_by_id[2]) | flex,
            }));
            content.push_back(separator());
            content.push_back(quit->Render() | center);
            if (!isFound && selected == 2) {
                content.push_back(text("未查询到学生信息") | color(Color::Red) | center);
            }
            return vbox(content) | border | size(WIDTH, EQUAL, 80) | center;
        }
        return text("未知选项") | center;
    });
    screen.Loop(renderer);
}

void searchEnrollmentInfo(const std::vector<Enrollment> &enrollments,
                          const std::vector<Student> &students,
                          const std::vector<Course> &courses) {
    std::string test = "";
    int selected = 1;
    int page = 0, max_all = 0;
    auto screen = ScreenInteractive::TerminalOutput();
    std::string courseID;
    Elements el_by_id[4];
    bool enrollmentFound = true;

    // 按钮组件
    auto choose_all = Button("全部选课信息", [&] {
        selected = 1;
        page = 0;
    });
    auto choose_by_id = Button("按课程ID查询选课信息", [&] {
        selected = 2;
        page = 0;
        for (auto &el : el_by_id) {
            el.clear();
        }
        enrollmentFound = true;
    });
    auto quit = Button("退出", screen.ExitLoopClosure());
    auto page_last = Button("上一页", [&] {
        if (page > 0)
            page--;
    });
    auto page_next = Button("下一页", [&] {
        if (page < max_all)
            page++;
    });

    // 输入框和查询按钮
    auto input_box = Input(&courseID, "请输入课程ID") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);
    auto submit_button = Button("查询", [&] {
        try {
            // 根据课程ID查询选课信息
            for (auto &el : el_by_id) {
                el.clear();
            }
            for (const auto &enrollment : enrollments) {
                if (enrollment.getCourseId() == courseID) {
                    auto student =
                        std::find_if(students.begin(), students.end(), [&](const Student &s) {
                            return s.getId() == enrollment.getStudentId();
                        });
                    auto course =
                        std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
                            return c.getId() == enrollment.getCourseId();
                        });
                    if (student != students.end() && course != courses.end()) {
                        el_by_id[0].push_back(text(student->getId()) | center);
                        el_by_id[1].push_back(text(student->getName()) | center);
                        el_by_id[2].push_back(text(course->getName()) | center);
                        el_by_id[3].push_back(text(std::to_string(enrollment.getCompositeGrade())) |
                                              center);
                    }
                }
            }
            enrollmentFound = !el_by_id[0].empty();
        } catch (const std::exception &e) {
            for (auto &el : el_by_id) {
                el.clear();
            }
            enrollmentFound = false;
        }
    });

    // 容器组件
    auto comp = Container::Horizontal({
        choose_all,
        choose_by_id,
        quit,
        page_last,
        page_next,
        input_box,
        submit_button,
    });

    // 渲染器
    auto renderer = Renderer(comp, [&] {
        Elements el_all[4];

        // 全部选课信息
        if (selected == 1) {
            int now = 0;
            for (const auto &enrollment : enrollments) {
                if (now / 8 == page) {
                    auto student =
                        std::find_if(students.begin(), students.end(), [&](const Student &s) {
                            return s.getId() == enrollment.getStudentId();
                        });
                    auto course =
                        std::find_if(courses.begin(), courses.end(), [&](const Course &c) {
                            return c.getId() == enrollment.getCourseId();
                        });
                    if (student != students.end() && course != courses.end()) {
                        el_all[0].push_back(text(student->getId()) | center);
                        el_all[1].push_back(text(student->getName()) | center);
                        el_all[2].push_back(text(course->getName()) | center);
                        el_all[3].push_back(text(std::to_string(enrollment.getCompositeGrade())) |
                                            center);
                    }
                }
                now++;
            }
            max_all = (now - 1) / 8;
            test = "第 " + std::to_string(page + 1) + " 页 / 共 " + std::to_string(max_all + 1) +
                   " 页";
        }

        // 根据选项渲染界面
        if (selected == 1) {
            return vbox({
                       hbox(choose_all->Render() | flex, choose_by_id->Render() | flex),
                       text("全部选课信息") | bold | center,
                       separator(),
                       hbox({
                           vbox(text("学生ID") | center, el_all[0]) | flex,
                           separator(),
                           vbox(text("姓名") | center, el_all[1]) | flex,
                           separator(),
                           vbox(text("课程名称") | center, el_all[2]) | flex,
                           separator(),
                           vbox(text("综合成绩") | center, el_all[3]) | flex,
                       }),
                       separator(),
                       hbox(page_last->Render(), separatorEmpty(), quit->Render(), separatorEmpty(),
                            page_next->Render()) |
                           center,
                       text(test) | align_right,
                   }) |
                   border | size(WIDTH, EQUAL, 80) | center;
        } else if (selected == 2) {
            Elements content;
            content.push_back(hbox(choose_all->Render() | flex, choose_by_id->Render() | flex));
            content.push_back(text("按课程ID查询选课信息") | bold | center);
            content.push_back(separator());
            content.push_back(hbox({
                                  text("输入课程ID: "),
                                  input_box->Render(),
                                  submit_button->Render(),
                              }) |
                              center);
            content.push_back(separator());
            content.push_back(hbox({
                vbox(text("学生ID") | center, el_by_id[0]) | flex,
                separator(),
                vbox(text("姓名") | center, el_by_id[1]) | flex,
                separator(),
                vbox(text("课程名称") | center, el_by_id[2]) | flex,
                separator(),
                vbox(text("综合成绩") | center, el_by_id[3]) | flex,
            }));
            content.push_back(separator());
            content.push_back(quit->Render() | center);
            if (!enrollmentFound && selected == 2) {
                content.push_back(text("未查询到选课信息") | color(Color::Red) | center);
            }
            return vbox(content) | border | size(WIDTH, EQUAL, 80) | center;
        }
        return text("未知选项") | center;
    });

    // 启动界面
    screen.Loop(renderer);
}

// 选课系统主菜单界面
void ShowEnrollmentScreen(std::vector<Enrollment> &enrollments, std::vector<Student> &students,
                          std::vector<Course> &courses) {
    auto screen = ScreenInteractive::TerminalOutput();

    int selected = 0;
    std::vector<std::string> entries = {"查询选课信息", "添加选课信息", "修改选课信息",
                                        "删除选课信息", "查询不及格名单"};

    auto menu = Menu(&entries, &selected);
    auto enter_button = Button("进入", [&] {
        // screen.Exit();
        switch (selected) {
            case 0:
                searchEnrollmentInfo(enrollments, students, courses);
                break;
            case 1:
                AddEnrollmentScreen(enrollments);
                break;
            case 2:
                ModifyEnrollmentScreen(enrollments);
                break;
            case 3:
                DeleteEnrollmentScreen(enrollments);
                break;
            case 4:
                QueryFailingListScreen(enrollments, students, courses);
                break;
        }
    });

    auto quit_button = Button("返回", [&] { screen.Exit(); });

    auto main_menu_container = Container::Horizontal({menu, enter_button, quit_button});

    auto main_menu_renderer = Renderer(main_menu_container, [&] {
        return vbox({text("选课系统管理") | bold | center, separator(), menu->Render() | center,
                     hbox(enter_button->Render(), separatorEmpty(), quit_button->Render()) |
                         center}) |
               border | size(WIDTH, EQUAL, 50) | center;
    });

    screen.Loop(main_menu_renderer);
}
