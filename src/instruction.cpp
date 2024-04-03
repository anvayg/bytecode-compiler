#include <iostream>
#include <variant>
#include "../include/instruction.hpp"

Instruction::Instruction(OpCode op, ArgType arg) : opCode(op), arg(arg) {}

bool Instruction::operator==(const Instruction& other) const {
    return (this->opCode == other.opCode) && (this->arg == other.arg);
}

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    os << "OpCode: " << instr.opCode << ", Arg: ";
    std::visit([&os](const auto& arg) {
        os << arg;
    }, instr.arg);
    return os;
}
