#ifndef EXP_H
#define EXP_H
#include <string>
#include <list>
#include "visitor.h"

struct Visitor;
struct PythonVisitor;

struct Exp {
    enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP };

    static std::string binarOpToChar(BinaryOp op);

    virtual ~Exp() = 0;
};

struct BinaryExp final : Exp {
    Exp *left;
    Exp *right;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    ~BinaryExp() override;

    int accept(Visitor *visitor);
};

struct NumberExp final : Exp {
    int value;

    explicit NumberExp(int value);

    ~NumberExp() override;

    int accept(Visitor *visitor);
};

struct IdExp final : Exp {
    std::string id;

    explicit IdExp(std::string id);

    ~IdExp() override;

    int accept(Visitor *visitor);
};

struct Equation {
    Exp *left;
    Exp *right;

    Equation(Exp *l, Exp *r);

    ~Equation();

    int accept(Visitor *visitor);
};

struct ModelBlock {
    std::list<Equation *> equations;

    explicit ModelBlock(std::list<Equation *> equations);

    ~ModelBlock();

    int accept(Visitor *visitor);
};

struct VarBlock {
    std::list<std::string> vars;

    explicit VarBlock(std::list<std::string> vars);

    ~VarBlock();

    int accept(Visitor *visitor);
};

struct Program {
    VarBlock *var_block;
    ModelBlock *model_block;
};

#endif //EXP_H
