cmake_minimum_required(VERSION 2.8.11)
project(animation)

set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#TODO change this for "install"
# add_definitions(-DASSETS_FOLDER=\"${PROJECT_SOURCE_DIR}/assets/\")

# Find the Eigen library
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(animation_SRC
    src/animation/controller.cpp
    src/animation/keyboard_controller.cpp
    src/animation/nn/neuralnetwork.cpp
    src/animation/nn/tensor.cpp
    src/animation/nn/models/model.cpp
    src/animation/nn/models/pfnnmodel.cpp
    src/animation/nn/models/vanillamodel.cpp
    src/animation/trajectory/trajectory.cpp
    src/animation/trajectory/trajectorypoint.cpp
    src/math/matrix4f.cpp
    src/math/quaternionf.cpp
    src/math/vector3f.cpp
)

# set(nn_debug_view_SRC
#   src/main.cpp
#   src/graphics/chart.cpp
#   src/graphics/fakeskeleton.cpp
#   src/graphics/faketerrain.cpp
#   src/graphics/nn_debug_view.cpp
#   src/graphics/timedgenerator.cpp
# )

# Create code from a list of Qt designer ui files.
#set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
# qt5_wrap_ui(nn_debug_view_SRC src/graphics/nn_debug_view.ui)

# Tell CMake to create the helloworld executable
# add_executable(nn_debug_view ${nn_debug_view_SRC})

add_library(animation STATIC ${animation_SRC})

set_property(TARGET animation
             PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(animation 
    PUBLIC "${PROJECT_SOURCE_DIR}/src/"
    PUBLIC ${EIGEN_INCLUDE_DIRS})

# Use the Widgets module from Qt 5.
# target_link_libraries(nn_debug_view animation Qt5::Widgets Qt5::Charts Eigen3::Eigen)
target_link_libraries(animation Eigen3::Eigen)

# Install the executable
# install(TARGETS nn_debug_view DESTINATION bin)

# Tests
if (WITH_TESTS)
    message("TESTS ACTIVATED")
    find_package(Catch2 REQUIRED)
    enable_testing()

    set(test_SRC
        test/TestTrajectoryIterator.cpp
        test/TestMain.cpp
    )

    add_executable(catch_test ${test_SRC})
    target_link_libraries(catch_test animation Catch2::Catch)
    include(ParseAndAddCatchTests)
    ParseAndAddCatchTests(catch_test)
else ()
    message("TESTS DEACTIVATED")
endif ()
