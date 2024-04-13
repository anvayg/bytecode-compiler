// expression.hpp

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <boost/variant.hpp>

// Forward declarations
class Constant;
class BinaryOperation;
class StringConstant;
class ExpressionList;
class Instruction;
class Function;
class Environment;
class Expression;

// Available Opcodes
enum class OpCode {
  LOAD_CONST,
  STORE_NAME,
  LOAD_NAME,
  RELATIVE_JUMP,
  RELATIVE_JUMP_IF_TRUE,
  MAKE_FUNCTION,
  CALL_FUNCTION
};

typedef boost::variant<int, std::string, std::vector<std::string>,
                       std::shared_ptr<Function>,
                       boost::recursive_wrapper<std::vector<Instruction>>>
    ValueType;

// Definition of Instruction
class Instruction {
public:
  OpCode opCode;
  ValueType arg;

  Instruction(OpCode op, ValueType arg);

  bool operator==(const Instruction &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

// Definition of Environment
using Table = std::unordered_map<std::string, ValueType>;

class Environment {
private:
  Table table;
  Environment *parent;

public:
  Environment();

  Environment(Table env, Environment *parent);

  void define(std::string name, ValueType value);

  void assign(std::string name, ValueType value);

  ValueType lookup(std::string name);

  Table &resolve(std::string name);

  bool isDefined(std::string name);

  friend std::ostream &operator<<(std::ostream &os, const Environment &env);
};


// Visitor interface
class ExpressionVisitor {
public:
  virtual ~ExpressionVisitor() {}
  virtual void visit(Constant &constant) = 0;
  virtual void visit(BinaryOperation &binaryOperation) = 0;
  virtual void visit(StringConstant &stringConstant) = 0;
  virtual void visit(ExpressionList &expressionList) = 0;
};

// Visitor interface for compilation
class CompilerVisitor {
public:
  virtual ~CompilerVisitor() {}
  virtual std::vector<Instruction> visit(Constant &constant) = 0;
  virtual std::vector<Instruction> visit(BinaryOperation &binaryOperation) = 0;
  virtual std::vector<Instruction> visit(StringConstant &stringConstant) = 0;
  virtual std::vector<Instruction> visit(ExpressionList &expressionList) = 0;
};

// Base class representing an expression
class Expression {
public:
  virtual ~Expression() {}
  virtual void accept(ExpressionVisitor &visitor) = 0;
  virtual std::vector<Instruction> accept(CompilerVisitor &visitor) = 0;

  // Overload the << operator
  friend std::ostream &operator<<(std::ostream &os, Expression &expr);
};

// Derived class representing a constant value
class Constant : public Expression {
private:
  int value;

public:
  Constant(int val) : value(val) {}

  int getValue() const { return value; }

  void accept(ExpressionVisitor &visitor) override { visitor.visit(*this); }

  std::vector<Instruction> accept(CompilerVisitor &visitor) override {
    return visitor.visit(*this);
  }
};

// Derived class representing a binary operation
class BinaryOperation : public Expression {
private:
  char op;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;

public:
  BinaryOperation(char _op, std::unique_ptr<Expression> _left,
                  std::unique_ptr<Expression> _right)
      : op(_op), left(std::move(_left)), right(std::move(_right)) {}

  char getOperator() const { return op; }
  Expression &getLeft() const { return *left; }
  Expression &getRight() const { return *right; }

  void accept(ExpressionVisitor &visitor) override { visitor.visit(*this); }

  std::vector<Instruction> accept(CompilerVisitor &visitor) override {
    return visitor.visit(*this);
  }
};

// Derived class representing a string constant
class StringConstant : public Expression {
private:
  std::string value;

public:
  StringConstant(const std::string &val) : value(val) {}

  const std::string &getValue() const { return value; }

  void accept(ExpressionVisitor &visitor) override { visitor.visit(*this); }

  std::vector<Instruction> accept(CompilerVisitor &visitor) override {
    return visitor.visit(*this);
  }
};

// Derived class representing a list of expressions
class ExpressionList : public Expression {
private:
  std::vector<std::unique_ptr<Expression>> expressions;

public:
  ExpressionList(std::vector<std::unique_ptr<Expression>> exprs)
      : expressions(std::move(exprs)) {}

  const std::vector<std::unique_ptr<Expression>> &getExpressions() const {
    return expressions;
  }

  void accept(ExpressionVisitor &visitor) override { visitor.visit(*this); }

  std::vector<Instruction> accept(CompilerVisitor &visitor) override {
    return visitor.visit(*this);
  }
};

// Concrete visitor implementation
class PrintVisitor : public ExpressionVisitor {
private:
  std::ostream &os;

public:
  PrintVisitor(std::ostream &_os) : os(_os) {}

  void visit(Constant &constant) override;
  void visit(BinaryOperation &binaryOperation) override;
  void visit(StringConstant &stringConstant) override;
  void visit(ExpressionList &expressionList) override;
};

// Concrete visitor implementation
class Compiler : public CompilerVisitor {
public:
  Compiler() {}

  std::vector<Instruction> visit(Constant &constant) override;
  std::vector<Instruction> visit(BinaryOperation &binaryOperation) override;
  std::vector<Instruction> visit(StringConstant &stringConstant) override;
  std::vector<Instruction> visit(ExpressionList &expressionList) override;
};

// Definition of Function
class Function {

public:
  Function(std::vector<std::string> params, std::vector<Instruction> body,
           Environment env);

  std::vector<std::string> params;
  std::vector<Instruction> body;
  Environment env;

  friend std::ostream &operator<<(std::ostream &os, const Function &f);
};

#endif // EXPRESSION_HPP