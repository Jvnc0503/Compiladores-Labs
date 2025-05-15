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
    return visitor->visit(this);
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

ImpValue PrintVisitor::visit(UnaryExp *exp) {
    cout << Exp::unaryopToChar(exp->op) << ' ';
    exp->exp->accept(this);
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
    cout << "for " << stm->id << " in range(";
    stm->start->accept(this);
    cout << ", ";
    stm->end->accept(this);
    cout << ", ";
    stm->step->accept(this);
    cout << ")";
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
    ImpValue result(exp->type);
    const ImpValue left = exp->left->accept(this);
    const ImpValue right = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result.int_value = left.int_value + right.int_value;
            break;
        case MINUS_OP:
            result.int_value = left.int_value - right.int_value;
            break;
        case MUL_OP:
            result.int_value = left.int_value * right.int_value;
            break;
        case DIV_OP:
            result.int_value = left.int_value / right.int_value;
            break;
        case AND_OP:
            result.bool_value = left.bool_value && right.bool_value;
            break;
        case OR_OP:
            result.bool_value = left.bool_value || right.bool_value;
            break;
        default:
            cout << "Error: operador no soportado en la expresión binaria\n";
            exit(1);
    }
    return result;
}

ImpValue EVALVisitor::visit(UnaryExp *exp) {
    ImpValue result = exp->exp->accept(this);
    result.type = exp->type;
    switch (exp->op) {
        case NOT_OP:
            result.bool_value = !result.bool_value;
            break;
        default:
            cout << "Error: operador no soportado en la expresión unaria\n";
            exit(1);
    }
    return result;
}

ImpValue EVALVisitor::visit(NumberExp *exp) {
    return ImpValue("int", exp->value);
}

ImpValue EVALVisitor::visit(BoolExp *exp) {
    return ImpValue("bool", 0, exp->value);
}

ImpValue EVALVisitor::visit(IdentifierExp *exp) {
    if (!env.check(exp->name)) {
        cout << "Error: variable no declarada: " << exp->name << '\n';
        exit(1);
    }
    const string type = env.lookup_type(exp->name);
    if (type == "int") {
        return ImpValue("int", env.lookup(exp->name));
    }
    if (type == "bool") {
        return ImpValue("bool", 0, env.lookup(exp->name));
    }
    cout << "Error: tipo de variable no soportado: " << exp->name << '\n';
    exit(1);
}

void EVALVisitor::visit(AssignStatement *stm) {
    if (!env.check(stm->id)) {
        cout << "Error: variable no declarada: " << stm->id << '\n';
        exit(1);
    }
    ImpValue result = stm->rhs->accept(this);
    if (result.type == "int") {
        env.update(stm->id, result.int_value);
    } else if (result.type == "bool") {
        env.update(stm->id, result.bool_value);
    } else {
        cout << "Error: tipo de variable no soportado: " << stm->id << '\n';
        exit(1);
    }
}

void EVALVisitor::visit(PrintStatement *stm) {
    ImpValue result = stm->e->accept(this);
    if (result.type == "int") {
        cout << result.int_value << '\n';
    } else if (result.type == "bool") {
        cout << (result.bool_value ? "true" : "false") << '\n';
    } else {
        cout << "Error: tipo de variable no soportado en print\n";
        exit(1);
    }
}

void EVALVisitor::ejecutar(Program *program) {
    program->body->accept(this);
}

void EVALVisitor::visit(IfStatement *stm) {
    if (stm->condition->accept(this).bool_value) {
        stm->then->accept(this);
    } else {
        stm->els->accept(this);
    }
}

void EVALVisitor::visit(WhileStatement *stm) {
    while (stm->condition->accept(this).bool_value) {
        stm->b->accept(this);
    }
}

ImpValue EVALVisitor::visit(IFExp *pepito) {
    ImpValue condition = pepito->cond->accept(this);
    ImpValue left = pepito->left->accept(this);
    ImpValue right = pepito->right->accept(this);
    ImpValue result(left.type);
    if (condition.bool_value) {
        if (left.type == "int") {
            result.int_value = left.int_value;
        } else if (left.type == "bool") {
            result.bool_value = left.bool_value;
        }
    } else {
        if (right.type == "int") {
            result.int_value = right.int_value;
        } else if (right.type == "bool") {
            result.bool_value = right.bool_value;
        }
    }
    return result;
}

