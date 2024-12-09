MODULE = Statistics_Module

# Compiler and flags
CXX = g++
CPPFLAGS = -Iinclude/$(MODULE)
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Werror -fPIC

# Dependencies configuration
DEPS = eigen3

# Collect all flags from pkg-config
PKG_CFLAGS := $(shell pkg-config --cflags $(DEPS) 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs $(DEPS) 2>/dev/null)
GETPOT_CXXFLAGS = -Wno-unused-variable -Wno-sign-compare
	
# Fallback paths if pkg-config fails
ifeq ($(PKG_CFLAGS),)

	# Eigen fallback
	EIGEN_INCLUDE ?= /usr/include/eigen3
	EIGEN_FLAGS = -I$(EIGEN_INCLUDE)

	# Combine all flags
	PKG_CFLAGS = $(EIGEN_FLAGS)
endif

CPPFLAGS += $(PKG_CFLAGS)
LDFLAGS += $(PKG_LIBS)

# Relative paths
SRC_DIR = ../../src/$(MODULE)
MAIN_DIR = ../../main
TEST_DIR = ../../tests
BUILD_DIR = ../../build/$(MODULE)
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Library and main program targets
LIB_NAME = lib$(MODULE).so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
MAIN_TARGET = $(BIN_DIR)/$(MODULE)_main

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

# Compiling the shared library
$(LIB_TARGET): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $^ -o $@ $(LDFLAGS)

# Compiling the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@;

# Compiling the main program for testing
$(MAIN_TARGET): $(MAIN_DIR)/$(MODULE)_main.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB_DIR) -l$(MODULE) $< -o $@ $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Export library path when running the program
run: $(MAIN_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(MAIN_TARGET) 
.PHONY: all clean run check_deps
