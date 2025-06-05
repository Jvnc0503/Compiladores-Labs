#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

class BinaryExp;
class NumberExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class Program;

class Visitor {
public:
    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(IdentifierExp *exp) = 0;

    virtual void visit(AssignStatement *stm) = 0;

    virtual void visit(PrintStatement *stm) = 0;
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdentifierExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;
};

class EVALVisitor : public Visitor {
public:
    void ejecutar(Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdentifierExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;
};

#define output "../output.s"

class GenCodeVisitor : public Visitor {
    std::ofstream file;
    std::unordered_map<std::string, int> map;
    int counter = 1;

public:
    GenCodeVisitor() {
        if (std::filesystem::exists(output)) {
            std::filesystem::remove(output);
        }
        file = std::ofstream(output);
    }

    void genCode(const Program *program);

    int visit(BinaryExp *exp) override;

    int visit(NumberExp *exp) override;

    int visit(IdentifierExp *exp) override;

    void visit(AssignStatement *stm) override;

    void visit(PrintStatement *stm) override;
};

#endif // VISITOR_H
