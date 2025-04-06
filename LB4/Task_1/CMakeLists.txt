cmake_minimum_required(VERSION 3.21)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем директории, где находятся заголовочные файлы
include_directories(Models services Sort)

# Явно указываем все исходные файлы
set(SOURCES
    main.cpp
    Models/array.cpp
    Sort/heapSort.cpp
    Sort/mergeSort.cpp
    Sort/quickSort.cpp
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})