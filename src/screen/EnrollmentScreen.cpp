#include "screen/EnrollmentScreen.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void ShowEnrollmentScreen(ScreenInteractive& screen,
                          std::vector<Enrollment>& enrollments,
                          std::vector<Student>& students,
                          std::vector<Course>& courses) {
    auto back_button = Button("返回主菜单", screen.ExitLoopClosure());
    auto renderer = Renderer(back_button, [&] {
        return vbox({
            text("选课信息管理") | bold | center,
            separator(),
            text("这里是选课信息的功能界面") | center,
            separator(),
            back_button->Render() | center,
        });
    });
    screen.Loop(renderer);
}