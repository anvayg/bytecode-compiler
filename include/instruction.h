#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <variant>

enum OpCode {LOAD_CONST, STORE_NAME, LOAD_NAME};

using ArgType = std::variant<int, std::string>;

class Instruction {
private:
    OpCode opCode;
    ArgType arg;

public:
    Instruction(OpCode op, ArgType arg = nullptr);

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instr);
};

#endif // INSTRUCTION_H