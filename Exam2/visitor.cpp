#include "visitor.h"
#include "exp.h"
#include <iostream>

void PythonVisitor::imprimir(Program *program) {
    program->var_block->accept(this);
    program->model_block->accept(this);
}

int PythonVisitor::visit(BinaryExp *exp) {
    return 0;
}

int PythonVisitor::visit(NumberExp *exp) {
    return 0;
}

int PythonVisitor::visit(IdExp *exp) {
    return 0;
}

void PythonVisitor::visit(Equation *eq) {
}

void PythonVisitor::visit(ModelBlock *mb) {
}

void PythonVisitor::visit(VarBlock *vb) {
}
