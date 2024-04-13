# bytecode-compiler

This is an under-development implementation of a bytecode compiler in C++ for a 
subset of Lisp. I'm adapting a similar compiler implemented in Python, 
described in this blog post [blog post](https://bernsteinbear.com/blog/bytecode-interpreters/) 
by Michael Bernstein. This is an educational project so that I can learn about 
bytecode compilers/interpreters and become more proficient with C++.

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