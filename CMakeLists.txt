cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(JWS)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 查找 包
# 独立的指令
find_package(nlohmann_json REQUIRED)
find_package(ftxui CONFIG REQUIRED)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(JWS ${SOURCES} main.cpp)

# 链接库
target_link_libraries(JWS PRIVATE 
    nlohmann_json::nlohmann_json 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
)