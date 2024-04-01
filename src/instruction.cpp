#include <iostream>
#include <variant>
#include "instruction.h"


Instruction::Instruction(OpCode op, ArgType arg) : opCode(op), arg(arg) {}

std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
    os << "OpCode: " << instr.opCode << ", Arg: ";
    std::visit([&os](const auto& arg) {
        os << arg;
    }, instr.arg);
    return os;
}

int main() {
    return 0;
}