#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>
#include "../include/instruction.h"

namespace interpreter {
    using Expression = std::vector<ArgType>;

    std::vector<Instruction> compile(Expression exp) {

        if (exp.size() == 1) {
            auto subexp = exp[0];
            // Define the visitor
            auto visitor = [](auto&& arg) -> std::vector<Instruction> {
                if constexpr (std::is_same_v<decltype(arg), int>) {
                    std::vector<Instruction> ins;
                    ins.push_back(Instruction(OpCode::LOAD_CONST, 5));
                } else {
                    throw std::logic_error("Not supported");
                }
            };
            return std::visit(visitor, subexp);

        } else {
            throw std::runtime_error("Currently not supported");
        }
    }
}
