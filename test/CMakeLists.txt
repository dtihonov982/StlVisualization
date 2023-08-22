cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(stopwatch stopwatch.cpp)
target_link_libraries(stopwatch PRIVATE Logger)
target_include_directories(logger PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(record record.cpp)
target_link_libraries(record PRIVATE Common)
target_include_directories(record PUBLIC "${PROJECT_SOURCE_DIR}/src")
