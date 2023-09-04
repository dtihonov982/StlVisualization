cmake_minimum_required(VERSION 3.25)
set(VISUAL_SOURCE App.cpp Player.cpp Rectangle.cpp Label.cpp
                  Playlist.cpp
)
add_library(Visual STATIC ${VISUAL_SOURCE})
target_link_libraries(Visual Common)
target_include_directories(Visual PUBLIC ${PROJECT_SOURCE_DIR}/src)
