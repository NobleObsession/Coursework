cmake_minimum_required(VERSION 2.8)

project(Coursework)

set (CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Solution ${SOURCES})
