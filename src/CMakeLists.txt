file(GLOB SRC *.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC .)
