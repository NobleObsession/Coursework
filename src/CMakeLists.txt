cmake_minimum_required(VERSION 3.10.2)

project(Coursework VERSION 1.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(include)
link_directories(/home/ychernysheva/Documents/Gtest_Gmock/lib/)

file(GLOB SOURCES "src/*.cpp")

add_executable(Solution ${SOURCES})
