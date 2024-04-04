#define BOOST_TEST_MODULE Compiler Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../include/instruction.hpp"
#include "../include/interpreter.hpp"

#include <vector>

using Expression = std::vector<ValueType>;
using Code = std::vector<Instruction>;

BOOST_AUTO_TEST_CASE(compile_int) {
  Expression exps = Expression();
  exps.push_back(5);

  Code bytecode = interpreter::compile(exps);
  Instruction i = *bytecode.begin();
  BOOST_TEST(i == Instruction(OpCode::LOAD_CONST, 5));
}
