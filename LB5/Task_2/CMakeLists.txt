cmake_minimum_required(VERSION 3.18)
project(myString)
set(SOURCES
    sources/iterator.cpp
    sources/myString.cpp)

set(INCLUDES
    headers/myString.h)

# set(SO_MYSTRING)
add_library(myString SHARED ${SOURCES} ${INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE myString)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

# target_compile_definitions(myString PUBLIC _IMPORTING)