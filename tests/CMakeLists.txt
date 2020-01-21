cmake_minimum_required(VERSION 2.8)

project(Tests_for_Coursework)
set (CMAKE_CXX_STANDARD 17)


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests tests.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} gtest_main pthread)
