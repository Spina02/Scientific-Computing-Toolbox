# The following lines are used to define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror 

# Directories
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Modules and directories
MODULES = Interpolation_Module ODE_Module Statistics_Module

all: $(MODULES)

# Compile each module
$(MODULES):
	@$(MAKE) -C $(SRC_DIR)/$@

# Rules for multiple targets
run clean test:
	@if [ -n "$(MAKECMDGOALS)" ] && [ "$(word 2,$(MAKECMDGOALS))" != "" ]; then \
		case "$(word 2,$(MAKECMDGOALS))" in \
			ode) module="ODE_Module" ;; \
			statistics) module="Statistics_Module" ;; \
			interpolation) module="Interpolation_Module" ;; \
		esac; \
		$(MAKE) -C $(SRC_DIR)/$$module $@ -s; \
	else \
		for module in $(MODULES); do \
			$(MAKE) -C $(SRC_DIR)/$$module $@ -s; \
		done; \
	fi

# Targets for individual modules
ode statistics interpolation:
	@:

.PHONY: all clean run test $(MODULES) ode statistics interpolation