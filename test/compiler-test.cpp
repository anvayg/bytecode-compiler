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
  Instruction i = bytecode[0];
  BOOST_TEST(i == Instruction(OpCode::LOAD_CONST, 5));
}

BOOST_AUTO_TEST_CASE(compile_and_eval_val) {
    Expression exps = Expression();
    exps.push_back("val");
    exps.push_back("x");
    exps.push_back(5);

    // Test compilation
    Code bytecode = interpreter::compile(exps);
    BOOST_TEST(bytecode.size() == 2);

    Instruction i1 = bytecode[0];
    BOOST_TEST(i1 == Instruction(OpCode::LOAD_CONST, 5));

    Instruction i2 = bytecode[1];
    BOOST_TEST(i2 == Instruction(OpCode::STORE_NAME, "x"));

    // Test evaluation
    Environment env = Environment();
    auto result = interpreter::eval(bytecode, env);
    BOOST_TEST(std::get<int>(env.lookup("x")) == 5);
}
