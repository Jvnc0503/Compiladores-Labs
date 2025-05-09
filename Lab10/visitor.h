#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>

class IndexExp;
class ForStringStatement;
class ForRangeStatement;
class StringExp;
class BinaryExp;
class NumberExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class WhileStatement;
class IFExp;
class IfStatement;
class Program;

class Visitor {
public:
    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(IdentifierExp *exp) = 0;

    virtual int visit(IFExp *exp) = 0;

    virtual int visit(StringExp *exp) = 0;

    virtual int visit(IndexExp *exp) = 0;

    virtual void visit(AssignStatement *stm) = 0;

    virtual void visit(PrintStatement *stm) = 0;

    virtual void visit(WhileStatement *stm) = 0;

    virtual void visit(IfStatement *stm) = 0;

    virtual void visit(ForRangeStatement *stm) = 0;

    virtual void visit(ForStringStatement *stm) = 0;
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdentifierExp *exp) override;

    int visit(StringExp *exp) override;

    int visit(IndexExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(WhileStatement *stm) override;

    int visit(IFExp *exp) override;

    void visit(ForRangeStatement *stm) override;

    void visit(ForStringStatement *stm) override;
};

class EVALVisitor : public Visitor {
public:
    void ejecutar(Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdentifierExp *exp) override;

    int visit(StringExp *exp) override;

    int visit(IndexExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(WhileStatement *stm) override;

    int visit(IFExp *exp) override;

    void visit(ForRangeStatement *stm) override;

    void visit(ForStringStatement *stm) override;
};

#endif // VISITOR_H
