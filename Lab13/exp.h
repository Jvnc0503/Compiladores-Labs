#ifndef EXP_H
#define EXP_H
#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP };

class Body;

class Exp {
public:
    virtual int accept(Visitor *visitor) = 0;

    virtual ~Exp() = 0;

    static string binopToChar(BinaryOp op);
};

class IFExp : public Exp {
public:
    Exp *cond, *left, *right;

    IFExp(Exp *cond, Exp *l, Exp *r);

    int accept(Visitor *visitor);

    ~IFExp();
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    string type;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    int accept(Visitor *visitor);

    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;

    NumberExp(int v);

    int accept(Visitor *visitor);

    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;

    BoolExp(bool v);

    int accept(Visitor *visitor);

    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;

    IdentifierExp(const std::string &n);

    int accept(Visitor *visitor);

    ~IdentifierExp();
};

class FunctionCallExp : public Exp {
public:
    std::string name;
    std::list<Exp *> args;

    FunctionCallExp(string name, std::list<Exp *> args): name(name), args(args) {
    }

    int accept(Visitor *visitor) override;

    ~FunctionCallExp();
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
    Body *then;
    Body *els;

    IfStatement(Exp *condition, Body *then, Body *els);

    int accept(Visitor *visitor);

    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp *condition;
    Body *b;

    WhileStatement(Exp *condition, Body *b);

    int accept(Visitor *visitor);

    ~WhileStatement();
};

class ReturnStatement : public Stm {
public:
    Exp *e;

    ReturnStatement(Exp *e): e(e) {
    }

    int accept(Visitor *visitor);

    ~ReturnStatement();
};

class VarDec {
public:
    string type;
    list<string> vars;

    VarDec(string type, list<string> vars);

    int accept(Visitor *visitor);

    ~VarDec();
};

class VarDecList {
public:
    list<VarDec *> vardecs;

    VarDecList();

    void add(VarDec *vardec);

    int accept(Visitor *visitor);

    ~VarDecList();
};

class FunDec {
public:
    string type;
    string name;
    list<string> params;
    list<string> types;
    Body *body;

    FunDec(string type, string name, list<string> params, list<string> types, Body *body): type(std::move(type)),
        name(std::move(name)),
        params(std::move(params)), types(std::move(types)), body(body) {
    }

    int accept(Visitor *visitor);

    ~FunDec();
};

class FunDecList {
public:
    list<FunDec *> fundecs;

    FunDecList(list<FunDec *> fundecs): fundecs(fundecs) {
    }

    int accept(Visitor *visitor);

    ~FunDecList();
};

class StatementList {
public:
    list<Stm *> stms;

    StatementList();

    void add(Stm *stm);

    int accept(Visitor *visitor);

    ~StatementList();
};

class Body {
public:
    VarDecList *vardecs;
    StatementList *slist;

    Body(VarDecList *vardecs, StatementList *stms);

    int accept(Visitor *visitor);

    ~Body();
};

class Program {
public:
    VarDecList *vardecs;
    FunDecList *fundecs;

    Program() = default;

    Program(VarDecList *vardecs, FunDecList *fundecs): vardecs(vardecs), fundecs(fundecs) {
        ;
    }

    ~Program();
};


#endif // EXP_H
