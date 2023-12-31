cmake_minimum_required(VERSION 3.25)
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(stopwatch stopwatch.cpp)
target_link_libraries(stopwatch PRIVATE Logger)
target_include_directories(stopwatch PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(record record.cpp)
target_link_libraries(record PRIVATE Common)
target_include_directories(record PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(label label.cpp)
target_link_libraries(label PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(label PRIVATE SDL2::SDL2)
target_link_libraries(label PRIVATE Visual)
target_include_directories(label PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(playlist playlist.cpp)
target_link_libraries(playlist PRIVATE Common Visual)
target_include_directories(playlist PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(series series.cpp)
target_link_libraries(series PRIVATE SDL2_ttf::SDL2_ttf SDL2::SDL2 Visual)
target_include_directories(series PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(ColoredSeries ColoredSeries.cpp)
target_link_libraries(ColoredSeries PRIVATE SDL2_ttf::SDL2_ttf SDL2::SDL2 Visual)
target_include_directories(ColoredSeries PUBLIC "${PROJECT_SOURCE_DIR}/src")

add_executable(RecordingPauseTest RecordingPauseTest.cpp)
target_link_libraries(RecordingPauseTest PRIVATE Logger)
target_include_directories(RecordingPauseTest PUBLIC "${PROJECT_SOURCE_DIR}/src")

