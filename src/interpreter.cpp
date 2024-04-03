#include <stack>
#include <stdexcept>
#include <variant>
#include "../include/instruction.hpp"
#include "../include/interpreter.hpp"
#include "../include/environment.hpp"

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
                ins.push_back(Instruction(OpCode::LOAD_NAME, std::get<std::string>(subexp)));
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

    ValueType eval(Code& bytecode, Environment& env) {
        int program_counter = 0;
        std::stack<ValueType> stack;

        while (program_counter < bytecode.size()) {
            Instruction ins = bytecode[program_counter];
            auto op = ins.opCode;
            program_counter++;

            if (op == OpCode::LOAD_CONST) {
                stack.push(ins.arg);
            } else if (op == OpCode::LOAD_NAME) {
                // Find name in environment and push corresponding value onto stack
                if (std::holds_alternative<std::string>(ins.arg)) {
                    auto val = env.lookup(std::get<std::string>(ins.arg));
                    stack.push(val);
                } else {
                    throw std::runtime_error("Currently not supported");
                }

            } else if (op == OpCode::STORE_NAME) {
                // Get name from top of stack and add a binding for it in the environment
                auto name = stack.top();
                stack.pop();

                if (std::holds_alternative<std::string>(name)) {
                    env.define(std::get<std::string>(name), ins.arg);
                } else {
                    throw std::runtime_error("Currently not supported");
                }
            } else {
                throw std::runtime_error("Currently not supported");
            }
        }

        if (!stack.empty()) return stack.top();
        else return -1;
    }
}
