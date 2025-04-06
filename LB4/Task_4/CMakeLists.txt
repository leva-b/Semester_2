cmake_minimum_required(VERSION 3.18)
project(Vector)
set(VECTOR_SOURCES sources/vector.cpp)
set(VECTOR_INCLUDES
    headers/iterator.h
    headers/reverseIterator.h
    headers/vector.h)

set(PAIR_INCLUDES headers/pair.h)

add_library(Vector STATIC ${VECTOR_SOURCES} ${VECTOR_INCLUDES})