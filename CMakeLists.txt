cmake_minimum_required(VERSION 3.0.2)
project(AnimationTest)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(SCONS_PLATFORM windows)
elseif(APPLE)
    set(SCONS_PLATFORM osx)
elseif(UNIX AND NOT APPLE)
    set(SCONS_PLATFORM linux)
else()
    message("Platorm not supported. Please subbmit a patch to add support to your platorm!")
endif()

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(SCONS_TARGET debug)
else()
    set(SCONS_TARGET release)
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

include(ProcessorCount)
ProcessorCount(N_CPU)
if(N_CPU EQUAL 0)
    SET(N_CPU 1)
endif()

add_definitions(-DASSETS_FOLDER=\"${PROJECT_SOURCE_DIR}/assets/\")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/godot-cpp/include/core
)

add_custom_command(OUTPUT src/godot-cpp/godot_api.json
                  COMMAND godot --gdnative-generate-json-api godot_api.json
                  WORKING_DIRECTORY src/godot-cpp
                  DEPENDS prepare
)

set(GODOT_CPP_LIB ${CMAKE_SOURCE_DIR}/src/godot-cpp/bin/libgodot-cpp.${SCONS_PLATFORM}.${SCONS_TARGET}.64.a)
add_custom_command(OUTPUT ${GODOT_CPP_LIB}
    COMMAND scons platform=${SCONS_PLATFORM} headers_dir=../godot_headers generate_bindings=yes target=${SCONS_TARGET} -j${N_CPU}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/godot-cpp)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/src/godot-cpp/include/Reference.hpp
    DEPENDS ${GODOT_CPP_LIB}
)

set(ANIMATION_TEST_SRC
    src/gdlibrary.cpp # "Main" file
    src/godotcompatibilitytools.h
    src/gdexample.cpp
    src/gdexample.h
    src/procedural_moving_skeleton.cpp
    src/procedural_moving_skeleton.h
    src/godot_skeleton.cpp
    src/godot_skeleton.h
    src/godot-cpp/godot_api.json
    ${CMAKE_SOURCE_DIR}/src/godot-cpp/include/Reference.hpp
)

add_library(animationtest SHARED ${ANIMATION_TEST_SRC})

add_subdirectory(src/animation)

target_link_libraries(animationtest animation ${GODOT_CPP_LIB})

# install stuff
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

install(
    TARGETS animationtest
    DESTINATION bin
)
