cmake_minimum_required(VERSION 2.8.11)
project(AnimationTest)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASSETS_FOLDER=\"${PROJECT_SOURCE_DIR}/assets/\")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/core
)

set(ANIMATION_TEST_SRC 
    src/gdexample.cpp
    src/gdlibrary.cpp
    src/procedural_moving_skeleton.cpp
    src/godot_skeleton.cpp
)

add_library(animationtest SHARED ${ANIMATION_TEST_SRC})

add_subdirectory(src/animation)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(GODOT_CPP_LIB ${CMAKE_SOURCE_DIR}/src/godot-cpp/bin/libgodot-cpp.linux.debug.64.a)
else()
    set(GODOT_CPP_LIB ${CMAKE_SOURCE_DIR}/src/godot-cpp/bin/libgodot-cpp.linux.release.64.a)
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

target_link_libraries(animationtest animation ${GODOT_CPP_LIB})

# install stuff
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

install(
    TARGETS animationtest
    DESTINATION bin
)
