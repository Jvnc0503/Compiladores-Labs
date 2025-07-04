#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
#include <algorithm>
#include <variant>
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

int ForStatement::accept(Visitor *visitor) {
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

int PrintVisitor::visit(AssignStatement *stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    return 0;
}

int PrintVisitor::visit(PrintStatement *stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
    return 0;
}

int PrintVisitor::visit(IfStatement *stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    stm->then->accept(this);
    if (stm->els) {
        cout << "else" << endl;
        stm->els->accept(this);
    }
    cout << "endif";
    return 0;
}

int PrintVisitor::visit(WhileStatement *stm) {
    cout << "while ";
    stm->condition->accept(this);
    cout << " do";
    stm->body->accept(this);
    cout << "endwhile\n";
    return 0;
}

int PrintVisitor::visit(ForStatement *stm) {
    cout << "for (";
    stm->init->accept(this);
    cout << ", ";
    stm->condition->accept(this);
    cout << ", ";
    stm->increment->accept(this);
    cout << ")";
    stm->body->accept(this);
    cout << "endfor";
    return 0;
}

int PrintVisitor::imprimir(Program *program) {
    program->body->accept(this);
    return 0;
}

int PrintVisitor::visit(VarDec *stm) {
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for (const auto &i: stm->vars) {
        cout << i;
        if (i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
    return 0;
}

int PrintVisitor::visit(VarDecList *stm) {
    for (auto i: stm->vardecs) {
        i->accept(this);
        cout << endl;
    }
    return 0;
}

int PrintVisitor::visit(StatementList *stm) {
    for (const auto &i: stm->stms) {
        i->accept(this);
        if (i != stm->stms.back()) {
            cout << ';';
        }
        cout << '\n';
    }
    return 0;
}

int PrintVisitor::visit(Body *stm) {
    stm->vardecs->accept(this);
    cout << endl;
    stm->slist->accept(this);
    return 0;
}

// GenCode

int GenCodeVisitor::gencode(Program *program) {
    typeChecker.revisar(program);
    memoria.clear();
    cantidad = 1;
    ifCounter = 1;
    whileCounter = 1;
    forCounter = 1;
    cout << ".data" << endl;
    cout << "print_fmt: .string \"%ld \\n\" " << endl;
    cout << ".text " << endl;
    cout << ".globl main " << endl;
    cout << "main: " << endl;
    cout << " pushq %rbp" << endl;
    cout << " movq %rsp, %rbp" << endl;
    cout << " subq $" << typeChecker.cantidad * 8 << ", %rsp" << endl;
    program->body->accept(this);
    cout << " movl $0, %eax " << endl;
    cout << " leave" << endl;
    cout << " ret" << endl;
    cout << ".section .note.GNU-stack,\"\",@progbits" << endl;
    return 0;
}

int GenCodeVisitor::visit(BinaryExp *exp) {
    exp->left->accept(this);
    cout << " pushq %rax" << endl;
    switch (Exp::binopToChar(exp->op)) {
        case '+': {
            exp->right->accept(this);
            cout << " movq %rax, %rcx" << endl;
            cout << " popq %rax" << endl;
            cout << " addq %rcx, %rax" << endl;
            break;
        }
        case '-': {
            exp->right->accept(this);
            cout << " movq %rax, %rcx" << endl;
            cout << " popq %rax" << endl;
            cout << " subq %rcx, %rax" << endl;
            break;
        }
        case '*': {
            exp->right->accept(this);
            cout << " movq %rax, %rcx" << endl;
            cout << " popq %rax" << endl;
            cout << " mulq %rcx, %rax" << endl;
            break;
        }
        case '<': {
            exp->right->accept(this);
            cout << " movq %rax, %rcx" << endl;
            cout << " popq %rax" << endl;
            cout << " cmpq %rcx, %rax" << endl;
            cout << " movl $0, %eax" << endl;
            cout << " setl %al" << endl;
            cout << " movzbq %al, %rax" << endl;
            break;
        }
    }
    return 0;
}

int GenCodeVisitor::visit(NumberExp *exp) {
    cout << " movq $" << exp->value << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(BoolExp *exp) { return 0; }

int GenCodeVisitor::visit(IdentifierExp *exp) {
    cout << " movq " << memoria[exp->name] << "(%rbp), %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(AssignStatement *stm) {
    stm->rhs->accept(this);
    cout << " movq %rax," << memoria[stm->id] << " (%rbp)" << endl;
    return 0;
}

int GenCodeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
    cout << " movq %rax, %rsi" << endl;
    cout << " leaq print_fmt(%rip), %rdi" << endl;
    cout << " movl $0, %eax" << endl;
    cout << " call printf@PLT" << endl;
    return 0;
}

int GenCodeVisitor::visit(IfStatement *stm) {
    const int localCounter = ifCounter;
    ifCounter++;
    stm->condition->accept(this);
    cout << " cmpq $0, %rax" << endl;
    cout << " je else_" << localCounter << endl;
    stm->then->accept(this);
    cout << " jmp endif_" << localCounter << endl;
    cout << " else_" << localCounter << ':' << endl;
    stm->els->accept(this);
    cout << " endif_" << localCounter << ':' << endl;
    return 0;
}

int GenCodeVisitor::visit(WhileStatement *stm) {
    const int localCounter = whileCounter;
    whileCounter++;
    cout << " while_" << localCounter << ':' << endl;
    stm->condition->accept(this);
    cout << " cmpq $0, %rax" << endl;
    cout << " je endwhile_" << localCounter << endl;
    stm->body->accept(this);
    cout << " jmp while_" << localCounter << endl;
    cout << " endwhile_" << localCounter << ':' << endl;
    return 0;
}

int GenCodeVisitor::visit(ForStatement *stm) {
    const int localCounter = forCounter;
    forCounter++;
    stm->init->accept(this);
    cout << " for_" << localCounter << ':' << endl;
    stm->condition->accept(this);
    cout << " cmpq $0, %rax" << endl;
    cout << " je endfor_" << localCounter << endl;
    stm->body->accept(this);
    stm->increment->accept(this);
    cout << " jmp for_" << localCounter << endl;
    cout << " endfor_" << localCounter << ':' << endl;
    return 0;
}

int GenCodeVisitor::visit(VarDec *stm) {
    for (const auto &i: stm->vars) {
        memoria[i] = -8 * cantidad;
        cantidad++;
    }
    return 0;
}

int GenCodeVisitor::visit(VarDecList *stm) {
    for (auto i: stm->vardecs) {
        i->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(StatementList *stm) {
    for (auto i: stm->stms) {
        i->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(Body *b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
    return 0;
}

// TypeChecker

int TypeCheckerVisitor::revisar(Program *program) {
    program->body->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(BinaryExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(NumberExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(BoolExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(IdentifierExp *exp) {
    return 0;
}

int TypeCheckerVisitor::visit(AssignStatement *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(PrintStatement *stm) {
    return 0;
}

int TypeCheckerVisitor::visit(IfStatement *stm) {
    stm->then->accept(this);
    stm->els->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(WhileStatement *stm) {
    stm->body->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(ForStatement *stm) {
    stm->body->accept(this);
    return 0;
}

int TypeCheckerVisitor::visit(VarDec *stm) {
    for (auto i: stm->vars) {
        auto it = std::find(variables.begin(), variables.end(), i);
        if (it == variables.end()) {
            variables.push_back(i);
            cantidad++;
        } else {
            cout << "Variable tiene dos declaraciones" << endl;
            exit(0);
        }
    }
    return 0;
}

int TypeCheckerVisitor::visit(VarDecList *stm) {
    for (auto i: stm->vardecs) {
        i->accept(this);
    }
    return 0;
}

int TypeCheckerVisitor::visit(StatementList *stm) {
    for (auto i: stm->stms) {
        i->accept(this);
    }
    return 0;
}

int TypeCheckerVisitor::visit(Body *b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
    return 0;
}
