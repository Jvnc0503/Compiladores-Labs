#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include "environment.h"
#include <list>
#include <fstream>
#include <unordered_map>
#include <filesystem>

class BinaryExp;
class NumberExp;
class BoolExp;
class IFExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual int visit(IFExp *exp) = 0;

    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(BoolExp *exp) = 0;

    virtual int visit(IdentifierExp *exp) = 0;

    virtual void visit(AssignStatement *stm) = 0;

    virtual void visit(PrintStatement *stm) = 0;

    virtual void visit(IfStatement *stm) = 0;

    virtual void visit(VarDec *stm) = 0;

    virtual void visit(VarDecList *stm) = 0;

    virtual void visit(StatementList *stm) = 0;

    virtual void visit(Body *b) = 0;
};

class PrintVisitor final : public Visitor {
public:
    void imprimir(Program *program);

    int visit(IFExp *exp) override;

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(BoolExp *exp) override;

    int visit(IdentifierExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(VarDec *stm) override;

    void visit(VarDecList *stm) override;

    void visit(StatementList *stm) override;

    void visit(Body *b) override;
};

#define OUTPUT "../output.s"

class GenCodeVisitor final : public Visitor {
    std::ofstream file;
    std::unordered_map<std::string, int> variables;
    int counter = 1;

public:
    GenCodeVisitor() {
        if (std::filesystem::exists(OUTPUT)) {
            std::filesystem::remove(OUTPUT);
        }
        file = std::ofstream(OUTPUT);
    }

    ~GenCodeVisitor() override {
        if (file.is_open()) {
            file.close();
        }
    }

    void imprimir(Program *program);

    int visit(IFExp *exp) override;

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(BoolExp *exp) override;

    int visit(IdentifierExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;

    void visit(IfStatement *stm) override;

    void visit(VarDec *stm) override;

    void visit(VarDecList *stm) override;

    void visit(StatementList *stm) override;

    void visit(Body *b) override;
};

#endif // VISITOR_H
