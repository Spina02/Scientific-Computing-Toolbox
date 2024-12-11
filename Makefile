# The following lines are used to define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror 

# Directories
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Modules and directories
MODULES = $(filter-out Utilities, $(shell ls src/))
MODULE_DIRS = $(addprefix $(SRC_DIR)/, $(MODULES))

all: $(MODULES)

# Compile each module
$(MODULES):
	@$(MAKE) -C $(SRC_DIR)/$@

statistics: Statistics_Module
ode: ODE_Module
interpolation: Interpolation_Module

clean:
	@for module in $(MODULES); do \
		$(MAKE) -C $(SRC_DIR)/$$module clean; \
	done

run:
	@for module in $(MODULES); do \
		$(MAKE) -C $(SRC_DIR)/$$module run; \
	done

run-statistics:
	@$(MAKE) -C $(SRC_DIR)/Statistics_Module run

test-statistics:
	@$(MAKE) -C $(SRC_DIR)/Statistics_Module test

run-ode:
	@$(MAKE) -C $(SRC_DIR)/ODE_Module run

run-interpolation:
	@$(MAKE) -C $(SRC_DIR)/Interpolation_Module run

test-interpolation:
	@$(MAKE) -C $(SRC_DIR)/Interpolation_Module test


.PHONY: all clean run statistics ode interpolation run-statistics run-ode run-interpolation