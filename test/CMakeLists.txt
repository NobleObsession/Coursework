include_directories("${PROJECT_SOURCE_DIR}")

add_executable(main main.cpp)
target_link_libraries(main gtest)

add_executable(tests tests.cpp)
target_link_libraries(tests gtest gmock_main)
add_test(NAME test COMMAND tests)


# First Simple Google Test
#add_executable(simple simple.cpp)
#target_link_libraries(simple gtest gmock_main)
#add_test(NAME simple_test COMMAND simple)
