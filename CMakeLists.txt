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
    COMMAND ${CMAKE_COMMAND} --build . --target run-interpolation
    COMMAND ${CMAKE_COMMAND} --build . --target run-ode
    COMMAND ${CMAKE_COMMAND} --build . --target run-statistics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Root-level test target
add_custom_target(test-all
    COMMAND ${CMAKE_COMMAND} --build . --target test-interpolation
    COMMAND ${CMAKE_COMMAND} --build . --target test-ode
    COMMAND ${CMAKE_COMMAND} --build . --target test-statistics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Root-level py test target
add_custom_target(py-test-all
    COMMAND ${CMAKE_COMMAND} --build . --target py-test-interpolation
    #COMMAND ${CMAKE_COMMAND} --build . --target py-test-ode
    COMMAND ${CMAKE_COMMAND} --build . --target py-test-statistics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Root-level clean target
add_custom_target(clean-all
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/lib
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/bin
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/output/*
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)