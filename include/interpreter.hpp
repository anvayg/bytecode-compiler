#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "instruction.hpp"
#include <vector>
#include <variant>

namespace interpreter {
    // Alias for expression and code
    using ArgType = std::variant<int, std::string>;
    using Expression = std::vector<ArgType>;
    using Code = std::vector<Instruction>;

    // Function to compile expression into bytecode
    Code compile(Expression exp);

    // Function to evaluate bytecode
    int eval(Code bytecode);
}

#endif
