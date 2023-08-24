cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(stopwatch stopwatch.cpp)
target_link_libraries(stopwatch PRIVATE Logger)
target_include_directories(logger PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(record record.cpp)
target_link_libraries(record PRIVATE Common)
target_include_directories(record PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(label label.cpp)
target_link_libraries(label PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(label PRIVATE SDL2::SDL2)
target_link_libraries(label PRIVATE Visual)
target_include_directories(label PUBLIC "${PROJECT_SOURCE_DIR}/src")
