# The following lines are used to define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror 

# Directories
#SRC_DIR = src
#OBJ_DIR = build
#INC_DIR = include

# Modules and directories
MODULES = Interpolation_Module ODE_Module Statistics_Module

all: 
	@mkdir -p build
	@cd build && cmake .. && $(MAKE) -j$(nproc)
	@cd .. 
	@pip install .

# Help target
help:
	@echo "Available commands:"
	@echo "  make                          # Build all modules"
	@echo "  make <module>                 # Build specific module"
	@echo "  make run                      # Run all modules"
	@echo "  make run <module> <args>      # Run specific module"
	@echo "  make test                     # Run all modules tests"
	@echo "  make test <module>            # Run specific module tests"
	@echo "  make clean                    # Clean all modules build files"
	@echo "  make clean <module>           # Clean specific module build files"
	@echo "  make help                     # Show this help"
	@echo ""
	@echo "Modules:"
	@echo "  interpolation                 # Interpolation module"
	@echo "    Usage: make run interpolation <input_file> <x_value_to_interpolate>"
	@echo "    Example: make run interpolation data/cubic_data.csv 3.5"
	@echo ""
	@echo "  ode                           # ODE solver module"
	@echo "    Usage: make run ode <input_file> <output_file>"
	@echo "    Example: make run ode data/ode_tests.csv output/ODE_output.txt"
	@echo ""
	@echo "  statistics                    # Statistics module"
	@echo "    Usage: make run statistics <input_file> <output_file> <column(s)>"
	@echo "    Example: make run statistics data/Food_and_Nutrition__.csv output/Statistics_output.txt Weight Ages"

# Compile each module
$(MODULES):
	@$(MAKE) -C build 


# Rules for multiple targets
run test py-run py-test:
	@if [ -n "$(MAKECMDGOALS)" ] && [ "$(word 2,$(MAKECMDGOALS))" != "" ]; then \
		module="$(word 2,$(MAKECMDGOALS))"; \
		args="$(wordlist 3,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))"; \
		$(MAKE) -C build $@-$$module ARGS="$$args" -s; \
	else \
		$(MAKE) -C build $@-all -s; \
	fi

clean:
	@if [ -n "$(word 2,$(MAKECMDGOALS))" ]; then \
		module="$(word 2,$(MAKECMDGOALS))"; \
		rm -rf build/src/$$module/*; \
	else \
		rm -rf build/* lib/* bin/* output/* scientific_toolbox.egg-info; \
	fi

# Targets for individual modules
ode statistics interpolation:
	@:

# Allow arbitrary targets (for arguments)
%:
	@:

.PHONY: all clean run test $(MODULES) ode statistics interpolation help