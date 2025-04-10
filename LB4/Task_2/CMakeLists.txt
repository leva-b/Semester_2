cmake_minimum_required(VERSION 3.21)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(Models services Sort)

set(SOURCES
    main.cpp
    Models/array.cpp
    Sort/interpolation.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})