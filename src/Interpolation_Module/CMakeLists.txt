cmake_minimum_required(VERSION 3.10)
project(Interpolation_Module)

# Set the module
set(MODULE Interpolation_Module)

# Compilation Flags
set(CXXFLAGS "-Wall -Wextra -Wpedantic -Werror -Wno-nested-anon-types -g -fPIC")



# Find GSL
find_package(GSL REQUIRED)
if (NOT GSL_FOUND)
    message(FATAL_ERROR "GSL not found!")
endif()


# Add the pybind11 submodule
set(PYBIND11_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/pybind11)
add_subdirectory(${PYBIND11_DIR} ${CMAKE_BINARY_DIR}/pybind11)  # Specify binary directory explicitly

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
set(PY_DIR ${ROOT_DIR}/python_bindings)

# Add the pybind11 module
pybind11_add_module(interpolation ${PY_DIR}/interpolation_bindings.cpp)

target_link_libraries(interpolation PRIVATE ${MODULE})

# Link the pybind11 module to GSL
target_link_libraries(interpolation PRIVATE GSL::gsl)

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
target_link_libraries(${MODULE} GSL::gsl)

# Set the output directory for the library
set_target_properties(${MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

# Create executable for the main program
add_executable(main_interpolation ${MAIN_DIR}/Interpolation_Module_main.cpp)

# Link the main program with the module
target_link_libraries(main_interpolation ${MODULE})

# Create executable for testing
add_executable(test_interpolation ${TEST_DIR}/Interpolation_Module_test.cpp)

# Link the test program with the module
target_link_libraries(test_interpolation ${MODULE})

# Specify include directories for the test and main programs
target_include_directories(main_interpolation PUBLIC ${INCLUDE_DIR} ${MODULE_INCLUDE_DIR})
target_include_directories(test_interpolation PUBLIC ${INCLUDE_DIR} ${MODULE_INCLUDE_DIR})

# Set the output directory for executables at runtime (main and test)
set_target_properties(main_interpolation test_interpolation PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Read from environment variables if set
set(DATA_FILE $ENV{DATA_FILE} CACHE STRING "Path to the data file")
set(VALUE $ENV{VALUE} CACHE STRING "Value to interpolate")

# Custom targets
add_custom_target(run-interpolation
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} -j$(nproc) --target main_interpolation
    COMMAND ${BIN_DIR}/main_interpolation ${ARGS}
    DEPENDS main_interpolation
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Interpolation Module main program"
)

# Custom target for running the test program
add_custom_target(test-interpolation
    COMMAND cmake --build . -j$(nproc) --target test_interpolation
    COMMAND ${BIN_DIR}/test_interpolation
    DEPENDS test_interpolation
    WORKING_DIRECTORY ${BIN_DIR}
)

add_custom_target(bind-interpolation
    DEPENDS interpolation
    WORKING_DIRECTORY ${BIN_DIR}
)

# setting Python3_EXECUTABLE
find_package(Python3 COMPONENTS Interpreter Development)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found!")
endif()

# Define variables for paths
set(VENV_DIR ${ROOT_DIR}/venv)
set(REQUIREMENTS_FILE ${ROOT_DIR}/scripts/requirements.txt)
set(PYTHON_TEST_SCRIPT ${ROOT_DIR}/tests/Interpolation_Test.py)
set(PYTHON_MAIN_SCRIPT ${ROOT_DIR}/main/Interpolation_Module_main.py)

# Custom target for setting up the virtual environment
add_custom_target(setup-venv
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
    COMMAND ${VENV_DIR}/bin/pip install --upgrade pip
    COMMAND ${VENV_DIR}/bin/pip install -r ${REQUIREMENTS_FILE}
    COMMENT "Setting up the virtual environment and installing dependencies"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Custom target for running the Python test script
add_custom_target(py-test-interpolation
    COMMAND ${VENV_DIR}/bin/python ${PYTHON_TEST_SCRIPT}
    COMMENT "Running Python test script"
    WORKING_DIRECTORY ${TEST_DIR}
    DEPENDS setup-venv
)

# Custom target for running the Python main script
add_custom_target(py-run-interpolation
    COMMAND ${VENV_DIR}/bin/python ${PYTHON_MAIN_SCRIPT}
    COMMENT "Running Python main script"
    WORKING_DIRECTORY ${MAIN_DIR}
    DEPENDS setup-venv
)

