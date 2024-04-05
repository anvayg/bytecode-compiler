#include "../include/interpreter.hpp"
#include "../include/environment.hpp"
#include "../include/instruction.hpp"
#include <stack>
#include <stdexcept>
#include <variant>

interpreter::Code interpreter::compile(Expression exp) {
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
      ins.push_back(
          Instruction(OpCode::LOAD_NAME, std::get<std::string>(subexp)));
    }
  } else if (exp.size() == 3) {
    // [val, x, 5]
    auto first_exp = exp[0];
    if (std::holds_alternative<std::string>(first_exp)) {
      std::string first = std::get<std::string>(first_exp);
      if (first == "val") {
        auto name = exp[1];
        Instruction store(OpCode::STORE_NAME, name);

        Expression subexp(exp.begin() + 2, exp.end());
        Code subexp_code = compile(subexp);
        ins.insert(ins.end(), subexp_code.begin(), subexp_code.end());
        ins.push_back(store);
      } else
        throw std::runtime_error("Unsupported instruction");
    }
  } else if (exp.size() == 4) {
    auto first_exp = exp[0];
    if (std::holds_alternative<std::string>(first_exp)) {
      std::string first = std::get<std::string>(first_exp);
      if (first == "if") {
        // Compile condition, true and false branches
        Expression cond(exp.begin() + 1, exp.begin() + 2);
        Code cond_code = compile(cond);

        Expression true_exp(exp.begin() + 2, exp.begin() + 3);
        Code true_code = compile(true_exp);

        Expression false_exp(exp.begin() + 3, exp.begin() + 4);
        Code false_code = compile(false_exp);

        // Construct relative jumps
        Instruction jmp_to_end(OpCode::RELATIVE_JUMP,
                               static_cast<int>(true_code.size()));
        Instruction jmp_to_true(OpCode::RELATIVE_JUMP_IF_TRUE,
                                static_cast<int>(false_code.size()) + 1);

        // Put instructions together
        ins.insert(ins.end(), cond_code.begin(), cond_code.end());
        ins.push_back(jmp_to_true);
        ins.insert(ins.end(), false_code.begin(), false_code.end());
        ins.push_back(jmp_to_end);
        ins.insert(ins.end(), true_code.begin(), true_code.end());

      } else
        throw std::runtime_error("Unsupported instruction");
    } else
      throw std::runtime_error("Unsupported instruction");
  } else
    throw std::runtime_error("Unsupported supported");

  return ins;
}

ValueType interpreter::eval(Code &bytecode, Environment &env) {
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
        throw std::runtime_error("Unsupported instruction");
      }
    } else if (op == OpCode::STORE_NAME) {
      // Get name from top of stack and add a binding for it in the
      // environment
      auto name = stack.top();
      stack.pop();

      if (std::holds_alternative<int>(name) &&
          std::holds_alternative<std::string>(ins.arg)) {
        env.define(std::get<std::string>(ins.arg), std::get<int>(name));
      } else {
        throw std::runtime_error("Unsupported instruction");
      }
    } else if (op == OpCode::RELATIVE_JUMP_IF_TRUE) {
        auto cond = stack.top();
        stack.pop();

        if(std::get<int>(cond)) program_counter += std::get<int>(ins.arg);
        
    } else if (op == OpCode::RELATIVE_JUMP) {
        program_counter += std::get<int>(ins.arg);
    } else {
      throw std::runtime_error("Unsupported instruction");
    }
  }

  if (!stack.empty())
    return stack.top();
  else
    return -1;
}
