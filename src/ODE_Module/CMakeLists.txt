cmake_minimum_required(VERSION 3.10)
project(ODE_Module)

# Set the module
set(MODULE ODE_Module)

# Compilation Flags
set(CXXFLAGS "-Wall -Wpedantic -fPIC -Wno-nested-anon-types -Werror")

# Check dependencies muparser and eigen3 and if not found, give a message
find_package(PkgConfig REQUIRED)
pkg_check_modules(MUPARSER REQUIRED muparser)
find_package(Eigen3 REQUIRED)

if(NOT MUPARSER_FOUND)
    message(FATAL_ERROR "muparser not found!")
endif()

if(NOT EIGEN3_FOUND)
    message(FATAL_ERROR "eigen3 not found!")
endif()

# Add the include directory for muParser
find_path(MUPARSER_INCLUDE_DIR muParser.h)
find_library(MUPARSER_LIBRARY NAMES muparser)

if (MUPARSER_INCLUDE_DIR AND MUPARSER_LIBRARY)
    message(STATUS "Found muParser: ${MUPARSER_LIBRARY}")
    include_directories(${MUPARSER_INCLUDE_DIR})
    set(MUPARSER_LIBRARIES ${MUPARSER_LIBRARY})
else()
    message(FATAL_ERROR "muParser not found")
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
set(PY_DIR ${ROOT_DIR}/python_bindings)

# Add the pybind11 module
pybind11_add_module(ODE ${PY_DIR}/ode_bindings.cpp)

target_link_libraries(ODE PRIVATE ${MODULE})

# Link the pybind11 module to muparser
target_link_libraries(ODE PRIVATE ${MUPARSER_LIBRARIES})

# Link the pybind11 module to eigen3
target_link_libraries(ODE PRIVATE ${EIGEN3_LIBRARIES})

# Set the include directories
include_directories(
    ${INCLUDE_DIR}
    ${MODULE_INCLUDE_DIR}
    ${MUPARSER_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
)

link_directories(
    ${MUPARSER_LIBRARY_DIRS}
    ${EIGEN3_LIBRARY_DIRS}
)


# Set the source files for the module
file(GLOB_RECURSE MODULE_SOURCES ${MODULE_SRC_DIR}/*.cpp)

# Create the DYNAMIC library
add_library(${MODULE} SHARED ${MODULE_SOURCES})

# Link all dependencies with keyword syntax
target_link_libraries(${MODULE} 
    PUBLIC 
        Utilities
    PRIVATE
        ${MUPARSER_LIBRARY}
        ${EIGEN3_LIBRARY}
)

# Set the output directory for the library
set_target_properties(${MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

# Create executable for the main program
add_executable(main_ode ${MAIN_DIR}/ODE_Module_main.cpp)

# Link the main program with the module
target_link_libraries(main_ode ${MODULE})

# Create executable for testing
add_executable(test_ode ${TEST_DIR}/ODE_Module_test.cpp)

# Link the test program with the module
target_link_libraries(test_ode ${MODULE})

# Specify include directories for the test and main programs
target_include_directories(main_ode PUBLIC ${INCLUDE_DIR} ${MUPARSER_INCLUDE_DIR} ${MODULE_INCLUDE_DIR})
target_include_directories(test_ode PUBLIC ${INCLUDE_DIR} ${MUPARSER_INCLUDE_DIR} ${MODULE_INCLUDE_DIR})

# Set the output directory for executables at runtime (main and test)
set_target_properties(main_ode test_ode PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Custom target for running the main program
add_custom_target(run-ode
    COMMAND ${BIN_DIR}/main_ode ${ARGS}
    DEPENDS main_ode
    WORKING_DIRECTORY ${ROOT_DIR}
    COMMENT "Running ODE Module main program"
)

# Custom target for running the test program
add_custom_target(test-ode
    COMMAND cmake --build . -j$(nproc) --target test_ode
    COMMAND test_ode
    DEPENDS test_ode
    WORKING_DIRECTORY ${BIN_DIR}
)

## setting Python3_EXECUTABLE
#find_package(Python3 COMPONENTS Interpreter Development)
#if (NOT Python3_FOUND)
#    message(FATAL_ERROR "Python3 not found!")
#endif()
#
## Define variables for paths
#set(VENV_DIR ${ROOT_DIR}/venv)
#set(REQUIREMENTS_FILE ${ROOT_DIR}/scripts/requirements.txt)
#set(PYTHON_TEST_SCRIPT ${ROOT_DIR}/tests/ODE_Test.py)
#set(PYTHON_MAIN_SCRIPT ${ROOT_DIR}/main/ODE_Module_main.py)
#
## Custom target for setting up the virtual environment
#add_custom_target(setup-venv
#    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
#    COMMAND ${VENV_DIR}/bin/pip install --upgrade pip
#    COMMAND ${VENV_DIR}/bin/pip install -r ${REQUIREMENTS_FILE}
#    COMMENT "Setting up the virtual environment and installing dependencies"
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)
#
## Custom target for running the Python test script
#add_custom_target(py-test-ode
#    COMMAND ${VENV_DIR}/bin/python ${PYTHON_TEST_SCRIPT}
#    COMMENT "Running Python test script"
#    WORKING_DIRECTORY ${TEST_DIR}
#    DEPENDS setup-venv
#)
#
## Custom target for running the Python main script
#add_custom_target(py-run-ode
#    COMMAND ${VENV_DIR}/bin/python ${PYTHON_MAIN_SCRIPT}
#    COMMENT "Running Python main script"
#    WORKING_DIRECTORY ${MAIN_DIR}
#    DEPENDS setup-venv
#)