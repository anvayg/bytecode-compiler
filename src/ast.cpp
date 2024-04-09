#include "../include/ast.hpp"

// Overload the << operator
std::ostream& operator<<(std::ostream& os, Expression& expr) {
    PrintVisitor visitor(os);
    expr.accept(visitor);
    return os;
}

void PrintVisitor::visit(Constant& constant) {
    os << constant.getValue();
}

void PrintVisitor::visit(BinaryOperation& binaryOperation) {
    os << "(";
    binaryOperation.getLeft().accept(*this);
    os << " " << binaryOperation.getOperator() << " ";
    binaryOperation.getRight().accept(*this);
    os << ")";
}

void PrintVisitor::visit(StringConstant& stringConstant) {
    os << stringConstant.getValue();
}

void PrintVisitor::visit(ExpressionList& expressionList) {
    os << "[";
    bool first = true;
    for (const auto& expr : expressionList.getExpressions()) {
        if (!first) os << ", ";
        expr->accept(*this);
        first = false;
    }
    os << "]";
}
