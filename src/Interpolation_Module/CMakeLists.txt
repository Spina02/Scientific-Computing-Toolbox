cmake_minimum_required(VERSION 3.10)
project(Interpolation_Module)

# Set the module
set(MODULE Interpolation_Module)

# Compilation Flags
set(CXXFLAGS "-Wall -Wextra -Wpedantic -Werror -Wno-nested-anon-types -g -fPIC")

# Check dependencies and if not found, give a message
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSL REQUIRED gsl)

if(NOT GSL_FOUND)
    message(FATAL_ERROR "GSL not found!")
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

# Explicitly set the gsl include directory
set(GSL_LIB_DIR "/opt/homebrew/opt/gsl/lib") # Adjust based on your installation path
set(GSL_INCLUDE_DIR "/opt/homebrew/opt/gsl/include") # Adjust based on your installation path

# Set the include directories
include_directories(
    ${INCLUDE_DIR}
    ${MODULE_INCLUDE_DIR}
    ${GSL_INCLUDE_DIR}
)

# Set the source files for the module
file(GLOB_RECURSE MODULE_SOURCES ${MODULE_SRC_DIR}/*.cpp)

# Create the DYNAMIC library
add_library(${MODULE} SHARED ${MODULE_SOURCES})

# Link the module with GSL
target_link_libraries(${MODULE} ${GSL_LIB_DIR}/libgsl.dylib ${GSL_LIB_DIR}/libgslcblas.dylib)

# Set the output directory for the library
set_target_properties(${MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

# Create executable for the main program
add_executable(main ${MAIN_DIR}/Interpolation_Module_main.cpp)

# Link the main program with the module
target_link_libraries(main ${MODULE})

# Create executable for testing
add_executable(test ${TEST_DIR}/Interpolation_Module_test.cpp)

# Link the test program with the module
target_link_libraries(test ${MODULE})

# Specify include directories for the test and main programs
target_include_directories(main PUBLIC ${INCLUDE_DIR} ${GSL_INCLUDE_DIR} ${MODULE_INCLUDE_DIR})
target_include_directories(test PUBLIC ${INCLUDE_DIR} ${GSL_INCLUDE_DIR} ${MODULE_INCLUDE_DIR})

# Set the output directory for executables at runtime (main and test)
set_target_properties(main test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Custom target for running the main program
add_custom_target(run_main
    COMMAND ${BIN_DIR}/main
    DEPENDS main
    WORKING_DIRECTORY ${BIN_DIR}
)

# Custom target for running the test program
add_custom_target(run_test
    COMMAND ${BIN_DIR}/test
    DEPENDS test
    WORKING_DIRECTORY ${BIN_DIR}
)

# Custom target for cleaning build artifacts
add_custom_target(clean_interpolation
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIB_DIR}
    COMMAND ${CMAKE_COMMAND} -E rm -rf $(CMAKE_BINARY_DIR)/*
)
