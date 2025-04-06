cmake_minimum_required(VERSION 3.16)

project(Task_3_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Task_3_4 main.cpp
    number.h number.cpp
    expression.h expression.cpp
    binaryoperation.h binaryoperation.cpp)

include(GNUInstallDirs)
install(TARGETS Task_3_4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
