#include "../include/function.hpp"

Function::Function(std::vector<ValueType> params, interpreter::Expression body,
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