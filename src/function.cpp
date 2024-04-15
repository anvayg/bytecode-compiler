#include "../include/ast.hpp"

Function::Function(std::vector<std::string> params,
                   std::vector<Instruction> body, Environment env)
    : params(params), body(body), env(env) {}

std::ostream &operator<<(std::ostream &os, const Function &f) {
  os << "Function {";
  os << "params: ";
  for (const auto &param : f.params) {
    os << param << ", ";
  }
  os << "body: ";
  for (const auto &inst : f.body) {
    os << inst << ", ";
  }
  os << "env: " << f.env;
  os << "}";
  return os;

  return os;
}