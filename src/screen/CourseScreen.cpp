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
    auto input_box = Input(&courseID, "请输入课程ID") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);
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
                       el_by_id[1].push_back(vbox(text(result.getName())) | flex | center);
                       el_by_id[2].push_back(vbox(text(std::to_string(result.getCredit()))) | flex |
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
                    el_all[2].push_back(text(std::to_string(course.getCredit())) | center);
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
                       hbox(page_last->Render(), separatorEmpty(), quit->Render(), separatorEmpty(),
                            page_next->Render()) |
                           center,
                       text(test) | align_right,
                   }) |
                   border | size(WIDTH, EQUAL, 80) | center;
        } else if (selected == 2) {
            Elements content;
            content.push_back(hbox(choose_all->Render() | flex, choose_by_id->Render() | flex));
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
                content.push_back(text("未查询到课程信息") | color(Color::Red) | center);
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

    auto add_course_container =
        Container::Vertical({id_input, name_input, credits_input, add_button, back_button});

    auto renderer = Renderer(add_course_container, [&] {
        return vbox({
                   text("录入课程信息") | bold | center,
                   separator(),
                   id_input->Render() | center,
                   name_input->Render() | center,
                   credits_input->Render() | center,
                   hbox(add_button->Render() | center, back_button->Render() | center) | center,
                   text(addCourseMessage) | center,
               }) |
               border;
    });

    screen.Loop(renderer);
}

