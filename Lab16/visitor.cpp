#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int IFExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor *visitor) {
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

int IfStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int StatementList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int Body::accept(Visitor *visitor) {
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

int PrintVisitor::visit(BoolExp *exp) {
    if (exp->value) cout << "true";
    else cout << "false";
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

void PrintVisitor::visit(IfStatement *stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    stm->then->accept(this);
    if (stm->els) {
        cout << "else" << endl;
        stm->els->accept(this);
    }
    cout << "endif";
}

void PrintVisitor::imprimir(Program *program) {
    program->body->accept(this);
};


int PrintVisitor::visit(IFExp *pepito) {
    cout << "ifexp(";
    pepito->cond->accept(this);
    cout << ",";
    pepito->left->accept(this);
    cout << ",";
    pepito->right->accept(this);
    cout << ")";
    return 0;
}


void PrintVisitor::visit(VarDec *stm) {
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for (auto i: stm->vars) {
        cout << i;
        if (i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList *stm) {
    for (auto i: stm->vardecs) {
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList *stm) {
    for (auto i: stm->stms) {
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body *stm) {
    stm->vardecs->accept(this);
    cout << endl;
    stm->slist->accept(this);
}

// GenCode

void GenCodeVisitor::imprimir(Program *program) {
    file << ".data\n";
    file << "print_fmt: .string \"%ld\\n\"\n";
    file << ".text\n";
    file << ".globl main\n";
    file << "main:\n";
    file << " pushq %rbp\n";
    file << " movq %rsp, %rbp\n";
    file << " subq $8, %rsp\n";


    file << " movl $0, %eax\n";
    file << " leave\n";
    file << " ret\n";
    file << ".section .note.GNU-stack,\"\",@progbits\n";
    file.close();
    cout << "Codigo generado en: " << std::filesystem::absolute(OUTPUT) << '\n';
}

int GenCodeVisitor::visit(IFExp *exp) {
    return 0;
}

int GenCodeVisitor::visit(BinaryExp *exp) {
    return 0;
}

int GenCodeVisitor::visit(NumberExp *exp) {
    file << " movq $" << exp->value << ", %rax\n";
    return 0;
}

int GenCodeVisitor::visit(BoolExp *exp) {
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp *exp) {
    file << " movq " << variables[exp->name] << "(%rbp), %rax\n";
    return 0;
}

void GenCodeVisitor::visit(AssignStatement *stm) {
    stm->rhs->accept(this);
    file << " movq %rax, " << variables[stm->id] << "(%rbp)\n";
}

void GenCodeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
    file << " movq %rax, %rsi\n";
    file << " leaq print_fmt(%rip), %rdi\n";
    file << " movl $0, %eax\n";
    file << " call printf@PLT\n";
}

void GenCodeVisitor::visit(IfStatement *stm) {
    stm->condition->accept(this);
    cout << " cmpq $0, %rax" << endl;
    cout << " je else1" << endl;
    stm->then->accept(this);
    cout << " jmp endif1" << endl;
    cout << " else1:" << endl;
    stm->els->accept(this);
    cout << " endif1:" << endl;
}

void GenCodeVisitor::visit(VarDec *stm) {
    for (const auto &v: stm->vars) {
        variables[v] = 8 * counter;
    }
}

void GenCodeVisitor::visit(VarDecList *stm) {
    for (const auto &v: stm->vardecs) {
        v->accept(this);
    }
}

void GenCodeVisitor::visit(StatementList *stm) {
    for (const auto &s: stm->stms) {
        s->accept(this);
    }
}

void GenCodeVisitor::visit(Body *b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
}
