#include "../include/function.hpp"
#include "../include/ast.hpp"

Function::Function(std::vector<ValueType> params, Expression& body,
                   Environment &env)
    : params(params), body(body), env(env) {}

std::ostream &operator<<(std::ostream &os, const Function &f) {
  os << "lambda: (";
  // TODO
  os << ")";

  
  return os;
}