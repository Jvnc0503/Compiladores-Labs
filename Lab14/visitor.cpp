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

int FunctionCallExp::accept(Visitor *visitor) {
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

int ReturnStatement::accept(Visitor *visitor) {
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

int FunDec::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

int FunDecList::accept(Visitor *visitor) {
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

int PrintVisitor::visit(FunctionCallExp *exp) {
    cout << exp->name << "(";
    for (const auto &i: exp->args) {
        i->accept(this);
        if (i != exp->args.back()) cout << ", ";
    }
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
    stm->then->accept(this);
    if (stm->els) {
        cout << "else" << endl;
        stm->els->accept(this);
    }
    cout << "endif";
}

void PrintVisitor::imprimir(Program *program) {
    program->vardecs->accept(this);
    program->fundecs->accept(this);
}

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
    cout << "while ";
    stm->condition->accept(this);
    cout << " do\n";
    stm->b->accept(this);
    cout << "endwhile";
}

void PrintVisitor::visit(ReturnStatement *stm) {
    cout << "return ";
    stm->e->accept(this);
}

void PrintVisitor::visit(VarDec *stm) {
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for (const auto &i: stm->vars) {
        cout << i;
        if (i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList *stm) {
    for (const auto &i: stm->vardecs) {
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(FunDec *stm) {
    cout << "fun " << stm->type << ' ' << stm->name << '(';
    for (const auto &i: stm->params) {
        cout << i;
        if (i != stm->params.back()) cout << ", ";
    }
    cout << ")\n";
    stm->body->accept(this);
    cout << "endfun\n";
}

void PrintVisitor::visit(FunDecList *stm) {
    for (const auto &i: stm->fundecs) {
        i->accept(this);
        cout << '\n';
    }
}

void PrintVisitor::visit(StatementList *stm) {
    for (const auto &i: stm->stms) {
        i->accept(this);
        cout << '\n';
    }
}

void PrintVisitor::visit(Body *stm) {
    stm->vardecs->accept(this);
    stm->slist->accept(this);
}

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
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
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
    if (env.check(exp->name)) {
        return env.lookup(exp->name);
    }
    cout << "Variable no declarada: " << exp->name << endl;
    return 0;
}

int EVALVisitor::visit(FunctionCallExp *exp) {
    env.add_level();
    retcall = false;
    FunDec *f = functions[exp->name];
    if (f == nullptr) {
        cout << "Error: función no declarada: " << exp->name << endl;
        exit(0);
    }
    if (f->params.size() != exp->args.size()) {
        cout << "Error: número de argumentos incorrecto para la función " << exp->name << endl;
        exit(0);
    }
    for (size_t i = 0; i < f->params.size(); ++i) {
        env.add_var(f->params[i], f->type);
    }
    for (size_t i = 0; i < exp->args.size(); ++i) {
        env.update(f->params[i], exp->args[i]->accept(this));
    }
    f->body->accept(this);
    env.remove_level();
    if (!retcall) {
        cout << "Error: función no retorna: " << exp->name << endl;
        exit(0);
    }
    return retval;
}

void EVALVisitor::visit(AssignStatement *stm) {
    if (!env.check(stm->id)) {
        cout << "Variable no declarada: " << stm->id << endl;
        return;
    }

    int value = stm->rhs->accept(this);
    env.update(stm->id, value);
}

void EVALVisitor::visit(PrintStatement *stm) {
    cout << stm->e->accept(this);
    cout << endl;
}

void EVALVisitor::ejecutar(Program *program) {
    env.add_level();
    program->vardecs->accept(this);
    program->fundecs->accept(this);
    if (!functions.contains("main")) {
        exit(0);
    }
    FunDec *main = functions["main"];
    retcall = false;
    main->body->accept(this);
    env.remove_level();
    if (!retcall) {
        cout << "Error: main no retorna\n";
        exit(1);
    }
}

void EVALVisitor::visit(IfStatement *stm) {
    if (stm->condition->accept(this)) {
        stm->then->accept(this);
    } else {
        if (stm->els) stm->els->accept(this);
    }
}

void EVALVisitor::visit(WhileStatement *stm) {
    while (stm->condition->accept(this)) {
        stm->b->accept(this);
    }
}

void EVALVisitor::visit(ReturnStatement *stm) {
    retcall = true;
    retval = stm->e->accept(this);
}

int EVALVisitor::visit(IFExp *exp) {
    if (exp->cond->accept(this)) {
        return exp->left->accept(this);
    }
    return exp->right->accept(this);
}

void EVALVisitor::visit(VarDec *stm) {
    list<string>::iterator it;
    for (it = stm->vars.begin(); it != stm->vars.end(); it++) {
        env.add_var(*it, stm->type);
    }
}

void EVALVisitor::visit(VarDecList *stm) {
    list<VarDec *>::iterator it;
    for (it = stm->vardecs.begin(); it != stm->vardecs.end(); it++) {
        (*it)->accept(this);
    }
}

void EVALVisitor::visit(FunDec *stm) {
    functions[stm->name] = stm;
}

void EVALVisitor::visit(FunDecList *stm) {
    for (const auto &i: stm->fundecs) {
        i->accept(this);
    }
}

void EVALVisitor::visit(StatementList *stm) {
    list<Stm *>::iterator it;
    for (it = stm->stms.begin(); it != stm->stms.end(); it++) {
        (*it)->accept(this);
    }
}

void EVALVisitor::visit(Body *b) {
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}
