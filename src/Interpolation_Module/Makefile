# Compiler and flags
CXX = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Werror -fPIC

# Relative paths
MODULE = Interpolation_Module
SRC_DIR = ../../src/$(MODULE)
TEST_DIR = ../../tests
BUILD_DIR = ../../build/$(MODULE)
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source files and object files
SRCS = $(SRC_DIR)/$(MODULE).cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Library and main program targets
LIB_NAME = lib$(MODULE).so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
MAIN_TARGET = $(BIN_DIR)/$(MODULE)_main

# Default target
all: $(LIB_TARGET) $(MAIN_TARGET)

# Compiling the shared library
$(LIB_TARGET): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $^ -o $@

# Compiling the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compiling the main program for testing
$(MAIN_TARGET): $(TEST_DIR)/$(MODULE)_main.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB_DIR) -l$(MODULE) $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Export library path when running the program
run: $(MAIN_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(MAIN_TARGET)

.PHONY: all clean run
