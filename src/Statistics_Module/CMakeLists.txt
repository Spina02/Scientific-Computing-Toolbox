cmake_minimum_required(VERSION 3.10)
project(Statistics_Module)

# Set the module
set(MODULE Statistics_Module)

# Compilation Flags
set(CXXFLAGS "-Wall -Wextra -Wpedantic -Werror")

# Check dependencies and if not found, give a message
find_package(Eigen3 REQUIRED)

if(NOT EIGEN3_FOUND)
    message(FATAL_ERROR "Eigen3 not found!")
endif()

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

add_library(Statistics_Module INTERFACE)

# Set the include directories
include_directories(
    ${INCLUDE_DIR}
    ${MODULE_INCLUDE_DIR}
)

# Since it's a header-only library, we don't need to compile anything
# We just need to set the include directories

# Link the module with Eigen3
target_link_libraries(Statistics_Module INTERFACE Eigen3::Eigen)

# Compile the main program
add_executable(main_stat ${MAIN_DIR}/Statistics_Module_main.cpp)

# Link the main program with the module
target_link_libraries(main_stat PRIVATE ${MODULE})

# Compile the test program
add_executable(test_stat ${TEST_DIR}/Statistics_Module_test.cpp)

# Link the test program with the module
target_link_libraries(test_stat PRIVATE ${MODULE})

# Set the output directory for the executables
set_target_properties(main_stat PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set_target_properties(test_stat PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Custom target to run the main program
add_custom_target(run-statistics
    COMMAND ${BIN_DIR}/main_stat
    DEPENDS main_stat
    WORKING_DIRECTORY ${BIN_DIR}
)

# Custom target to run the test program
add_custom_target(test-statistics
    COMMAND ${BIN_DIR}/test_stat
    DEPENDS test_stat
    WORKING_DIRECTORY ${BIN_DIR}
)

