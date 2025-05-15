#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
ImpValue BinaryExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

ImpValue UnaryExp::accept(Visitor *visitor) {
    return ImpValue();
}

ImpValue IFExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

ImpValue NumberExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

ImpValue BoolExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

ImpValue IdentifierExp::accept(Visitor *visitor) {
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

ImpValue PrintVisitor::visit(BinaryExp *exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return ImpValue();
}

ImpValue PrintVisitor::visit(NumberExp *exp) {
    cout << exp->value;
    return ImpValue();
}

ImpValue PrintVisitor::visit(BoolExp *exp) {
    if (exp->value) cout << "true";
    else cout << "false";
    return ImpValue();
}

ImpValue PrintVisitor::visit(IdentifierExp *exp) {
    cout << exp->name;
    return ImpValue();
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


ImpValue PrintVisitor::visit(IFExp *pepito) {
    cout << "ifexp(";
    pepito->cond->accept(this);
    cout << ",";
    pepito->left->accept(this);
    cout << ",";
    pepito->right->accept(this);
    cout << ")";
    return ImpValue();
}

void PrintVisitor::visit(WhileStatement *stm) {
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
    cout << "endwhile";
}

void PrintVisitor::visit(ForStatement *stm) {
    cout << "for ";
    stm->start->accept(this);
    cout << " to ";
    stm->end->accept(this);
    cout << " step ";
    stm->step->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
    cout << "endfor";
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

///////////////////////////////////////////////////////////////////////////////////
ImpValue EVALVisitor::visit(BinaryExp *exp) {
    return ImpValue();
}

ImpValue EVALVisitor::visit(NumberExp *exp) {
    return ImpValue();
}

ImpValue EVALVisitor::visit(BoolExp *exp) {
    return ImpValue();
}

ImpValue EVALVisitor::visit(IdentifierExp *exp) {
    return ImpValue();
}

void EVALVisitor::visit(AssignStatement *stm) {
}

void EVALVisitor::visit(PrintStatement *stm) {
}

void EVALVisitor::ejecutar(Program *program) {
}

void EVALVisitor::visit(IfStatement *stm) {
}

void EVALVisitor::visit(WhileStatement *stm) {
}


ImpValue EVALVisitor::visit(IFExp *pepito) {
    return ImpValue();
}

void EVALVisitor::visit(ForStatement *stm) {
}


void EVALVisitor::visit(VarDec *stm) {
}

void EVALVisitor::visit(VarDecList *stm) {
}

void EVALVisitor::visit(StatementList *stm) {
}

void EVALVisitor::visit(Body *b) {
}

///////////////////////////////////////////////////////////////////////////////////

//0 = undefined
//1 = int
//2 = bool

void TypeVisitor::check(Program *program) {
}

ImpValue TypeVisitor::visit(BinaryExp *exp) {
    return ImpValue();
}

ImpValue TypeVisitor::visit(NumberExp *exp) {
    return ImpValue();
}

ImpValue TypeVisitor::visit(BoolExp *exp) {
    return ImpValue();
}

ImpValue TypeVisitor::visit(IdentifierExp *exp) {
    return ImpValue();
}

ImpValue TypeVisitor::visit(IFExp *exp) {
    return ImpValue();
}

void TypeVisitor::visit(AssignStatement *stm) {
}

void TypeVisitor::visit(PrintStatement *stm) {
}

void TypeVisitor::visit(IfStatement *stm) {
}

void TypeVisitor::visit(WhileStatement *stm) {
}

void TypeVisitor::visit(ForStatement *stm) {
}

void TypeVisitor::visit(VarDec *stm) {
}

void TypeVisitor::visit(VarDecList *stm) {
}

void TypeVisitor::visit(StatementList *stm) {
}

void TypeVisitor::visit(Body *b) {
}
