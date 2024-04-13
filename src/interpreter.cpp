#include "../include/interpreter.hpp"
#include "../include/ast.hpp"
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

interpreter::Code interpreter::compile(Expression &e) {
  Compiler compiler;
  return e.accept(compiler);
}

std::vector<Instruction> Compiler::visit(Constant &constant) {
  std::vector<Instruction> ins;
  ins.push_back(Instruction(OpCode::LOAD_CONST, constant.getValue()));
  return ins;
}

std::vector<Instruction> Compiler::visit(StringConstant &constant) {
  std::vector<Instruction> ins;
  ins.push_back(Instruction(OpCode::LOAD_NAME, constant.getValue()));
  return ins;
}

std::vector<Instruction> Compiler::visit(BinaryOperation &binOp) {
  std::vector<Instruction> ins;

  return ins;
}

std::vector<Instruction> Compiler::visit(ExpressionList &list) {
  std::vector<Instruction> ins;
  const auto &exps = list.getExpressions();

  if (exps.size() == 3) {
    auto &first = exps[0];
    const StringConstant *strConstPtr =
        dynamic_cast<const StringConstant *>(first.get());
    if (strConstPtr && strConstPtr->getValue() == "val") {
      auto &name = exps[1];
      const StringConstant *strConstPtr =
          dynamic_cast<const StringConstant *>(name.get());

      if (strConstPtr) {
        Instruction store(OpCode::STORE_NAME, strConstPtr->getValue());

        auto &subexp = exps[2];
        std::vector<Instruction> subexp_code = subexp.get()->accept(*this);

        ins.insert(ins.end(), subexp_code.begin(), subexp_code.end());
        ins.push_back(store);
      } else {
        throw std::runtime_error("Unsupported instruction");
      }

    } else if (strConstPtr && strConstPtr->getValue() == "lambda") {
      auto &params = exps[1];
      const ExpressionList *listPtr =
          dynamic_cast<const ExpressionList *>(std::move(params.get()));
      if (!listPtr) throw std::runtime_error("Invalid params");
      else {
        // Make list of parameters;
        std::vector<std::string> paramStrings;
        for (const auto& ptr : listPtr->getExpressions()) {
          const StringConstant *strConstPtr =
          dynamic_cast<const StringConstant *>(ptr.get());

          if (!strConstPtr) throw std::runtime_error("Invalid param");
          else paramStrings.push_back(strConstPtr->getValue());
        }
        // Instruction for loading parameters
        Instruction load_params(OpCode::LOAD_CONST, paramStrings);

        // Instruction for loading body
        auto &body = exps[2];
        std::vector<Instruction> body_ins = body.get()->accept(*this);
        Instruction load_body(OpCode::LOAD_CONST, body_ins);

        Instruction mk_function(OpCode::MAKE_FUNCTION, 1);

        // Put instructions together
        ins.push_back(load_params);
        ins.push_back(load_body);
        ins.push_back(mk_function);
      }
    } else {
      throw std::runtime_error("Unsupported instruction");
    }

  } else if (exps.size() == 4) {
    auto &first = exps[0];
    const StringConstant *strConstPtr =
        dynamic_cast<const StringConstant *>(first.get());
    if (strConstPtr && strConstPtr->getValue() == "if") {
      // Compile condition, true and false branches
      auto &cond = exps[1];
      std::vector<Instruction> cond_code = cond.get()->accept(*this);

      auto &true_branch = exps[2];
      std::vector<Instruction> true_code = true_branch.get()->accept(*this);

      auto &false_branch = exps[3];
      std::vector<Instruction> false_code = false_branch.get()->accept(*this);

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

    } else {
      throw std::runtime_error("Unsupported instruction");
    }

  } else {
    throw std::runtime_error("Unsupported instruction");
  }

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
      if (ins.arg.type() == typeid(std::string)) {
        auto val = env.lookup(boost::get<std::string>(ins.arg));
        stack.push(val);
      } else {
        throw std::runtime_error("Unsupported instruction");
      }
    } else if (op == OpCode::STORE_NAME) {
      // Get name from top of stack and add a binding for it in the
      // environment
      auto name = stack.top();
      stack.pop();

      if (ins.arg.type() == typeid(std::string)) {
        env.define(boost::get<std::string>(ins.arg), name);
      } else {
        throw std::runtime_error("Unsupported instruction");
      }
    } else if (op == OpCode::RELATIVE_JUMP_IF_TRUE) {
      auto cond = stack.top();
      stack.pop();

      if (boost::get<int>(cond))
        program_counter += boost::get<int>(ins.arg);

    } else if (op == OpCode::RELATIVE_JUMP) {
      program_counter += boost::get<int>(ins.arg);
    } else if (op == OpCode::MAKE_FUNCTION) {
      int nargs = boost::get<int>(ins.arg);
      std::vector<Instruction> body_code = boost::get<std::vector<Instruction>>(stack.top());
      stack.pop();
      std::vector<std::string> params = boost::get<std::vector<std::string>>(stack.top());
      stack.pop();
      
      Function f(params, body_code, env);
      stack.push(std::make_shared<Function>(f));
    } else {
      throw std::runtime_error("Unsupported instruction");
    }
  }

  if (!stack.empty())
    return stack.top();
  else
    return -1;
}
