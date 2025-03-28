#include "screen/EnrollmentScreen.hpp"
#include "core/Course.hpp"
#include "core/Student.hpp"
#include "utils/EnrollmentUtils.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

// 模拟查询选课信息的函数
void QueryEnrollmentInfo(std::vector<Enrollment> &enrollments) {
  // 这里可以添加具体的查询逻辑
  // 目前仅作为占位
}

// 模拟添加选课信息的函数
void AddEnrollmentInfo(std::vector<Enrollment> &enrollments) {
  // 这里可以添加具体的添加逻辑
  // 目前仅作为占位
}

// 选课系统主菜单界面
void ShowEnrollmentScreen(std::vector<Enrollment> &enrollments,std::vector<Student> &students,std::vector<Course> &courses) {
  auto screen = ScreenInteractive::TerminalOutput();

  int selected = 0;
  std::vector<std::string> entries = {"查询选课信息", "添加选课信息"};

  auto menu = Menu(&entries, &selected);
  auto enter_button = Button("进入", [&] {
    screen.Exit();
    switch (selected) {
    case 0:
      QueryEnrollmentInfo(enrollments);
      break;
    case 1:
      AddEnrollmentInfo(enrollments);
      break;
    }
  });

  auto quit_button = Button("返回", [&] { screen.Exit(); });

  auto main_menu_container =
      Container::Horizontal({menu, enter_button, quit_button});

  auto main_menu_renderer = Renderer(main_menu_container, [&] {
    return vbox({text("选课系统管理") | bold | center, separator(),
                 menu->Render() | center,
                 hbox(enter_button->Render(), separatorEmpty(),
                      quit_button->Render()) |
                     center}) |
           border | size(WIDTH, EQUAL, 50) | center;
  });

  screen.Loop(main_menu_renderer);
}
