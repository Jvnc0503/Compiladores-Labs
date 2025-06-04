#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>

struct BinaryExp;
struct NumberExp;
struct IdExp;
struct Equation;
struct ModelBlock;
struct VarBlock;
struct Program;

struct Visitor {
    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(IdExp *exp) = 0;

    virtual void visit(Equation *eq) = 0;

    virtual void visit(ModelBlock *mb) = 0;

    virtual void visit(VarBlock *vb) = 0;
};

struct PythonVisitor : Visitor {
    void imprimir(Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdExp *exp) override;

    void visit(Equation *eq) override;

    void visit(ModelBlock *mb) override;

    void visit(VarBlock *vb) override;
};

#endif //VISITOR_H
