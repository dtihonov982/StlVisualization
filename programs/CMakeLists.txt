cmake_minimum_required(VERSION 3.25)

find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory(logger)

add_executable(visual visual.cpp)
target_link_libraries(visual PRIVATE Visual SDL2::SDL2 SDL2_ttf::SDL2_ttf)
target_include_directories(visual PUBLIC "${PROJECT_SOURCE_DIR}/src")
