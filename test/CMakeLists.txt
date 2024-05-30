file(GLOB TEST_FILES *.cpp)

add_executable(run_tests ${TEST_FILES})

target_link_libraries(run_tests PRIVATE ${CMAKE_PROJECT_NAME} gtest gtest_main)
add_test(NAME run_tests COMMAND run_tests)
