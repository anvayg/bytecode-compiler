#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/variant.hpp>

enum class OpCode { LOAD_CONST, STORE_NAME, LOAD_NAME, RELATIVE_JUMP, RELATIVE_JUMP_IF_TRUE, MAKE_FUNCTION };

struct Instruction;

typedef boost::variant<int, std::string, boost::recursive_wrapper<std::vector<Instruction>>> ValueType;

struct Instruction {
public:
  OpCode opCode;
  ValueType arg;

  Instruction(OpCode op, ValueType arg);

  bool operator==(const Instruction &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Instruction &instr);
};

#endif // INSTRUCTION_H