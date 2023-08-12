cmake_minimum_required(VERSION 3.25)
project(StlVisualization VERSION 0.1)

add_subdirectory(Visual)

add_executable(visual visual.cpp)

target_link_libraries(visual Visual)

target_include_directories(visual PUBLIC
                           "${PROJECT_SOURCE_DIR}/Visual")
