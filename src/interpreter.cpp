#include <stack>
#include <stdexcept>
#include <variant>
#include "../include/instruction.hpp"
#include "../include/interpreter.hpp"

namespace interpreter {
    Code compile(Expression exp) {
        Code ins;

        if (exp.size() == 0) {
            return ins;
        }
        if (exp.size() == 1) {
            auto subexp = exp[0];
            // Check if the variant holds an int
            if (std::holds_alternative<int>(subexp)) {
                ins.push_back(Instruction(OpCode::LOAD_CONST, std::get<int>(subexp)));
            } else {
                throw std::runtime_error("Not supported");
            }
        } else if (exp.size() == 3) {
            auto first_exp = exp[0];
            if (std::holds_alternative<std::string>(first_exp)) {
                std::string first = std::get<std::string>(first_exp);
                if (first == "val") {
                    auto name = exp[1];
                    Instruction store = Instruction(OpCode::STORE_NAME, name);

                    Expression subexp(exp.begin() + 2, exp.end());
                    Code subexp_code = compile(subexp);
                    ins.insert(ins.end(), subexp_code.begin(), subexp_code.end());
                    ins.push_back(store);
                }

            } else throw std::runtime_error("Currently not supported");

        } else {
            throw std::runtime_error("Currently not supported");
        }
        
        return ins;
    }

    int eval(Code bytecode) {
        int program_counter = 0;
        std::stack<ValueType> stack;

        while (program_counter < bytecode.size()) {
            Instruction ins = bytecode[program_counter];
            auto op = ins.opCode;
            program_counter++;

            if (op == OpCode::LOAD_CONST) {
                stack.push(ins.arg);
            } else if (op == OpCode::STORE_NAME) {
                auto name = stack.top();
                stack.pop();
                

            } else {
                throw std::runtime_error("Currently not supported");
            }
        }

        return 0;
    }
}
