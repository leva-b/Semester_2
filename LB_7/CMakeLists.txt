cmake_minimum_required(VERSION 3.31)
project(LB_7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "Task_1")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(LB_7 main.cpp)
target_link_libraries(LB_7
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

