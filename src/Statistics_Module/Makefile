MODULE = Statistics_Module

# Compiler and flags
CXX = g++
CPPFLAGS = -Iinclude/$(MODULE) -I/opt/homebrew/opt/eigen/include/eigen3
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Werror

# Dependencies configuration
DEPS = eigen3

# Collect all flags from pkg-config
PKG_CFLAGS := $(shell pkg-config --cflags $(DEPS) 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs $(DEPS) 2>/dev/null)

# Fallback paths if pkg-config fails
ifeq ($(PKG_CFLAGS),)
	# Eigen fallback
	EIGEN_INCLUDE ?= /usr/include/eigen3
	EIGEN_FLAGS = -I$(EIGEN_INCLUDE)

	# Combine all flags
	PKG_CFLAGS = $(EIGEN_FLAGS)
endif

CPPFLAGS += $(PKG_CFLAGS)

# Relative paths
SRC_DIR = ../../src/$(MODULE)
MAIN_DIR = ../../main
TEST_DIR = ../../tests
BUILD_DIR = ../../build/$(MODULE)
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Library and main program targets
LIB_NAME = lib$(MODULE).a  # Static library
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
MAIN_TARGET = $(BIN_DIR)/$(MODULE)_main
TEST_TARGET = $(BIN_DIR)/$(MODULE)_test

# Default target
all: check_deps $(LIB_TARGET) $(MAIN_TARGET)

# Check for all dependencies
check_deps:
	@for dep in $(DEPS); do \
		if ! pkg-config --exists $$dep 2>/dev/null; then \
			echo ""; \
			echo "Error: $$dep not found."; \
			echo "Please install it with one of:"; \
			case $$dep in \
				"eigen3") \
					echo "  sudo apt-get install libeigen3-dev     # Debian/Ubuntu"; \
					echo "  sudo pacman -S eigen                   # Arch Linux"; \
					echo "  sudo dnf install eigen3-devel          # Fedora"; \
					echo "  brew install eigen                     # macOS";; \
			esac; \
			echo ""; \
			exit 1; \
		fi; \
	done

# Header-only library: No actual compilation needed
$(LIB_TARGET):
	@mkdir -p $(LIB_DIR)
	@echo "Header-only library. No compilation required."

# Compiling the main program for testing (no -fPIC needed here)
$(MAIN_TARGET): $(MAIN_DIR)/$(MODULE)_main.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(PKG_LIBS)

$(TEST_TARGET): $(TEST_DIR)/$(MODULE)_test.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $(BIN_DIR)/$(MODULE)_test $(PKG_LIBS)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Export library path when running the program
run: $(MAIN_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(MAIN_TARGET) $(ARGS)

test: $(TEST_TARGET) $(LIB_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(TEST_TARGET)

.PHONY: all clean run check_deps test
