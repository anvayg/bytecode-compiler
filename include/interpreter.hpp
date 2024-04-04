#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "environment.hpp"
#include "instruction.hpp"
#include <vector>

namespace interpreter {
// Alias for expression and code
using Expression = std::vector<ValueType>;
using Code = std::vector<Instruction>;

// Function to compile expression into bytecode
Code compile(Expression exp);

// Function to evaluate bytecode
ValueType eval(Code &bytecode, Environment &env);
} // namespace interpreter

#endif
