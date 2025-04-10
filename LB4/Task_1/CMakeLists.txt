cmake_minimum_required(VERSION 3.21)
project(Task_1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(Models services Sort)

set(SOURCES
    main.cpp
    Models/array.cpp
    Sort/heapSort.cpp
    Sort/mergeSort.cpp
    Sort/quickSort.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})