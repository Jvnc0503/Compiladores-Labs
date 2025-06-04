#include "visitor.h"
#include "exp.h"
#include <iostream>

void PythonVisitor::imprimir(const Program *program) {
    std::cout << "import numpy as np\n\n";
    program->var_block->accept(this);
    program->model_block->accept(this);
    std::cout << "A = np.array([\n";
    for (auto &coef : coefs) {
        std::cout << "[";
        for (const auto &var: variables) {
            std::cout << coef[var];
            if (var != *variables.rbegin()) {
                std::cout << ", ";
            }
        }
        std::cout << "]";
        if (coef != coefs.back()) {
            std::cout << ",";
        }
        std::cout << '\n';
    }
}

int PythonVisitor::visit(BinaryExp *exp) {
    const int left = exp->left->accept(this);
    if (const auto idExp = dynamic_cast<IdExp *>(exp->right)) {
        coefs[equation_idx][idExp->id] = left;
        return 0;
    }
    const int right = exp->right->accept(this);
    switch (exp->op) {
        case Exp::PLUS_OP:
            return left + right;
        case Exp::MINUS_OP:
            return left - right;
        case Exp::MUL_OP:
            return left * right;
        default:
            std::cerr << "Unknown operator: " << exp->op << '\n';
            exit(2);
    }
}

int PythonVisitor::visit(NumberExp *exp) {
    return exp->value();
}

int PythonVisitor::visit(IdExp *exp) {
    variables.insert(exp->id);
    return 0;
}

void PythonVisitor::visit(Equation *eq) {
    eq->left->accept(this);
    results[equation_idx] = eq->right->accept(this);
}

void PythonVisitor::visit(ModelBlock *mb) {
    equation_idx = 0;
    coefs.resize(mb->equations.size());
    results.resize(mb->equations.size());
    for (const auto &eq: mb->equations) {
        eq->accept(this);
        equation_idx++;
    }
}

void PythonVisitor::visit(VarBlock *vb) {
}
