MODULE = Interpolation_Module

# Compiler and flags
CXX = g++
CPPFLAGS = -Iinclude/$(MODULE) -I/opt/homebrew/opt/gsl/include -I/opt/homebrew/Cellar/gsl/2.8/include
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wno-nested-anon-types -g -fPIC
LDFLAGS = -L/opt/homebrew/opt/gsl/lib

DEPS = gsl

# Collect all flags from pkg-config
PKG_CFLAGS := $(shell pkg-config --cflags $(DEPS) 2>/dev/null) 
PKG_LIBS := $(shell pkg-config --libs $(DEPS) 2>/dev/null)

ifeq ($(PKG_CFLAGS),)
    GSL_INCLUDE ?= /usr/include
    GSL_LIB ?= /usr/lib
    GSL_FLAGS = -I$(GSL_INCLUDE)
    GSL_LIBS = -L$(GSL_LIB) -lgsl -lgslcblas 

    PKG_CFLAGS = $(GSL_FLAGS)
    PKG_LIBS = $(GSL_LIBS)
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
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test files and object files
TESTS_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(TESTS_SRCS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Library and main program targets
LIB_NAME = lib$(MODULE).so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
MAIN_TARGET = $(BIN_DIR)/$(MODULE)_main
TEST_TARGET = $(BIN_DIR)/$(MODULE)_test

# Default target
all: $(LIB_TARGET) $(MAIN_TARGET) $(TEST_TARGET)

# Compiling the shared library
$(LIB_TARGET): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $^ -o $@ $(LDFLAGS)

# Compiling the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compiling the test object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compiling the main program for testing
$(MAIN_TARGET): $(MAIN_DIR)/$(MODULE)_main.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -L$(LIB_DIR) -l$(MODULE)  -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_DIR)/$(MODULE)_test.cpp $(LIB_TARGET)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -L$(LIB_DIR) -l$(MODULE)  -o $(BIN_DIR)/$(MODULE)_test $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Export library path when running the program
run: $(MAIN_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(MAIN_TARGET) $(ARGS)

test: $(TEST_TARGET) $(LIB_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(TEST_TARGET) 

.PHONY: all clean run test