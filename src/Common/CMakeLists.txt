cmake_minimum_required(VERSION 3.25)
set(COMMON_SOURCE Common.cpp Record.cpp Config.cpp)
add_library(Common STATIC ${COMMON_SOURCE})
target_include_directories(Common PUBLIC ${PROJECT_SOURCE_DIR}/src
                                         ${PROJECT_SOURCE_DIR}/json/include)


