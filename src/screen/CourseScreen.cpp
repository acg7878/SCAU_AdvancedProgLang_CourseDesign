#include "screen/CourseScreen.hpp"
#include "utils/CourseUtils.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <vector>

using namespace ftxui;

void PrintCourseInfo(std::vector<Course> &courses) {
  std::string test = "";
  int selected = 1;
  int page = 0, max_all = 0;
  auto screen = ScreenInteractive::TerminalOutput();
  std::string courseID, courseInfo;

  // 用于存储按ID查询结果的数组
  Elements el_by_id[3];
  bool courseFound = true;

  // 按钮组件
  auto choose_all = Button("全部课程信息", [&] {
    selected = 1;
    page = 0;
  });
  auto choose_by_id = Button("按ID查询课程信息", [&] {
    selected = 2;
    page = 0;
    courseInfo = ""; // 清空查询结果
    for (auto &el : el_by_id) {
      el.clear();
    }
    courseFound = true;
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
  auto input_box =
      Input(&courseID, "请输入课程ID") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);
  auto submit_button =
      Button("查询",
             [&] {
               try {
                 auto result = findCourseById(courses, courseID);
                 for (auto &el : el_by_id) {
                   el.clear();
                 }
                 //  el_by_id[0].push_back(vbox(text("课程ID"), separator()));
                 //  el_by_id[1].push_back(vbox(text("课程名称"), separator()));
                 //  el_by_id[2].push_back(vbox(text("学分"), separator()));

                 el_by_id[0].push_back(vbox(text(courseID)) | flex | center);
                 el_by_id[1].push_back(vbox(text(result.getName())) | flex |
                                       center);
                 el_by_id[2].push_back(
                     vbox(text(std::to_string(result.getCredit()))) | flex |
                     center);
                 courseFound = true;
               } catch (const std::exception &e) {
                 for (auto &el : el_by_id) {
                   el.clear();
                 }
                 courseFound = false;
               }
             }) |
      size(ftxui::HEIGHT, ftxui::EQUAL, 3);

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
    Elements el_all[3];

    // 全部课程信息
    if (selected == 1) {
      int now = 0;
      for (const auto &course : courses) {
        if (now / 8 == page) {
          el_all[0].push_back(text(course.getId()) | center);
          el_all[1].push_back(text(course.getName()) | center);
          el_all[2].push_back(text(std::to_string(course.getCredit())) |
                              center);
        }
        now++;
      }
      max_all = (now - 1) / 8;
      test = "第 " + std::to_string(page + 1) + " 页 / 共 " +
             std::to_string(max_all + 1) + " 页";
    }

    // 根据选项渲染界面
    if (selected == 1) {
      return vbox({
                 hbox(choose_all->Render() | flex,
                      choose_by_id->Render() | flex),
                 text("全部课程信息") | bold | center,
                 separator(),
                 hbox({
                     vbox(text("课程ID") | center, el_all[0]) | flex,
                     separator(),
                     vbox(text("课程名称") | center, el_all[1]) | flex,
                     separator(),
                     vbox(text("学分") | center, el_all[2]) | flex,
                 }),
                 separator(),
                 hbox(page_last->Render(), separatorEmpty(), quit->Render(),
                      separatorEmpty(), page_next->Render()) |
                     center,
                 text(test) | align_right,
             }) |
             border | size(WIDTH, EQUAL, 80) | center;
    } else if (selected == 2) {
      Elements content;
      content.push_back(
          hbox(choose_all->Render() | flex, choose_by_id->Render() | flex));
      content.push_back(text("按ID查询课程信息") | bold | center);
      content.push_back(separator());
      content.push_back(hbox({
                            text("输入课程ID: "),
                            input_box->Render(),
                            submit_button->Render(),
                        }) |
                        center);
      content.push_back(separator());
      content.push_back(hbox({
          vbox(text("课程ID") | center, el_by_id[0]) | flex,
          separator(),
          vbox(text("课程名称") | center, el_by_id[1]) | flex,
          separator(),
          vbox(text("学分") | center, el_by_id[2]) | flex,
      }));
      content.push_back(separator());
      content.push_back(quit->Render() | center);
      if (!courseFound && selected == 2) {
        content.push_back(text("未查询到课程信息") | color(Color::Red) |
                          center);
      }
      return vbox(content) | border | size(WIDTH, EQUAL, 80) | center;
    }
    return text("未知选项") | center;
  });

  // 启动界面
  screen.Loop(renderer);
}

void AddCourseScreen(std::vector<Course> &courses) {
  auto screen = ScreenInteractive::TerminalOutput();
  std::string newCourseID, newCourseName, newCredits;
  std::string addCourseMessage;

  auto id_input = Input(&newCourseID, "课程ID");
  auto name_input = Input(&newCourseName, "课程名称");
  auto credits_input = Input(&newCredits, "学分");

  auto add_button = Button("录入", [&] {
    try {
      int credits = std::stoi(newCredits);
      bool success = add_course(courses, newCourseID, newCourseName, credits);
      if (success) {
        screen.Exit();
      }
    } catch (const std::invalid_argument &e) {
      addCourseMessage = "输入的学分不是有效的数字，请重新输入！";
    } catch (const std::out_of_range &e) {
      addCourseMessage = "输入的学分超出范围，请重新输入！";
    } catch (const std::exception &e) {
      addCourseMessage = "发生未知错误，请重试！";
    }
  });

  auto back_button = Button("返回", screen.ExitLoopClosure());

  auto add_course_container = Container::Vertical(
      {id_input, name_input, credits_input, add_button, back_button});

  auto renderer = Renderer(add_course_container, [&] {
    return vbox({text("录入课程信息") | bold | center, separator(),
                 id_input->Render() | center, name_input->Render() | center,
                 credits_input->Render() | center,
                 add_button->Render() | center, text(addCourseMessage) | center,
                 back_button->Render() | center});
  });

  screen.Loop(renderer);
}

void ShowCourseScreen(std::vector<Course> &courses) {
  auto screen = ScreenInteractive::TerminalOutput();

  int selected = 0;
  std::vector<std::string> entries = {"查询课程信息", "录入课程信息"};

  auto menu = Menu(&entries, &selected);
  auto enter_button = Button("进入", [&] {
    screen.Exit();
    switch (selected) {
    case 0:
      PrintCourseInfo(courses);
      break;
    case 1:
      AddCourseScreen(courses);
      break;
    }
  });

  auto quit_button = Button("返回", screen.ExitLoopClosure());

  auto main_menu_container =
      Container::Horizontal({menu, enter_button, quit_button});

  auto main_menu_renderer = Renderer(main_menu_container, [&] {
    return vbox({text("课程信息管理") | bold | center, separator(),
                 menu->Render() | center,
                 hbox(enter_button->Render(), separatorEmpty(),
                      quit_button->Render()) |
                     center}) |
           border | size(WIDTH, EQUAL, 50) | center;
  });

  screen.Loop(main_menu_renderer);
}