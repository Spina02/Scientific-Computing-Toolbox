cmake_minimum_required(VERSION 3.10)
project(ScientificComputingToolbox)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/Interpolation_Module)

# Root-level clean target
add_custom_target(clean-interpolation
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/clean-all.cmake
)