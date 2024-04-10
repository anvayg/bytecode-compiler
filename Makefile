TESTDIR = test            # Directory containing test source files
BUILDDIR = build          # Directory to place object files
TEST_TARGET = test_exec        # Name of the test executable

# Compiler (your compiler path)
CXX = /opt/homebrew/opt/llvm@17/bin/clang++

# Include directories
INCLUDE_DIRS = -Iinclude/
TEST_INCLUDE_DIRS = -I/opt/homebrew/Cellar/boost/1.84.0_1/include/

# Link libraries
TEST_FLAGS = -L/opt/homebrew/Cellar/boost/1.84.0_1/lib -l boost_unit_test_framework

# Source files
SRCS = src/interpreter.cpp src/instruction.cpp src/environment.cpp src/ast.cpp

# List of test source files
TEST_SRCS = test/compiler-test.cpp

# Object files
OBJS = $(addprefix build/, $(notdir $(SRCS:.cpp=.o)))

# List of test object files (one for each test source file)
TEST_OBJS = $(addprefix build/, $(notdir $(TEST_SRCS:.cpp=.o)))

# Executable name
EXEC = build/interp

# Default target
all: $(EXEC)

# Rule to compile object files
build/%.o: src/%.cpp
	$(CXX) $(INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) -c $< -o $@

# Rule to link object files into executable
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

# Rule to build object files from test source files
build/%.o: test/%.cpp
	$(CXX) $(TEST_INCLUDE_DIRS) -c $< -o $@

# Rule to build the test executable
test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(TEST_FLAGS) $(TEST_INCLUDE_DIRS) $^ -o $(TEST_TARGET)

# Clean target
clean:
	rm -rf build/*
	rm $(TEST_TARGET)

.PHONY: clean test
