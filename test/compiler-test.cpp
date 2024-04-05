#define BOOST_TEST_MODULE Compiler Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "boost/test/unit_test_suite.hpp"

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

BOOST_AUTO_TEST_CASE(compile_and_eval_conditions) {
    Expression exps = Expression();
    exps.push_back("if");
    exps.push_back(1);
    exps.push_back(2);
    exps.push_back(3);

    // Test compilation
    Code bytecode = interpreter::compile(exps);
    BOOST_TEST(bytecode.size() == 5);

    Instruction i0 = bytecode[0];
    BOOST_TEST(i0 == Instruction(OpCode::LOAD_CONST, 1));

    Instruction i1 = bytecode[1];
    BOOST_TEST(i1 == Instruction(OpCode::RELATIVE_JUMP_IF_TRUE, 2));

    Instruction i2 = bytecode[2];
    BOOST_TEST(i2 == Instruction(OpCode::LOAD_CONST, 3));

    Instruction i3 = bytecode[3];
    BOOST_TEST(i3 == Instruction(OpCode::RELATIVE_JUMP, 1));

    Instruction i4 = bytecode[4];
    BOOST_TEST(i4 == Instruction(OpCode::LOAD_CONST, 2));

    // Test evaluation
    Environment env = Environment();
    auto result = interpreter::eval(bytecode, env);
    BOOST_TEST(std::get<int>(result) == 2);
}

BOOST_AUTO_TEST_CASE(compile_and_eval_conditions_with_vars) {
    Expression exps = Expression();
    exps.push_back("if");
    exps.push_back("cond");
    exps.push_back(2);
    exps.push_back(3);

    Code bytecode = interpreter::compile(exps);
    Environment env = Environment();
    env.define("cond", 1);
    auto result = interpreter::eval(bytecode, env);
    BOOST_TEST(std::get<int>(result) == 2);

    env = Environment();
    env.define("cond", 0);
    result = interpreter::eval(bytecode, env);
    BOOST_TEST(std::get<int>(result) == 3);
}