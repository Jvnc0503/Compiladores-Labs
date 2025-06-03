#include <string>
#include "exp.h"

std::string Exp::binarOpToChar(const BinaryOp op) {
    switch (op) {
        case PLUS_OP: return "+";
        case MINUS_OP: return "-";
        case MUL_OP: return "*";
        default: return "$";
    }
}

BinaryExp::BinaryExp(Exp *l, Exp *r, const BinaryOp op) : left(l), right(r), op(op) {
}

BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

NumberExp::NumberExp(const int value) : value(value) {
}

NumberExp::~NumberExp() = default;

IdExp::IdExp(std::string id) : id(std::move(id)) {
}

IdExp::~IdExp() = default;

Equation::Equation(Exp *l, Exp *r) {
}

Equation::~Equation() {
    delete left;
    delete right;
}

ModelBlock::ModelBlock(std::list<Equation *> equations) : equations(std::move(equations)) {
}

ModelBlock::~ModelBlock() {
    for (const auto &eq: equations) {
        delete eq;
    }
}

VarBlock::VarBlock(std::list<std::string> vars) : vars(std::move(vars)) {
}

VarBlock::~VarBlock() = default;

