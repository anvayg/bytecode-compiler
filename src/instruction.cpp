#include "../include/instruction.hpp"
#include <iostream>
#include <variant>

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
  }
  return os;
}

Instruction::Instruction(OpCode op, ValueType arg) : opCode(op), arg(arg) {}

bool Instruction::operator==(const Instruction &other) const {
  return (this->opCode == other.opCode) && (this->arg == other.arg);
}

std::ostream &operator<<(std::ostream &os, const Instruction &instr) {
  os << "OpCode: " << instr.opCode << ", Arg: ";
  std::visit([&os](const auto &arg) { os << arg; }, instr.arg);
  return os;
}
