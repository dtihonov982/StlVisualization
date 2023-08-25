cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)

set (SRCS reduce_accumulate.cpp
          sort.cpp
          shuffle.cpp
          next_permutation.cpp
          is_permutation.cpp
          adjacent_difference.cpp
)
add_executable(logger ${SRCS})
target_link_libraries(logger PRIVATE Logger GTest::GTest GTest::Main)
target_include_directories(logger PUBLIC "${PROJECT_SOURCE_DIR}/src")

