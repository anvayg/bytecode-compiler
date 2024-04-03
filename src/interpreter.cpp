#include <stack>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>
#include "../include/instruction.hpp"
#include "../include/interpreter.hpp"

namespace interpreter {
    Code compile(Expression exp) {
        Code ins;
        if (exp.size() == 1) {
            auto subexp = exp[0];
            // Check if the variant holds an int
            if (std::holds_alternative<int>(subexp)) {
                ins.push_back(Instruction(OpCode::LOAD_CONST, std::get<int>(subexp)));
            } else {
                throw std::logic_error("Not supported");
            }
        } else {
            throw std::runtime_error("Currently not supported");
        }
        return ins;
    }

    int eval(Code bytecode) {
        int program_counter = 0;
        std::stack<ArgType> stack;

        while (program_counter < bytecode.size()) {
            Instruction ins = bytecode[program_counter];
            auto op = ins.opCode;
            program_counter++;

            if (op == OpCode::LOAD_CONST) {
                stack.push(ins.arg);
            } else {
                throw std::runtime_error("Currently not supported");
            }
        }

        return 0;
    }
}