void EditCourseScreen(std::vector<Course> &courses) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string courseID, newCourseName, newCredits;
    std::string editCourseMessage;

    // 输入框
    auto id_input = Input(&courseID, "课程ID");
    auto name_input = Input(&newCourseName, "新课程名称");
    auto credits_input = Input(&newCredits, "新学分");

    // 修改按钮
    auto edit_button = Button("修改", [&] {
        try {
            // 验证课程是否存在
            auto it = std::find_if(courses.begin(), courses.end(), [&](const Course &course) {
                return course.getId() == courseID;
            });

            if (it == courses.end()) {
                editCourseMessage = "未找到该课程信息，请检查课程ID！";
                return;
            }
            int credits = std::stoi(newCredits); // 转换学分为整数
            editCourseMessage = modify_course(courses, courseID, newCourseName, credits);
        } catch (const std::exception &e) {
            editCourseMessage = "发生未知错误，请重试！";
        }
    });

    // 返回按钮
    auto back_button = Button("返回", screen.ExitLoopClosure());

    // 界面容器
    auto edit_course_container = Container::Vertical({
        id_input,
        name_input,
        credits_input,
        edit_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(edit_course_container, [&] {
        return vbox({
                   text("修改课程信息") | bold | center,
                   separator(),
                   id_input->Render() | center,
                   name_input->Render() | center,
                   credits_input->Render() | center,
                   hbox(edit_button->Render() | center, back_button->Render() | center) | center,
                   text(editCourseMessage) | color(Color::Red) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}

void DeleteCourseScreen(std::vector<Course> &courses) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string courseID;
    std::string deleteMessage;

    // 输入框
    auto id_input = Input(&courseID, "课程ID");

    // 删除按钮
    auto delete_button = Button("删除", [&] {
        auto it = std::find_if(courses.begin(), courses.end(),
                               [&](const Course &course) { return course.getId() == courseID; });

        if (it == courses.end()) {
            deleteMessage = "未找到该课程信息，删除失败！";
        } else {
            if (delete_course(courses, courseID)) {
                deleteMessage = "课程删除成功！";
            } else {
                deleteMessage = "删除失败，请重试！";
            }
        }
    });

    // 返回按钮
    auto back_button = Button("返回", screen.ExitLoopClosure());

    // 界面容器
    auto delete_course_container = Container::Vertical({
        id_input,
        delete_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(delete_course_container, [&] {
        return vbox({
                   text("删除课程信息") | bold | center,
                   separator(),
                   id_input->Render() | center,
                   separator(),
                   hbox({
                       delete_button->Render() | center,
                       back_button->Render() | center,
                   }) | center,
                   separator(),
                   text(deleteMessage) | color(Color::Red) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}

void SearchCourseScreen(std::vector<Course> &courses) {
    std::string keywords;
    std::string test = "";
    int page = 0, max_all = 0;
    auto screen = ScreenInteractive::TerminalOutput();
    Elements el_all[3];

    // 搜索结果
    std::vector<Course> searchResults;

    // 按钮组件
    auto search_button = Button("搜索", [&] {
        try {
            searchResults.clear();
            auto results = search_course(courses, keywords); // 调用模糊查找函数
            searchResults.assign(results.begin(), results.end());
            if (searchResults.empty()) {
                test = "未找到匹配的课程信息！";
            } else {
                test = "搜索完成，共找到 " + std::to_string(searchResults.size()) + " 条结果。";
                page = 0; // 重置到第一页
            }
        } catch (const std::exception &e) {
            test = "搜索时发生错误，请重试！";
        }
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

    // 输入框
    auto input_box = Input(&keywords, "请输入关键词") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);

    // 容器组件
    auto comp = Container::Horizontal({
        input_box,
        search_button,
        quit,
        page_last,
        page_next,
    });

    // 渲染器
    auto renderer = Renderer(comp, [&] {
        // 清空当前页的显示内容
        for (auto &el : el_all) {
            el.clear();
        }

        // 分页显示搜索结果
        int now = 0;
        for (const auto &course : searchResults) {
            if (now / 8 == page) { // 每页显示 8 条
                el_all[0].push_back(text(course.getId()) | center);
                el_all[1].push_back(text(course.getName()) | center);
                el_all[2].push_back(text(std::to_string(course.getCredit())) | center);
            }
            now++;
        }
        max_all = (now - 1) / 8;

        // 渲染界面
        return vbox({
                   hbox(input_box->Render() | flex, search_button->Render() | center |
                                                        ftxui::size(ftxui::WIDTH, ftxui::EQUAL,
                                                                    10) // 按钮宽度固定为 10
                        ),
                   text("关键词搜索课程") | bold | center,
                   separator(),
                   hbox({
                       vbox(text("课程ID") | center, el_all[0]) | flex,
                       separator(),
                       vbox(text("课程名称") | center, el_all[1]) | flex,
                       separator(),
                       vbox(text("学分") | center, el_all[2]) | flex,
                   }),
                   separator(),
                   hbox(page_last->Render(), separatorEmpty(), quit->Render(), separatorEmpty(),
                        page_next->Render()) |
                       center,
                   text(test) | align_right,
               }) |
               border | size(WIDTH, EQUAL, 80) | center;
    });

    // 启动界面
    screen.Loop(renderer);
}

void ShowCourseScreen(std::vector<Course> &courses) {
    auto screen = ScreenInteractive::TerminalOutput();

    int selected = 0;
    std::vector<std::string> entries = {"总揽课程信息", "关键词查询课程", "录入课程信息",
                                        "修改课程信息", "删除课程信息"};

    auto menu = Menu(&entries, &selected);
    auto enter_button = Button("进入", [&] {
        // screen.Exit();
        switch (selected) {
            case 0:
                PrintCourseInfo(courses);
                break;
            case 1:
                SearchCourseScreen(courses);
                break;
            case 2:
                AddCourseScreen(courses);
                break;
            case 3:
                EditCourseScreen(courses);
                break;
            case 4:
                DeleteCourseScreen(courses);
                break;
        }
    });

    auto quit_button = Button("返回", screen.ExitLoopClosure());

    auto main_menu_container = Container::Horizontal({menu, enter_button, quit_button});

    auto main_menu_renderer = Renderer(main_menu_container, [&] {
        return vbox({text("课程信息管理") | bold | center, separator(), menu->Render() | center,
                     hbox(enter_button->Render(), separatorEmpty(), quit_button->Render()) |
                         center}) |
               border | size(WIDTH, EQUAL, 50) | center;
    });

    screen.Loop(main_menu_renderer);
}