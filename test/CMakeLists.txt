
add_executable(main main.cpp)
target_link_libraries(main gtest)

add_executable(tests svg_lib_test.cpp coordinate_convertor_test.cpp)
target_link_libraries(tests gtest gmock_main json_lib svg_lib database_lib coord_convertor_lib)
add_test(NAME general_test COMMAND tests)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/queries_for_render_test.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
