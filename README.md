### lisp-cpp

----

## Introduction

This is an under-development implementation of a bytecode compiler in C++ for a 
subset of Lisp. I'm adapting a similar compiler implemented in Python, 
described in this [blog post](https://bernsteinbear.com/blog/bytecode-interpreters/) 
by Michael Bernstein. This is an educational project so that I can learn about 
bytecode compilers/interpreters and develop greater proficiency with C++.

The compiler currently consists of two phases:

1. A bytecode compilation phase that compiles the Lisp AST to
a simple Instruction Set Architecture (ISA), consisting of the following instructions:
    
    - LOAD_CONST: Pushes constants onto the stack.
    - STORE_NAME: Stores values into the environment.
    - LOAD_NAME: Reads values from the environment.
    - CALL_FUNCTION: Calls a function.
    - RELATIVE_JUMP_IF_TRUE: Jumps if the value on top of the stack is true.
    - RELATIVE_JUMP: Jumps.
    - MAKE_FUNCTION: Creates a function object from a code object on the stack and pushes it on the stack.

2. An interpretration phase where the bytecode is evaluated using a stack-based virtual machine.

The compiler currently lacks a frontend, which is hopefully soon to come. Build and run instructions also in the works.

## Installation

#### Dependencies

To build this project you need a C++ compiler (clang or gcc) and the 
C++ Boost library, which is used for its testing framework and for 
writing recursive variants. Using the recursive variants only requires the Boost header files, but the testing framework requires dynamic linking to the `boost_unit_test_framework`.

I installed Boost using homebrew on MacOS. Alternatively, you can follow the installation instructions on getting started with Boost [here](https://www.boost.org/doc/libs/1_85_0/more/getting_started/index.html).


#### Building the executable

Currently the only way to work with the compiler is by writing test expressions and then compiling/evaluating them, as in test/compiler-test.cpp. To build the test executable, the Makefile needs to be modified to include your compiler path. The argument to the 
`-I` flag should also be modified to point to where the `include` directory of the Boost library lives, and so should the argument to
the `-L` flag to point to directory where the Boost dynamic libraries live.

Once this is done, running `make` will build the test executable.

If you are using VSCode and `clangd` (as I have been for this project), then an easy way to configure the project such that `clangd`
can find the Boost library is to create a `compile_flags.txt` file
that specifies the path to the Boost library which needs to be dynamically linked using the `-L` flag and the Boost header files that need to be included using the `-I` flag (similar to what is done in the Makefile).

## Roadmap

**Upcoming features**

- Compiling `define`
- Lexer and parser
