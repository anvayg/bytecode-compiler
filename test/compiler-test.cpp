#include <memory>
#include <utility>
#define BOOST_TEST_MODULE Compiler Tests
#define BOOST_TEST_DYN_LINK

#include "boost/test/unit_test_suite.hpp"
#include <boost/test/unit_test.hpp>

#include "../include/instruction.hpp"
#include "../include/interpreter.hpp"

#include <vector>

using Code = std::vector<Instruction>;

BOOST_AUTO_TEST_CASE(compile_int) {
  Constant c(5);

  Code bytecode = interpreter::compile(c);
  Instruction i = bytecode[0];
  BOOST_TEST(i == Instruction(OpCode::LOAD_CONST, 5));
}

BOOST_AUTO_TEST_CASE(compile_and_eval_val) {
  std::unique_ptr<Expression> expr1 = std::make_unique<StringConstant>("val");
  std::unique_ptr<Expression> expr2 = std::make_unique<StringConstant>("x");
  std::unique_ptr<Expression> expr3 = std::make_unique<Constant>(5);

  std::vector<std::unique_ptr<Expression>> exps;
  exps.push_back(std::move(expr1));
  exps.push_back(std::move(expr2));
  exps.push_back(std::move(expr3));
  ExpressionList l(std::move(exps));

  // Test compilation
  Code bytecode = interpreter::compile(l);
  BOOST_TEST(bytecode.size() == 2);

  Instruction i1 = bytecode[0];
  BOOST_TEST(i1 == Instruction(OpCode::LOAD_CONST, 5));

  Instruction i2 = bytecode[1];
  BOOST_TEST(i2 == Instruction(OpCode::STORE_NAME, "x"));

  // Test evaluation
  Environment env = Environment();
  auto result = interpreter::eval(bytecode, env);
  BOOST_TEST(boost::get<int>(env.lookup("x")) == 5);
}

BOOST_AUTO_TEST_CASE(compile_and_eval_conditions) {
  std::unique_ptr<Expression> expr1 = std::make_unique<StringConstant>("if");
  std::unique_ptr<Expression> expr2 = std::make_unique<Constant>(1);
  std::unique_ptr<Expression> expr3 = std::make_unique<Constant>(2);
  std::unique_ptr<Expression> expr4 = std::make_unique<Constant>(3);

  std::vector<std::unique_ptr<Expression>> exps;
  exps.push_back(std::move(expr1));
  exps.push_back(std::move(expr2));
  exps.push_back(std::move(expr3));
  exps.push_back(std::move(expr4));
  ExpressionList l(std::move(exps));

  // Test compilation
  Code bytecode = interpreter::compile(l);
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
  BOOST_TEST(boost::get<int>(result) == 2);
}

BOOST_AUTO_TEST_CASE(compile_and_eval_conditions_with_vars) {
  std::unique_ptr<Expression> expr1 = std::make_unique<StringConstant>("if");
  std::unique_ptr<Expression> expr2 = std::make_unique<StringConstant>("cond");
  std::unique_ptr<Expression> expr3 = std::make_unique<Constant>(2);
  std::unique_ptr<Expression> expr4 = std::make_unique<Constant>(3);

  std::vector<std::unique_ptr<Expression>> exps;
  exps.push_back(std::move(expr1));
  exps.push_back(std::move(expr2));
  exps.push_back(std::move(expr3));
  exps.push_back(std::move(expr4));
  ExpressionList l(std::move(exps));

  Code bytecode = interpreter::compile(l);
  Environment env = Environment();
  env.define("cond", 1);
  auto result = interpreter::eval(bytecode, env);
  BOOST_TEST(boost::get<int>(result) == 2);

  env = Environment();
  env.define("cond", 0);
  result = interpreter::eval(bytecode, env);
  BOOST_TEST(boost::get<int>(result) == 3);
}

BOOST_AUTO_TEST_CASE(compile_function) {
  
}
