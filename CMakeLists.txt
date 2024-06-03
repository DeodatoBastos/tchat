cmake_minimum_required(VERSION 3.20)

# cmake_policy(SET CMP0048 NEW)
project(tchat VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set( CMAKE_CXX_FLAGS_DEBUG  "-fsanitize=address -fsanitize=leak -g")
set( CMAKE_CXX_FLAGS_RELEASE  "")
set( CMAKE_CXX_FLAGS  "-Ofast -DNDEBUG -std=c++20 -march=native -fpic -ftree-vectorize -pedantic -Wall -Wextra -Werror ")

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_VERSION)

enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)


add_subdirectory(src)
add_subdirectory(test)

add_executable(client main_users.cpp)
target_link_libraries(client PRIVATE ${CMAKE_PROJECT_NAME})

add_executable(server main_server.cpp)
target_link_libraries(server PRIVATE ${CMAKE_PROJECT_NAME})