void EVALVisitor::visit(ForStatement *stm) {
    ImpValue start = stm->start->accept(this);
    ImpValue end = stm->end->accept(this);
    ImpValue step = stm->step->accept(this);
    env.add_level();
    env.add_var(stm->id, start.int_value, "int");
    while (env.lookup(stm->id) < end.int_value) {
        stm->b->accept(this);
        env.update(stm->id, env.lookup(stm->id) + step.int_value);
    }
    env.remove_level();
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

ImpValue TypeVisitor::visit(BinaryExp *exp) {
    string left_type = exp->left->accept(this).type;
    string right_type = exp->right->accept(this).type;
    string op_type = exp->type;
    if (left_type != right_type) {
        cout << "Error: tipos incompatibles en la expresión binaria\n";
        exit(1);
    }
    if (left_type != op_type) {
        cout << "Error: tipo incompatible en la expresión binaria\n";
        exit(1);
    }
    ImpValue result;
    if (op_type == "bool") {
        result.type = "bool";
    } else if (op_type == "int") {
        result.type = "int";
    }
    return result;
}

ImpValue TypeVisitor::visit(UnaryExp *exp) {
    string exp_type = exp->exp->accept(this).type;
    string op_type = exp->type;
    if (exp_type != op_type) {
        cout << "Error: tipo incompatible en la expresión unaria\n";
        exit(1);
    }
    ImpValue result;
    if (op_type == "bool") {
        result.type = "bool";
    }
    return result;
}

ImpValue TypeVisitor::visit(NumberExp *exp) {
    return {"int"};
}

ImpValue TypeVisitor::visit(BoolExp *exp) {
    return {"bool"};
}

ImpValue TypeVisitor::visit(IdentifierExp *exp) {
    if (!env.check(exp->name)) {
        cout << "Error: variable no declarada: " << exp->name << '\n';
        exit(1);
    }
    string type = env.lookup_type(exp->name);
    return {type};
}

ImpValue TypeVisitor::visit(IFExp *exp) {
    string exp_type = exp->cond->accept(this).type;
    if (exp_type != "bool") {
        cout << "Error: tipo incompatible en la expresión condicional\n";
        exit(1);
    }
    string right_type = exp->right->accept(this).type;
    string left_type = exp->left->accept(this).type;
    if (right_type != left_type) {
        cout << "Error: tipos incompatibles en la expresión if\n";
        exit(1);
    }
    return {right_type};
}

void TypeVisitor::visit(AssignStatement *stm) {
    if (!env.check(stm->id)) {
        cout << "Error: variable no declarada: " << stm->id << '\n';
        exit(1);
    }
    string var_type = env.lookup_type(stm->id);
    string exp_type = stm->rhs->accept(this).type;
    if (var_type != exp_type) {
        cout << "Error: tipo incompatible en la asignación\n";
        exit(1);
    }
}

void TypeVisitor::visit(PrintStatement *stm) {
    stm->e->accept(this);
}

void TypeVisitor::visit(IfStatement *stm) {
    string cond_type = stm->condition->accept(this).type;
    if (cond_type != "bool") {
        cout << "Error: tipo incompatible en la condición del if\n";
        exit(1);
    }
}

void TypeVisitor::visit(WhileStatement *stm) {
    string cond_type = stm->condition->accept(this).type;
    if (cond_type != "bool") {
        cout << "Error: tipo incompatible en la condición del while\n";
        exit(1);
    }
    stm->b->accept(this);
}

void TypeVisitor::visit(ForStatement *stm) {
    string start_type = stm->start->accept(this).type;
    string end_type = stm->end->accept(this).type;
    string step_type = stm->step->accept(this).type;
    if (start_type != "int" || end_type != "int" || step_type != "int") {
        cout << "Error: tipos incompatibles en el for\n";
        exit(1);
    }
    env.add_level();
    env.add_var(stm->id, 0, "int");
    stm->b->accept(this);
    env.remove_level();
}

void TypeVisitor::visit(VarDec *stm) {
    for (const auto &i: stm->vars) {
        if (env.check(i)) {
            cout << "Error: variable ya declarada: " << i << '\n';
            exit(1);
        }
        env.add_var(i, 0, stm->type);
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
