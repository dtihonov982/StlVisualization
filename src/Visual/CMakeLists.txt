add_subdirectory(../Common)
add_library(Visual App.cpp)
target_link_libraries(Visual Common)
target_include_directories(Visual PUBLIC
                           "${PROJECT_SOURCE_DIR}/Common")
