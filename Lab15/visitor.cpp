#include <iostream>
#include "exp.h"
#include "visitor.h"

#include <filesystem>
#include <fstream>
#include <unordered_map>
using namespace std;

unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp *exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp *exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(IdentifierExp *exp) {
    cout << exp->name;
    return 0;
}

void PrintVisitor::visit(AssignStatement *stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement *stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::imprimir(Program *program) {
    for (Stm *s: program->slist) {
        s->accept(this);
        cout << endl;
    }
};

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp *exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (Exp::binopToChar(exp->op)) {
        case '+': result = v1 + v2;
            break;
        case '-': result = v1 - v2;
            break;
        case '*': result = v1 * v2;
            break;
        case '/':
            if (v2 != 0) result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp *exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp *exp) {
    return memoria[exp->name];
}

void EVALVisitor::visit(AssignStatement *stm) {
    memoria[stm->id] = stm->rhs->accept(this);
}

void EVALVisitor::visit(PrintStatement *stm) {
    cout << stm->e->accept(this);
}

void EVALVisitor::ejecutar(Program *program) {
    for (Stm *s: program->slist) {
        s->accept(this);
    }
}

// GenCodeVisitor

void GenCodeVisitor::genCode(const Program *program) {
    file << ".data\n";
    file << "print_fmt: .string \"%ld\\n\"\n";
    file << ".text\n";
    file << ".globl main\n";
    file << "main:\n";
    file << " pushq %rbp\n";
    file << " movq %rsp, %rbp\n";

    for (const auto s: program->slist) {
        s->accept(this);
    }

    file << " movl $0, %eax\n";
    file << " leave\n";
    file << " ret\n";
    file << ".section .note.GNU-stack,\"\",@progbits\n";
    file.close();
    cout << "Codigo generado en: " << std::filesystem::absolute(output) << endl;
}

int GenCodeVisitor::visit(BinaryExp *exp) {
    exp->left->accept(this);
    file << " pushq %rax\n";
    exp->right->accept(this);
    file << " movq %rax, %rcx\n";
    file << " popq %rax\n";
    switch (Exp::binopToChar(exp->op)) {
        case '+':
            file << " addq %rcx, %rax\n";
            break;
        case '-':
            file << " subq %rcx, %rax\n";
            break;
        case '*':
            file << " imulq %rcx, %rax\n";
            break;
        default:
            exit(2);
    }
    return 0;
}

int GenCodeVisitor::visit(NumberExp *exp) {
    file << " movq $" << exp->value << ", %rax\n";
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp *exp) {
    file << " movq " << map[exp->name] << "(%rbp), %rax\n";
    return 0;
}

void GenCodeVisitor::visit(AssignStatement *stm) {
    stm->rhs->accept(this);
    map[stm->id] = counter * (-8);
    counter++;
    file << " movq %rax, " << map[stm->id] << "(%rbp)\n";
}

void GenCodeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
    file << " movq %rax, %rsi\n";
    file << " leaq print_fmt(%rip), %rdi\n";
    file << " movl $0, %eax\n";
    file << " call printf@PLT\n";
}
