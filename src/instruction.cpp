#include "../include/ast.hpp"
#include <iostream>
#include <memory>

// Overload the << operator for the OpCode enum class
std::ostream &operator<<(std::ostream &os, const OpCode &opCode) {
  switch (opCode) {
  case OpCode::LOAD_CONST:
    os << "LOAD_CONST";
    break;
  case OpCode::STORE_NAME:
    os << "STORE_NAME";
    break;
  case OpCode::LOAD_NAME:
    os << "LOAD_NAME";
    break;
  case OpCode::RELATIVE_JUMP:
    os << "RELATIVE_JUMP";
    break;
  case OpCode::RELATIVE_JUMP_IF_TRUE:
    os << "RELATIVE_JUMP_IF_TRUE";
    break;
  case OpCode::MAKE_FUNCTION:
    os << "MAKE_FUNCTION";
    break;
  }
  return os;
}

Instruction::Instruction(OpCode op, ValueType arg) : opCode(op), arg(arg) {}

bool Instruction::operator==(const Instruction &other) const {
  return (this->opCode == other.opCode) && (this->arg == other.arg);
}

// Define the InstructionPrinter visitor
struct InstructionPrinter : public boost::static_visitor<void> {
  std::ostream &os_;

  InstructionPrinter(std::ostream &os) : os_(os) {}

  void operator()(const int &i) const { os_ << "int: " << i; }

  void operator()(const std::string &s) const { os_ << "std::string: " << s; }

  void operator()(const std::vector<std::string> &v) const {
    os_ << "[";
    for (const auto &param : v) {
      os_ << param << ", ";
    }
    os_ << "]";
  }

  void operator()(const std::shared_ptr<Function> ptr) const {
    // TODO
  }

  void operator()(const std::vector<Instruction> &instructions) const {
    os_ << "Instructions: [";
    for (const auto &instr : instructions) {
      os_ << instr << " ";
    }
    os_ << "]\n";
  }
};

std::ostream &operator<<(std::ostream &os, const Instruction &instr) {
  os << "Instruction(opCode=" << instr.opCode << ", arg=";
  boost::apply_visitor(InstructionPrinter(os), instr.arg);
  os << ")";
  return os;
}
