#include <string>
#include "exp.h"

Exp::~Exp() = default;

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

int BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

NumberExp::NumberExp(const int value) : value(value) {
}

NumberExp::~NumberExp() = default;

int NumberExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

IdExp::IdExp(std::string id) : id(std::move(id)) {
}

IdExp::~IdExp() = default;

int IdExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

Equation::Equation(Exp *l, Exp *r) : left(l), right(r) {
}

Equation::~Equation() {
    delete left;
    delete right;
}

int Equation::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

ModelBlock::ModelBlock(std::list<Equation *> equations) : equations(std::move(equations)) {
}

ModelBlock::~ModelBlock() {
    for (const auto &eq: equations) {
        delete eq;
    }
}

int ModelBlock::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

VarBlock::VarBlock(std::list<std::string> vars) : vars(std::move(vars)) {
}

VarBlock::~VarBlock() = default;

int VarBlock::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}
