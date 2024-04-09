#include "../include/function.hpp"
#include "../include/ast.hpp"

Function::Function(std::vector<ValueType> params, Expression& body,
                   Environment &env)
    : params(params), body(body), env(env) {}

std::ostream &operator<<(std::ostream &os, const Function &f) {
  os << "lambda: (";
  for (auto param : f.params) {
    std::visit([](const auto &val) { std::cout << val; }, param);
  }
  os << ")";

  

  return os;
}