cmake_minimum_required(VERSION 3.10)
project(Statistics_Module)

# Set the module
set(MODULE Statistics_Module)

# Compilation Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# Check dependencies
find_package(Eigen3 REQUIRED)

# Find Eigen3 package with correct config
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Add the pybind11 submodule
set(PYBIND11_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/pybind11)
#add_subdirectory(${PYBIND11_DIR} ${CMAKE_BINARY_DIR}/pybind11)

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

# Create pybind11 module
pybind11_add_module(stats_cpp ${PY_DIR}/statistics_bindings.cpp)

# Add include directories for the module
target_include_directories(stats_cpp PRIVATE
    ${INCLUDE_DIR}
    ${MODULE_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
)


# Link the pybind11 module to Eigen3
target_link_libraries(stats_cpp PRIVATE Eigen3::Eigen)



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
# Link with required libraries
target_link_libraries(stats_cpp PRIVATE 
    ${MODULE}
    Eigen3::Eigen
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


# setting Python3_EXECUTABLE
find_package(Python3 COMPONENTS Interpreter Development)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found!")
endif()


# Set Python module output directory
set(PYTHON_MODULE_DIR ${ROOT_DIR}/lib/python)
file(MAKE_DIRECTORY ${PYTHON_MODULE_DIR})

# Set the output directory for the Python module
set_target_properties(stats_cpp PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYTHON_MODULE_DIR}
    PREFIX ""
    SUFFIX ".so"
)

# Define variables for paths
set(VENV_DIR ${ROOT_DIR}/venv)
set(REQUIREMENTS_FILE ${ROOT_DIR}/stat_env/requirements.txt)
set(PYTHON_TEST_SCRIPT ${ROOT_DIR}/tests/Statistics_Test.py)
set(PYTHON_MAIN_SCRIPT ${ROOT_DIR}/main/Statistics_Module_main.py)



add_custom_target(setup-venv-stats_cpp
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
    COMMAND ${VENV_DIR}/bin/pip install --upgrade pip
    COMMAND ${VENV_DIR}/bin/pip install -r ${REQUIREMENTS_FILE}
    COMMENT "Setting up the virtual environment and installing dependencies"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Custom target for running the Python test script
add_custom_target(py-test-statistics
    COMMAND ${VENV_DIR}/bin/python ${PYTHON_TEST_SCRIPT}
    COMMENT "Running Python test script"
    WORKING_DIRECTORY ${TEST_DIR}
    DEPENDS setup-venv-stats_cpp 
)


# Custom target for running the Python main script
add_custom_target(py-run-stats_cpp
    COMMAND ${VENV_DIR}/bin/python ${PYTHON_MAIN_SCRIPT}
    COMMENT "Running Python main script"
    WORKING_DIRECTORY ${MAIN_DIR}
    DEPENDS setup-venv-stats_cpp  
)