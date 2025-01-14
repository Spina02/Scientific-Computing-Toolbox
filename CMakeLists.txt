cmake_minimum_required(VERSION 3.10)
project(ScientificComputingToolbox)

# Consenti linking in subdirectory
cmake_policy(SET CMP0079 NEW)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definisci la libreria header-only
add_library(Utilities INTERFACE)
target_include_directories(Utilities INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Aggiungi subdirectory (senza target_link_libraries)
add_subdirectory(src/Interpolation_Module)
add_subdirectory(src/ODE_Module)
add_subdirectory(src/Statistics_Module)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VENV_DIR ${ROOT_DIR}/venv)
set(REQUIREMENTS_FILE ${ROOT_DIR}/requirements.txt)

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

# Root-level py run target
add_custom_target(py-run-all
	COMMAND ${CMAKE_COMMAND} --build . --target py-run-interpolation
	COMMAND ${CMAKE_COMMAND} --build . --target py-run-ode
	COMMAND ${CMAKE_COMMAND} --build . --target py-run-statistics
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Root-level py test target
add_custom_target(py-test-all
	COMMAND ${CMAKE_COMMAND} --build . --target py-test-interpolation
	COMMAND ${CMAKE_COMMAND} --build . --target py-test-ode
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
	
macro(create_alias short long)
add_custom_target(run-${short}
	COMMAND ${CMAKE_COMMAND} --build . --target run-${long} ARGS="$ENV{ARGS}"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
add_custom_target(test-${short}
	COMMAND ${CMAKE_COMMAND} --build . --target test-${long}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
add_custom_target(py-run-${short}
	COMMAND ${CMAKE_COMMAND} --build . --target py-run-${long} ARGS="$ENV{ARGS}"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
add_custom_target(py-test-${short}
	COMMAND ${CMAKE_COMMAND} --build . --target py-test-${long}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endmacro()
create_alias(int interpolation)
create_alias(stat statistics)

# Build utilities Python module
pybind11_add_module(utilities src/python_bindings/utilities_bindings.cpp)
#target_link_libraries(utilities PRIVATE Utilities)

# Set output directory for Python modules
set_target_properties(utilities PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/ODE_Module
)

#setting Python3_EXECUTABLE
find_package(Python3 COMPONENTS Interpreter Development)
if (NOT Python3_FOUND)
	message(FATAL_ERROR "Python3 not found!")
endif()

message(STATUS "VENV_DIR=${VENV_DIR}")
message(STATUS "Python3_EXECUTABLE=${Python3_EXECUTABLE}")
message(STATUS "REQUIREMENTS_FILE=${REQUIREMENTS_FILE}")

if (NOT EXISTS ${REQUIREMENTS_FILE})
    message(FATAL_ERROR "File requirements.txt not found: ${REQUIREMENTS_FILE}")
endif()

add_custom_command(
    OUTPUT "${VENV_DIR}/bin/activate"
    COMMAND ${CMAKE_COMMAND} -E echo "Create virtual environment in ${VENV_DIR}..."
    COMMAND ${Python3_EXECUTABLE} -m venv "${VENV_DIR}"
    COMMAND "${VENV_DIR}/bin/pip" install --upgrade pip
    COMMAND "${VENV_DIR}/bin/pip" install -r "${REQUIREMENTS_FILE}"
    COMMENT "Setup del virtual environment"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(venv DEPENDS "${VENV_DIR}/bin/activate")