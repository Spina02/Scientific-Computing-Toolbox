# The following lines are used to define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -g

# Directories
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Modules and directories
# $(shell ls src/) but Utilities
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

run-ode:
	@$(MAKE) -C $(SRC_DIR)/ODE_Module run

run-interpolation:
	@$(MAKE) -C $(SRC_DIR)/Interpolation_Module run

.PHONY: all clean run statistics ode interpolation run-statistics run-ode run-interpolation