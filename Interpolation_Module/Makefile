CXX = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Werror

# Paths
SRC_DIR = src
TEST_DIR = test
LIB_DIR = lib
BUILD_DIR = build
LIB_NAME = libInterpolation_Module.so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
MAIN_TARGET = $(BUILD_DIR)/main

# Default target
all: $(MAIN_TARGET)

# Compiling the shared library
$(LIB_TARGET): $(SRC_DIR)/Interpolation_Module.cpp
	@mkdir -p $(LIB_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -fPIC $^ -o $@

# Compiling the main program
$(MAIN_TARGET): $(TEST_DIR)/main.cpp $(LIB_TARGET)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(LIB_DIR) -lInterpolation_Module $< -o $@

# Clean up build artifacts
clean:
	rm -f $(MAIN_TARGET) $(LIB_TARGET)

# Export library path when running the program
run: $(MAIN_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(MAIN_TARGET)

.PHONY: all clean run
