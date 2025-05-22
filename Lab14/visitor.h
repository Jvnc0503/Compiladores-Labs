#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include "environment.h"
#include <list>

class BinaryExp;
class NumberExp;
class BoolExp;
class IFExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;
class FunctionCallExp;
class FunDec;
class FunDecList;
class ReturnStatement;

class Visitor {
public:
    virtual int visit(IFExp *exp) = 0;

    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(BoolExp *exp) = 0;

    virtual int visit(IdentifierExp *exp) = 0;

    virtual int visit(FunctionCallExp *exp) = 0;

    virtual void visit(AssignStatement *stm) = 0;

    virtual void visit(PrintStatement *stm) = 0;

    virtual void visit(IfStatement *stm) = 0;

    virtual void visit(WhileStatement *stm) = 0;

    virtual void visit(ReturnStatement *stm) = 0;

    virtual void visit(VarDec *stm) = 0;

    virtual void visit(VarDecList *stm) = 0;

    virtual void visit(FunDec *stm) = 0;

    virtual void visit(FunDecList *stm) = 0;

    virtual void visit(StatementList *stm) = 0;

    virtual void visit(Body *b) = 0;
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program *program);

    int visit(IFExp *exp) override;

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(BoolExp *exp) override;

    int visit(IdentifierExp *exp) override;

    int visit(FunctionCallExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(WhileStatement *stm) override;

    void visit(ReturnStatement *stm) override;

    void visit(VarDec *stm) override;

    void visit(VarDecList *stm) override;

    void visit(FunDec *stm) override;

    void visit(FunDecList *stm) override;

    void visit(StatementList *stm) override;

    void visit(Body *b) override;
};

class EVALVisitor : public Visitor {
    Environment env;
    unordered_map<string, FunDec *> functions;
    bool retcall = false;
    int retval = 0;

public:
    void ejecutar(Program *program);

    int visit(IFExp *exp) override;

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(BoolExp *exp) override;

    int visit(IdentifierExp *exp) override;

    int visit(FunctionCallExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(WhileStatement *stm) override;

    void visit(ReturnStatement *stm) override;

    void visit(VarDec *stm) override;

    void visit(VarDecList *stm) override;

    void visit(FunDec *stm) override;

    void visit(FunDecList *stm) override;

    void visit(StatementList *stm) override;

    void visit(Body *b) override;
};

#endif // VISITOR_H
