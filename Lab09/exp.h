#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include <utility>
#include "visitor.h"
using namespace std;

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP, AND_OP, OR_OP };

enum UnaryOp { NOT_OP };

class Exp {
public:
    virtual int accept(Visitor *visitor) = 0;

    virtual ~Exp() = 0;

    static string binopToChar(BinaryOp op);

    static string unopToChar(UnaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    int accept(Visitor *visitor);

    ~BinaryExp() override;
};

class UnaryExp : public Exp {
public:
    Exp *exp;
    UnaryOp op;

    UnaryExp(Exp *exp, UnaryOp op): exp(exp), op(op) {
    }

    int accept(Visitor *visitor);

    ~UnaryExp() override;
};

class NumberExp : public Exp {
public:
    int value;

    NumberExp(int v);

    int accept(Visitor *visitor);

    ~NumberExp() override = default;
};

class IdentifierExp : public Exp {
public:
    std::string name;

    IdentifierExp(const std::string &n);

    int accept(Visitor *visitor);

    ~IdentifierExp() override = default;
};

class IfExp : public Exp {
public:
    Exp *first;
    Exp *second;
    Exp *third;

    IfExp(Exp *first, Exp *second, Exp *third): first(first), second(second), third(third) {
    }

    int accept(Visitor *visitor);

    ~IfExp() override;
};

class Stm {
public:
    virtual int accept(Visitor *visitor) = 0;

    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp *rhs;

    AssignStatement(std::string id, Exp *e);

    int accept(Visitor *visitor);

    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp *e;

    PrintStatement(Exp *e);

    int accept(Visitor *visitor);

    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp *condition;
    list<Stm *> then;
    list<Stm *> els;

    IfStatement(Exp *condition, list<Stm *> then, list<Stm *> els);

    int accept(Visitor *visitor);

    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp *condition;
    list<Stm *> body;

    WhileStatement(Exp *c, list<Stm *> b): condition(c), body(std::move(b)) {
    }

    int accept(Visitor *visitor);

    ~WhileStatement() {
    }
};

class ForStatement : public Stm {
public:
    Exp *first, *second, *third;
    list<Stm *> body;

    ForStatement(Exp *first, Exp *second, Exp *third, list<Stm *> b): first(first), second(second), third(third),
                                                                      body(std::move(b)) {
    }

    int accept(Visitor *visitor);

    ~ForStatement();
};

class Program {
public:
    std::list<Stm *> slist;

    Program();

    void add(Stm *s);

    ~Program();
};

#endif // EXP_H
