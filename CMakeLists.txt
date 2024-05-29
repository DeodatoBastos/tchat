cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(tchat VERSION 0.0.1 LANGUAGES CXX)

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_VERSION)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Now simply link your own targets against gtest, gmock,
# etc. as appropriate

include(GoogleTest)

SET(SRC "src" CACHE FILEPATH "Directory with sources")

FILE(GLOB sources ${SRC}/*.cpp)
add_executable(tchat_tests test/test_utils.cpp ${sources})

include_directories( ${SRC} include )

target_link_libraries(tchat_tests gtest gtest_main )
 
gtest_discover_tests(tchat_tests)





