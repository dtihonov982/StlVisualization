cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)

set (SRCS reduce_accumulate.cpp
          #sort.cpp
          #sort_list.cpp
          shuffle.cpp
          #shift.cpp need c++20 std
          reverse_copy.cpp
          next_permutation.cpp
          is_permutation.cpp
          adjacent_difference.cpp
          partition.cpp
          merge.cpp
          transform_numeric.cpp
          up_low_bound.cpp
          heap.cpp
          remove_if.cpp
)
add_executable(logger ${SRCS})
#set_property (TARGET logger PROPERTY CXX_STANDARD 20) problem with shift
target_link_libraries(logger PRIVATE Logger GTest::GTest GTest::Main)
target_include_directories(logger PUBLIC "${PROJECT_SOURCE_DIR}/src")

