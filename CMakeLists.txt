cmake_minimum_required(VERSION 3.10)
project(ScientificComputingToolbox)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/Interpolation_Module)
add_subdirectory(src/ODE_Module)
add_subdirectory(src/Statistics_Module)

# Root-level run target
add_custom_target(run-all
    COMMAND make run_interpolation
    COMMAND make run_ode
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Root-level test target
add_custom_target(test-all
    COMMAND make test_interpolation
    COMMAND make test_ode
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Root-level clean target (removing lib and bin directories, and clean the build directory)
add_custom_target(clean-all
    COMMAND rm -rf lib bin
    COMMAND rm -rf $(CMAKE_BINARY_DIR)/*
    COMMAND make clean
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)