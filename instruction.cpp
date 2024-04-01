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
    Instruction(OpCode op, ArgType arg=nullptr) {
        this->opCode = op;
        this->arg = arg;
    }

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instr) {
        os << "OpCode: " << instr.opCode << ", Arg: ";
        std::visit([&os](const auto& arg) {
            os << arg;
        }, instr.arg);
        return os;
    }
};

int main() {
    return 0;
}