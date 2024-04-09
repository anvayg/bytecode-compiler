#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "environment.hpp"
#include "instruction.hpp"
#include <vector>

namespace interpreter {
using Code = std::vector<Instruction>;

Code compile(Expression& exp);

// Function to compile expression into bytecode
Code compile(std::vector<ValueType> exp);

// Function to evaluate bytecode
ValueType eval(Code &bytecode, Environment &env);
} // namespace interpreter

// TODO: Add print functions for Expression and ValueType

#endif
