#include <iostream>
#include "exp.h"
#include "visitor.h"
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

int IfStatement::accept(Visitor *visitor) {
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

int PrintVisitor::visit(UnaryExp *exp) {
    cout << Exp::unopToChar(exp->op) << ' ';
    exp->exp->accept(this);
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

int PrintVisitor::visit(IfExp *exp) {
    cout << "ifexp(";
    exp->first->accept(this);
    cout << ", ";
    exp->second->accept(this);
    cout << ", ";
    exp->third->accept(this);
    cout << ")";
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
    for (Stm *s: stm->then) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }
    if (!stm->els.empty()) cout << "else" << endl;

    for (Stm *s: stm->els) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }

    cout << "endif";
}

void PrintVisitor::visit(WhileStatement *stm) {
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    for (Stm *s: stm->body) {
        cout << "    ";
        s->accept(this);
        cout << endl;
    }
    cout << "endwhile";
}

void PrintVisitor::visit(ForStatement *stm) {
    cout << "for (";
    stm->first->accept(this);
    cout << ", ";
    stm->second->accept(this);
    cout << ", ";
    stm->third->accept(this);
    cout << ")\n";
    for (Stm *s: stm->body) {
        s->accept(this);
        cout << '\n';
    }
    cout << "endfor";
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
    switch (exp->op) {
        case PLUS_OP: result = v1 + v2;
            break;
        case MINUS_OP: result = v1 - v2;
            break;
        case MUL_OP: result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0) result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case LT_OP: result = v1 < v2;
            break;
        case LE_OP: result = v1 <= v2;
            break;
        case EQ_OP: result = v1 == v2;
            break;
        case AND_OP: result = v1 && v2;
            break;
        case OR_OP: result = v1 || v2;
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(UnaryExp *exp) {
    int result;
    int v = exp->exp->accept(this);
    switch (exp->op) {
        case NOT_OP: result = !v;
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

int EVALVisitor::visit(IfExp *exp) {
    if (exp->first->accept(this)) {
        return exp->second->accept(this);
    }
    return exp->third->accept(this);
}

void EVALVisitor::visit(AssignStatement *stm) {
    memoria[stm->id] = stm->rhs->accept(this);
}

void EVALVisitor::visit(PrintStatement *stm) {
    cout << stm->e->accept(this) << '\n';
}

void EVALVisitor::ejecutar(Program *program) {
    for (Stm *s: program->slist) {
        s->accept(this);
    }
};

void EVALVisitor::visit(IfStatement *stm) {
    if (stm->condition->accept(this)) {
        for (Stm *s: stm->then) {
            s->accept(this);
        }
    } else {
        for (Stm *s: stm->els) {
            s->accept(this);
        }
    }
}

void EVALVisitor::visit(WhileStatement *stm) {
    while (stm->condition->accept(this)) {
        for (const auto &s: stm->body) {
            s->accept(this);
        }
    }
}

void EVALVisitor::visit(ForStatement *stm) {
    int a = stm->first->accept(this);
    int b = stm->second->accept(this);
    int c = stm->third->accept(this);
    for (int i = a; i <= b; i += c) {
        for (const auto &s: stm->body) {
            s->accept(this);
        }
    }
}
