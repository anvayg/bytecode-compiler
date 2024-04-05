#ifndef FUNCTION_H
#define FUNCTION_H

#include "environment.hpp"
#include "instruction.hpp"
#include "interpreter.hpp"
#include <ostream>
#include <vector>

class Function {

public:
  Function(std::vector<ValueType> params, interpreter::Expression body,
           Environment& env);

  std::vector<ValueType> params;
  interpreter::Expression body;
  Environment& env;

  friend std::ostream &operator<<(std::ostream &os, const Function& f);
};

#endif