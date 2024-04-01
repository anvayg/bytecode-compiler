# Compiler (your compiler path)
CXX = /opt/homebrew/opt/llvm@17/bin/clang++

# Source files
SRCS = src/interpreter.cpp src/instruction.cpp src/environment.cpp

# Object files
OBJS = $(addprefix build/, $(notdir $(SRCS:.cpp=.o)))

# Executable name
EXEC = build/interp

# Default target
all: $(EXEC)

# Rule to compile object files
build/%.o: src/%.cpp
	$(CXX) -I include/ -c $< -o $@

# Rule to link object files into executable
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

# Clean target
clean:
	rm -rf build/*
