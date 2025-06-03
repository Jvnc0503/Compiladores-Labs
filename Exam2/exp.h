#ifndef EXP_H
#define EXP_H
#include <string>
#include <list>

struct Exp {
    enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP };

    virtual ~Exp() = 0;
};

struct BinaryExp final : public Exp {
    Exp *left;
    Exp *right;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    ~BinaryExp() override;
};

struct NumberExp final : public Exp {
    int value;

    NumberExp(int value);

    ~NumberExp() override;
};

struct IdExp final : public Exp {
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

#endif //EXP_H
