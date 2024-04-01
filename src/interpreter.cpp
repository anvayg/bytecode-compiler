#include <stack>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>
#include "../include/instruction.h"

namespace interpreter {
    using Expression = std::vector<ArgType>;
    using Code = std::vector<Instruction>;

    Code compile(Expression exp) {
        if (exp.size() == 1) {
            auto subexp = exp[0];
            // Define the visitor
            auto visitor = [](auto&& arg) -> std::vector<Instruction> {
                if constexpr (std::is_same_v<decltype(arg), int>) {
                    Code ins;
                    ins.push_back(Instruction(OpCode::LOAD_CONST, 5));
                } else {
                    throw std::logic_error("Not supported");
                }
            };
            return std::visit(visitor, subexp);

        } else {
            throw std::runtime_error("Currently not supported");
        }
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
