cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(myString)

set(SOURCES
    sources/myString.cpp
    sources/iterator.cpp
)

set(INCLUDES
    headers/myString.h)

# set(SO_MYSTRING)
add_library(myString SHARED ${SOURCES} ${INCLUDES})

target_compile_options(myString PRIVATE -fvisibility=hidden)
target_compile_definitions(myString PRIVATE MYSTRING_API= "__attribute__((visibility(\"default\")))")

target_include_directories(myString PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)
