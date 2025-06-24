#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor *visitor) {
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

int WhileStatement::accept(Visitor *visitor) {
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

int FCallExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int ReturnStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FunDecList::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int Program::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

void GenCodeVisitor::generar(Program *program) {
    program->accept(this);
}

void GenCodeVisitor::visit(Program *program) {
    out << ".data\nprint_fmt: .string \"%ld \\n\"" << endl;
    program->vardecs->accept(this);

    for (auto &[var, _]: memoriaGlobal) {
        out << var << ": .quad 0" << endl;
    }

    out << ".text\n";
    program->fundecs->accept(this);
    out << ".section .note.GNU-stack,\"\",@progbits" << endl;
}

void GenCodeVisitor::visit(VarDec *stm) {
    for (const auto &var: stm->vars) {
        if (!entornoFuncion) {
            memoriaGlobal[var] = true;
        } else {
            memoria[var] = offset;
            offset -= 8;
        }
    }
}

void GenCodeVisitor::visit(VarDecList *stm) {
    for (const auto &dec: stm->vardecs)
        dec->accept(this);
}

int GenCodeVisitor::visit(NumberExp *exp) {
    out << " movq $" << exp->value << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp *exp) {
    if (memoriaGlobal.contains(exp->name))
        out << " movq " << exp->name << "(%rip), %rax" << endl;
    else
        out << " movq " << memoria[exp->name] << "(%rbp), %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(BinaryExp *exp) {
    exp->left->accept(this);
    out << " pushq %rax\n";
    exp->right->accept(this);
    out << " movq %rax, %rcx\n popq %rax\n";

    switch (exp->op) {
        case PLUS_OP: out << " addq %rcx, %rax\n";
            break;
        case MINUS_OP: out << " subq %rcx, %rax\n";
            break;
        case MUL_OP: out << " imulq %rcx, %rax\n";
            break;
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
                    << " movl $0, %eax\n"
                    << " setle %al\n"
                    << " movzbq %al, %rax\n";
            break;
        case LT_OP:
            out << " cmpq %rcx, %rax\n"
                    << " movl $0, %eax\n"
                    << " setl %al\n"
                    << " movzbq %al, %rax\n";
            break;
    }
    return 0;
}

void GenCodeVisitor::visit(AssignStatement *stm) {
    stm->rhs->accept(this);
    if (memoriaGlobal.contains(stm->id))
        out << " movq %rax, " << stm->id << "(%rip)" << endl;
    else
        out << " movq %rax, " << memoria[stm->id] << "(%rbp)" << endl;
}

void GenCodeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
    out <<
            " movq %rax, %rsi\n"
            " leaq print_fmt(%rip), %rdi\n"
            " movl $0, %eax\n"
            " call printf@PLT\n";
}

void GenCodeVisitor::visit(FunDecList *f) {
    for (const auto &dec: f->Fundecs)
        dec->accept(this);
}

void GenCodeVisitor::visit(StatementList *stm) {
    for (const auto &s: stm->stms)
        s->accept(this);
}

void GenCodeVisitor::visit(Body *b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
}

void GenCodeVisitor::visit(IfStatement *stm) {
    int label = labelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je else_" << label << endl;
    stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << endl;
    if (stm->els) stm->els->accept(this);
    out << "endif_" << label << endl;
}

void GenCodeVisitor::visit(WhileStatement *stm) {
    int label = labelcont++;
    out << "while_" << label << ":" << endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->b->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << endl;
}

int GenCodeVisitor::visit(BoolExp *exp) {
    out << " movq $" << exp->value << ", %rax" << endl;
    return 0;
}

void GenCodeVisitor::visit(ReturnStatement *stm) {
    stm->e->accept(this);
    out << " jmp .end_" << nombreFuncion << '\n';
}

void GenCodeVisitor::visit(FunDec *f) {
    entornoFuncion = true;
    memoria.clear();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->nombre << '\n'
            << f->nombre << ":\n"
            << " pushq %rbp\n"
            << " movq %rsp, %rbp\n";
    if (f->parametros.size() > 6) {
        cerr << "More than six arguments are not supported\n";
        exit(1);
    }
    const unsigned short numArgs = f->parametros.size();
    for (unsigned short i = 0; i < numArgs; i++) {
        memoria[f->parametros[i]] = offset;
        out << " movq " << argRegs[i] << ", " << offset << "(%rbp)\n";
        offset -= 8;
    }
    f->cuerpo->accept(this);
    const int reserva = -offset * 8;
    out << " subq $" << reserva << ", %rsp\n";
    f->cuerpo->slist->accept(this);
    out << ".end_" << f->nombre << '\n'
            << "leave\n"
            << "ret\n";
    entornoFuncion = false;
}

int GenCodeVisitor::visit(FCallExp *exp) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    if (exp->argumentos.size() > 6) {
        cerr << "More than six arguments are not supported\n";
        exit(1);
    }
    const unsigned short numArgs = exp->argumentos.size();
    for (unsigned short i = 0; i < numArgs; i++) {
        exp->argumentos[i]->accept(this);
        out << " mov %rax, " << argRegs[i] << '\n';
    }
    out << " call " << exp->nombre << '\n';
    return 0;
}
