#include "screen/StudentScreen.hpp"
#include "utils/StudentUtils.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

using namespace ftxui;

void searchStudentInfo(std::vector<Student> &students) {
    std::string test = "";
    int selected = 1;
    int page = 0, max_all = 0;
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentID, studentInfo;

    // 用于存储按ID查询结果的数组
    Elements el_by_id[6];
    // 标志变量，用于标记是否查询到学生信息
    bool studentFound = true;

    // 按钮组件
    auto choose_all = Button("全部学生信息", [&] {
        selected = 1;
        page = 0;
    });
    auto choose_by_id = Button("按ID查询学生信息", [&] {
        selected = 2;
        page = 0;
        studentInfo = ""; // 清空查询结果
        // 清空按ID查询的元素
        for (auto &el : el_by_id) {
            el.clear();
        }
        studentFound = true;
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
    auto input_box = Input(&studentID, "请输入学生ID") | size(ftxui::HEIGHT, ftxui::EQUAL, 1);
    auto submit_button =
        Button("查询",
               [&] {
                   try {
                       Student student = findStudentById(students, studentID);
                       // 清空之前的元素
                       for (auto &el : el_by_id) {
                           el.clear();
                       }

                       el_by_id[0].push_back(vbox(text(student.getId())) | flex | center);
                       el_by_id[1].push_back(vbox(text(student.getName())) | flex | center);
                       el_by_id[2].push_back(vbox(text(student.getSex() == 0 ? "男" : "女")) |
                                             flex | center);
                       el_by_id[3].push_back(vbox(text(std::to_string(student.getAge()))) | flex |
                                             center);
                       el_by_id[4].push_back(vbox(text(std::to_string(student.getDormNumber()))) |
                                             flex | center);
                       el_by_id[5].push_back(vbox(text(student.getPhoneNumber())) | flex | center);
                       studentFound = true;
                   } catch (const std::exception &e) {
                       // 清空之前的元素
                       for (auto &el : el_by_id) {
                           el.clear();
                       }
                       studentFound = false;
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
        Elements el_all[6];

        // 全部学生信息
        if (selected == 1) {
            int now = 0;
            for (const auto &student : students) {
                if (now / 8 == page) {
                    el_all[0].push_back(text(student.getId()) | center);
                    el_all[1].push_back(text(student.getName()) | center);
                    el_all[2].push_back(text(student.getSex() == 0 ? "男" : "女") | center);
                    el_all[3].push_back(text(std::to_string(student.getAge())) | center);
                    el_all[4].push_back(text(std::to_string(student.getDormNumber())) | center);
                    el_all[5].push_back(text(student.getPhoneNumber()) | center);
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
                       text("全部学生信息") | bold | center,
                       separator(),
                       hbox({
                           vbox(text("学生ID") | center, el_all[0]) | flex,
                           separator(),
                           vbox(text("姓名") | center, el_all[1]) | flex,
                           separator(),
                           vbox(text("性别") | center, el_all[2]) | flex,
                           separator(),
                           vbox(text("年龄") | center, el_all[3]) | flex,
                           separator(),
                           vbox(text("宿舍号") | center, el_all[4]) | flex,
                           separator(),
                           vbox(text("电话号码") | center, el_all[5]) | flex,
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
            content.push_back(text("按ID查询学生信息") | bold | center);
            content.push_back(separator());
            content.push_back(hbox({
                                  text("输入学生ID: "),
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
                vbox(text("性别") | center, el_by_id[2]) | flex,
                separator(),
                vbox(text("年龄") | center, el_by_id[3]) | flex,
                separator(),
                vbox(text("宿舍号") | center, el_by_id[4]) | flex,
                separator(),
                vbox(text("电话号码") | center, el_by_id[5]) | flex,
            }));
            content.push_back(separator());
            content.push_back(quit->Render() | center);
            if (!studentFound && selected == 2) {
                content.push_back(text("未查询到学生信息") | color(Color::Red) | center);
            }
            return vbox(content) | border | size(WIDTH, EQUAL, 80) | center;
        }
        return text("未知选项") | center;
    });

    // 启动界面
    screen.Loop(renderer);
}

// 录入学生信息界面
void AddStudentScreen(std::vector<Student> &students) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string newStudentID, newStudentName, newStudentPhone, newStudentAge, newStudentDorm;
    int newStudentSex = 0;
    std::string addStudentMessage;
    std::string test;
    std::vector<std::string> sex_options = {"男", "女"};

    auto id_input = Input(&newStudentID, "学生ID");
    auto name_input = Input(&newStudentName, "姓名");
    auto phone_input = Input(&newStudentPhone, "电话号码");
    auto sex_toggle = Toggle(sex_options, &newStudentSex);
    auto age_input = Input(&newStudentAge, "年龄");
    auto dorm_input = Input(&newStudentDorm, "宿舍号");

    auto add_button = Button("录入", [&] {
        try {
            // 验证手机号是否为11位
            if (newStudentPhone.length() != 11 ||
                !std::all_of(newStudentPhone.begin(), newStudentPhone.end(), ::isdigit)) {
                addStudentMessage = "";
                test = "电话号码必须为11位数字，请重新输入！";
                return;
            }

            int age = std::stoi(newStudentAge);
            int dorm = std::stoi(newStudentDorm);
            Student newStudent = createStudent(newStudentID, newStudentName, newStudentSex, age,
                                               dorm, newStudentPhone);
            if (addStudent(students, newStudent)) {
                addStudentMessage = "学生信息录入成功！";
                test = "";
                // screen.Exit();
            } else {
                addStudentMessage = "";
                test = "学生ID已存在，录入失败！";
            }
        } catch (const std::exception &e) {
            addStudentMessage = "";
            test = "发生未知错误，请重试！";
        }
    });

    auto back_button = Button("返回", [&] { screen.Exit(); });

    auto add_student_container =
        Container::Vertical({id_input, name_input, phone_input, sex_toggle, age_input, dorm_input,
                             add_button, back_button});

    auto renderer = Renderer(add_student_container, [&] {
        return vbox({
                   text("录入学生信息") | bold | center,
                   separator(),
                   id_input->Render() | center,
                   name_input->Render() | center,
                   hbox({text("性别: "), sex_toggle->Render()}) | center,
                   age_input->Render() | center,
                   dorm_input->Render() | center,
                   phone_input->Render() | center,
                   hbox(add_button->Render() | center, back_button->Render() | center) | center,
                   text(addStudentMessage) | color(Color::Red) | center,
                   text(test) | color(Color::Red) | center,
               }) |
               border;
    });

    screen.Loop(renderer);
}

void EditStudentScreen(std::vector<Student> &students) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentID, newName, newPhone, newAge, newDorm;
    int newSex = 0;
    std::string idErrorMessage, phoneErrorMessage, generalMessage;
    std::vector<std::string> sex_options = {"男", "女"};

    // 输入框和切换组件
    auto id_input = Input(&studentID, "学生ID");
    auto name_input = Input(&newName, "新姓名");
    auto phone_input = Input(&newPhone, "新电话号码");
    auto sex_toggle = Toggle(sex_options, &newSex);
    auto age_input = Input(&newAge, "新年龄");
    auto dorm_input = Input(&newDorm, "新宿舍号");

    // 修改按钮
    auto edit_button = Button("修改", [&] {
        try {
            // 验证学号是否存在
            bool valid = true;
            auto it = std::find_if(students.begin(), students.end(), [&](const Student &student) {
                return student.getId() == studentID;
            });
            if (it == students.end()) {
                idErrorMessage = "学号不存在，请检查后重试！";
                valid = false;
            }
            // 验证手机号是否为11位
            if (newPhone.length() != 11 ||
                !std::all_of(newPhone.begin(), newPhone.end(), ::isdigit)) {
                phoneErrorMessage = "电话号码必须为11位数字，请重新输入！";
                valid = false;
            }
            if (!valid) {
                return;
            }

            // 转换输入数据
            int age = std::stoi(newAge);
            int dorm = std::stoll(newDorm);

            // 调用修改学生信息的函数
            if (updateStudent(students, studentID, newName, newSex, age, dorm, newPhone)) {
                generalMessage = "学生信息修改成功！";
                idErrorMessage = "";
                phoneErrorMessage = "";
            } else {
                generalMessage = "未找到该学生，修改失败！";
                idErrorMessage = "";
                phoneErrorMessage = "";
            }
        } catch (const std::exception &e) {
            generalMessage = "输入数据有误，请检查后重试！";
            idErrorMessage = "";
            phoneErrorMessage = "";
        }
    });

    // 返回按钮
    auto back_button = Button("返回", [&] { screen.Exit(); });

    // 界面容器
    auto edit_student_container =
        Container::Vertical({id_input, name_input, phone_input, sex_toggle, age_input, dorm_input,
                             edit_button, back_button});

    // 渲染器
    auto renderer = Renderer(edit_student_container, [&] {
        return vbox({
                   text("修改学生信息") | bold | center,
                   separator(),
                   id_input->Render() | center,
                   name_input->Render() | center,
                   hbox({text("性别: "), sex_toggle->Render()}) | center,
                   age_input->Render() | center,
                   dorm_input->Render() | center,
                   phone_input->Render() | center,
                   hbox(edit_button->Render() | center, back_button->Render() | center) | center,
                   text(idErrorMessage) | color(Color::Red) | center,
                   text(phoneErrorMessage) | color(Color::Red) | center,
                   text(generalMessage) | color(Color::Green) | center,
               }) |
               border;
    });

    // 启动界面
    screen.Loop(renderer);
}
void DeleteStudentScreen(std::vector<Student> &students) {
    auto screen = ScreenInteractive::TerminalOutput();
    std::string studentID;
    std::string deleteMessage;

    // 输入框
    auto id_input = Input(&studentID, "学生ID");

    // 删除按钮
    auto delete_button = Button("删除", [&] {
        auto it = std::find_if(students.begin(), students.end(), [&](const Student &student) {
            return student.getId() == studentID;
        });

        if (it == students.end()) {
            deleteMessage = "未找到该学生，删除失败！";
        } else {
            if (deleteStudent(students, studentID)) {
                deleteMessage = "学生删除成功！";
            } else {
                deleteMessage = "删除失败，请重试！";
            }
        }
    });

    // 返回按钮
    auto back_button = Button("返回", [&] { screen.Exit(); });

    // 界面容器
    auto delete_student_container = Container::Vertical({
        id_input,
        delete_button,
        back_button,
    });

    // 渲染器
    auto renderer = Renderer(delete_student_container, [&] {
        return vbox({
                   text("删除学生信息") | bold | center,
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

// 主菜单界面
void ShowStudentScreen(std::vector<Student> &students) {
    auto screen = ScreenInteractive::TerminalOutput();

    int selected = 0;
    std::vector<std::string> entries = {"查询学生信息", "录入学生信息", "修改学生信息",
                                        "删除学生信息"};

    auto menu = Menu(&entries, &selected);
    auto enter_button = Button("进入", [&] {
        // screen.Exit();
        switch (selected) {
            case 0:
                searchStudentInfo(students);
                break;
            case 1:
                AddStudentScreen(students);
                break;
            case 2:
                EditStudentScreen(students);
                break;
            case 3:
                DeleteStudentScreen(students);
        }
    });

    auto quit_button = Button("返回", [&] { screen.Exit(); });

    auto main_menu_container = Container::Horizontal({menu, enter_button, quit_button});

    auto main_menu_renderer = Renderer(main_menu_container, [&] {
        return vbox({text("学生信息管理") | bold | center, separator(), menu->Render() | center,
                     hbox(enter_button->Render(), separatorEmpty(), quit_button->Render()) |
                         center}) |
               border | size(WIDTH, EQUAL, 50) | center;
    });

    screen.Loop(main_menu_renderer);
}
