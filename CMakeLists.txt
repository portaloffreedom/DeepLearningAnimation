cmake_minimum_required(VERSION 2.8.11)
project(AnimationTest)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/core
)

set(ANIMATION_TEST_SRC 
    src/gdexample.cpp
    src/gdlibrary.cpp
    src/procedural_moving_skeleton.cpp
)

add_library(animationtest SHARED ${ANIMATION_TEST_SRC})

target_link_libraries(animationtest ${CMAKE_SOURCE_DIR}/src/godot-cpp/bin/libgodot-cpp.linux.release.64.a)

# install stuff
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

install(
    TARGETS animationtest
    DESTINATION bin
)
