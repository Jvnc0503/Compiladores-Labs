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

void PrintVisitor::visit(AssignStatement *stm) {
    printTabs();
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement *stm) {
    printTabs();
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement *stm) {
    printTabs();
    cout << "if ";
    stm->condition->accept(this);
    cout << " then\n";
    tabs++;
    stm->then->accept(this);
    tabs--;
    if (stm->els) {
        printTabs();
        cout << "else" << endl;
        tabs++;
        stm->els->accept(this);
        tabs--;
    }
    printTabs();
    cout << "endif";
}

void PrintVisitor::printTabs() const {
    for (unsigned i = 0; i < tabs; i++) cout << "   ";
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

void PrintVisitor::visit(WhileStatement *stm) {
    printTabs();
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    tabs++;
    stm->b->accept(this);
    tabs--;
    cout << "endwhile";
}

void PrintVisitor::visit(ForStatement *stm) {
    printTabs();
    cout << "for ";
    stm->start->accept(this);
    cout << " to ";
    stm->end->accept(this);
    cout << " step ";
    stm->step->accept(this);
    cout << " do" << endl;
    tabs++;
    stm->b->accept(this);
    tabs--;
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
    for (const auto i: stm->vardecs) {
        printTabs();
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList *stm) {
    for (const auto i: stm->stms) {
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body *stm) {
    stm->vardecs->accept(this);
    stm->slist->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp *exp) {
    int result;
    const int v1 = exp->left->accept(this);
    const int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP: result = v1 + v2;
            break;
        case MINUS_OP: result = v1 - v2;
            break;
        case MUL_OP: result = v1 * v2;
            break;
        case DIV_OP: result = v1 / v2;
            break;
        case LT_OP: result = v1 < v2;
            break;
        case LE_OP: result = v1 <= v2;
            break;
        case EQ_OP: result = v1 == v2;
            break;
    }
    return result;
}

int EVALVisitor::visit(NumberExp *exp) {
    return exp->value;
}

int EVALVisitor::visit(BoolExp *exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp *exp) {
    return env.lookup(exp->name);
}

void EVALVisitor::visit(AssignStatement *stm) {
    env.update(stm->id, stm->rhs->accept(this));
}

void EVALVisitor::visit(PrintStatement *stm) {
    cout << stm->e->accept(this) << '\n';
}

void EVALVisitor::ejecutar(Program *program) {
    program->body->accept(this);
}

void EVALVisitor::visit(IfStatement *stm) {
    if (stm->condition->accept(this)) {
        stm->then->accept(this);
    } else {
        stm->els->accept(this);
    }
}

void EVALVisitor::visit(WhileStatement *stm) {
    while (stm->condition->accept(this)) {
        stm->b->accept(this);
    }
}

int EVALVisitor::visit(IFExp *pepito) {
    return 0;
}

void EVALVisitor::visit(ForStatement *stm) {
}

void EVALVisitor::visit(VarDec *stm) {
    for (const auto &i: stm->vars) {
        env.add_var(i, stm->type);
    }
}

void EVALVisitor::visit(VarDecList *stm) {
    for (const auto &i: stm->vardecs) {
        i->accept(this);
    }
}

void EVALVisitor::visit(StatementList *stm) {
    for (const auto &i: stm->stms) {
        i->accept(this);
    }
}

void EVALVisitor::visit(Body *b) {
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}

///////////////////////////////////////////////////////////////////////////////////

//0 = undefined
//1 = int
//2 = bool

void TypeVisitor::check(Program *program) {
    program->body->accept(this);
}

int TypeVisitor::visit(BinaryExp *exp) {
    int result;
    const int v1 = exp->left->accept(this);
    const int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP: result = 1;
            break;
        case MINUS_OP: result = 1;
            break;
        case MUL_OP: result = 1;
            break;
        case DIV_OP: result = 1;
            break;
        case LT_OP: result = 2;
            break;
        case LE_OP: result = 2;
            break;
        case EQ_OP: result = 2;
            break;
    }
    return result;
}

int TypeVisitor::visit(NumberExp *exp) {
    return 1;
}

int TypeVisitor::visit(BoolExp *exp) {
    return 2;
}

int TypeVisitor::visit(IdentifierExp *exp) {
    if (!env.check(exp->name)) {
        cout << "Variable no declarada: " << exp->name << '\n';
        exit(0);
    }
    return 0;
}

int TypeVisitor::visit(IFExp *exp) {
    return 0;
}

void TypeVisitor::visit(AssignStatement *stm) {
    if (!env.check(stm->id)) {
        cout << "Variable no declarada: " << stm->id << '\n';
    }
    string val_type;
    switch (stm->rhs->accept(this)) {
        case 1: val_type = "int";
            break;
        case 2: val_type = "bool";
            break;
    }
    if (env.lookup_type(stm->id) != val_type) {
        cout << "No coinciden los tipos de la asignacion\n";
        exit(0);
    }
}

void TypeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
}

void TypeVisitor::visit(IfStatement *stm) {
    if (stm->condition->accept(this) != 2) {
        cout << "Error de tipo: se esperaba bool en la condicion del if\n";
        exit(0);
    }
}

void TypeVisitor::visit(WhileStatement *stm) {
}

void TypeVisitor::visit(ForStatement *stm) {
}

void TypeVisitor::visit(VarDec *stm) {
    if (stm->type == "int") {
        for (const auto &i: stm->vars) {
            env.add_var(i, "int");
        }
    } else if (stm->type == "bool") {
        for (const auto &i: stm->vars) {
            env.add_var(i, "bool");
        }
    } else {
        cout << "Tipo no reconocido\n";
        exit(0);
    }
}

void TypeVisitor::visit(VarDecList *stm) {
    for (const auto &i: stm->vardecs) {
        i->accept(this);
    }
}

void TypeVisitor::visit(StatementList *stm) {
    for (const auto &i: stm->stms) {
        i->accept(this);
    }
}

void TypeVisitor::visit(Body *b) {
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}
