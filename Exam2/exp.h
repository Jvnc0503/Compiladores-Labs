#ifndef EXP_H
#define EXP_H
#include <string>
#include <list>

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
};

struct NumberExp final : Exp {
    int value;

    NumberExp(int value);

    ~NumberExp() override;
};

struct IdExp final : Exp {
    std::string id;

    explicit IdExp(std::string id);

    ~IdExp() override;
};

struct Equation {
    Exp *left;
    Exp *right;

    Equation(Exp *l, Exp *r);

    ~Equation();
};

struct ModelBlock {
    std::list<Equation *> equations;

    explicit ModelBlock(std::list<Equation *> equations);

    ~ModelBlock();
};

struct VarBlock {
    std::list<std::string> vars;

    explicit VarBlock(std::list<std::string> vars);

    ~VarBlock();
};

#endif //EXP_H
