#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <variant>

enum class OpCode { LOAD_CONST, STORE_NAME, LOAD_NAME };

using ValueType = std::variant<int, std::string>;

class Instruction {
public:
  OpCode opCode;
  ValueType arg;

  Instruction(OpCode op, ValueType arg = nullptr);

  bool operator==(const Instruction &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

#endif // INSTRUCTION_H