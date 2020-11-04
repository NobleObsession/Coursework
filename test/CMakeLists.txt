include_directories("${PROJECT_SOURCE_DIR}")

add_executable(main main.cpp)
target_link_libraries(main gtest)

add_executable(tests svg_lib_test.cpp ../src/src/svg_lib.cpp render_test.cpp ../src/src/json.cpp ../src/src/Database.cpp ../src/src/render.cpp)
target_link_libraries(tests gtest gmock_main)
add_test(NAME general_test COMMAND tests)

