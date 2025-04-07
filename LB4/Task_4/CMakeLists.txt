cmake_minimum_required(VERSION 3.18)
project(Vector)
set(VECTOR_SOURCES 
    sources/vector.cpp)
set(VECTOR_INCLUDES
    headers/iterator.h
    headers/reverseIterator.h
    headers/pair.h
    headers/vector.h)
    


add_library(Vector STATIC ${VECTOR_SOURCES} ${VECTOR_INCLUDES} )
include_directories(headers)
