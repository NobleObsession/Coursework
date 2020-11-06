include_directories("${PROJECT_SOURCE_DIR}")

add_executable(main main.cpp)
target_link_libraries(main gtest)

add_executable(tests tests.cpp svg_lib_test.cpp ../src/src/svg_lib.cpp)
target_link_libraries(tests gtest gmock_main)
add_test(NAME general_test COMMAND tests)

