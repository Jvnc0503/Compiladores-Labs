#include <iostream>
#include "exp.h"
using namespace std;

BinaryExp::BinaryExp(Exp *l, Exp *r, BinaryOp op): left(l), right(r), op(op) {
}

int UnaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

NumberExp::NumberExp(int v): value(v) {
}

IdentifierExp::IdentifierExp(const string &n): name(n) {
}

Exp::~Exp() {
}

BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

UnaryExp::~UnaryExp() {
    delete exp;
}

int IfExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

IfExp::~IfExp() {
    delete first;
    delete second;
    delete third;
}

AssignStatement::AssignStatement(string id, Exp *e): id(id), rhs(e) {
}

AssignStatement::~AssignStatement() {
    delete rhs;
}

PrintStatement::PrintStatement(Exp *e): e(e) {
}

PrintStatement::~PrintStatement() {
    delete e;
}

IfStatement::IfStatement(Exp *c, list<Stm *> t, list<Stm *> e): condition(c), then(t), els(e) {
}

IfStatement::~IfStatement() {
    delete condition;
    for (Stm *s: then) {
        delete s;
    }
    for (Stm *s: els) {
        delete s;
    }
}

int WhileStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int ForStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

ForStatement::~ForStatement() {
    delete first;
    delete second;
    delete third;
    for (const auto &s: body) {
        delete s;
    }
}

Program::Program() {
}

void Program::add(Stm *s) {
    slist.push_back(s);
}

Program::~Program() {
    for (Stm *s: slist) {
        delete s;
    }
}

Stm::~Stm() {
}

string Exp::binopToChar(BinaryOp op) {
    string c;
    switch (op) {
        case PLUS_OP: c = "+";
            break;
        case MINUS_OP: c = "-";
            break;
        case MUL_OP: c = "*";
            break;
        case DIV_OP: c = "/";
            break;
        case LT_OP: c = "<";
            break;
        case LE_OP: c = "<=";
            break;
        case EQ_OP: c = "==";
            break;
        case AND_OP: c = "and";
            break;
        case OR_OP: c = "or";
            break;
        default: c = "$";
    }
    return c;
}

string Exp::unopToChar(UnaryOp op) {
    string c;
    switch (op) {
        case NOT_OP: c = "not";
            break;
    }
    return c;
}
