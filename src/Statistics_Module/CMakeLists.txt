cmake_minimum_required(VERSION 3.10)
project(Statistics_Module)

# Set the module
set(MODULE Statistics_Module)

# Compilation Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# Check dependencies
find_package(Eigen3 REQUIRED)

# Set relative paths
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(SRC_DIR ${ROOT_DIR}/src)
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(MODULE_INCLUDE_DIR ${INCLUDE_DIR}/${MODULE})
set(MODULE_SRC_DIR ${SRC_DIR}/${MODULE})
set(MAIN_DIR ${ROOT_DIR}/main)
set(TEST_DIR ${ROOT_DIR}/tests)
set(LIB_DIR ${ROOT_DIR}/lib)
set(BIN_DIR ${ROOT_DIR}/bin)

# Define source files
set(SOURCES 
    ${MODULE_SRC_DIR}/StatsAnalyzer.cpp
    ${MODULE_SRC_DIR}/Dataset.cpp
)

# Create shared library
add_library(${MODULE} SHARED ${SOURCES})

# Set include directories
target_include_directories(${MODULE} PUBLIC
    ${INCLUDE_DIR}
    ${MODULE_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
)

# Link with Eigen3
target_link_libraries(${MODULE} PUBLIC Eigen3::Eigen)

# Compile the main program
add_executable(main_stat ${MAIN_DIR}/Statistics_Module_main.cpp)
target_link_libraries(main_stat PRIVATE ${MODULE})

# Compile the test program
add_executable(test_stat ${TEST_DIR}/Statistics_Module_test.cpp)
target_link_libraries(test_stat PRIVATE ${MODULE})

# Set output directories
set_target_properties(${MODULE} main_stat test_stat 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
)

# Custom targets
add_custom_target(run-statistics
    COMMAND ${BIN_DIR}/main_stat ${ARGS}
    DEPENDS main_stat
    WORKING_DIRECTORY ${BIN_DIR}
)

add_custom_target(test-statistics
    COMMAND ${BIN_DIR}/test_stat
    DEPENDS test_stat
    WORKING_DIRECTORY ${BIN_DIR}
)