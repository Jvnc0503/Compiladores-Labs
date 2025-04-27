#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <list>
#include <unordered_map>

class Program;
class PrintStatement;
class AssignStatement;
class Exp;
class BinaryExp;
class NumberExp;
class IdExp;
class ParenthExp;

using namespace std;

unordered_map<string, int> memory;

class Token {
public:
    enum Type { PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, END, PC, ID, PRINT, ASSIGN };

    Type type;
    string text;

    Token(Type);

    Token(Type, char c);

    Token(Type, const string &source, int first, int last);
};

class Scanner {
private:
    string input;
    int first, current;

public:
    Scanner(const char *in_s);

    Token *nextToken();

    ~Scanner();
};

enum BinaryOp { PLUS, MINUS, MUL, DIV };

class ASTVisitor {
public:
    virtual int visit(BinaryExp *exp) = 0;

    virtual int visit(NumberExp *exp) = 0;

    virtual int visit(IdExp *exp) = 0;

    virtual int visit(ParenthExp *exp) = 0;

    virtual int visit(AssignStatement *exp) = 0;

    virtual int visit(PrintStatement *exp) = 0;

    virtual int visit(Program *p) = 0;
};

class ASTPrinter : public ASTVisitor {
public:
    void print(Exp *exp);

    int visit(BinaryExp *e);

    int visit(NumberExp *e);

    int visit(IdExp *e);

    int visit(ParenthExp *e);

    int visit(AssignStatement *e);

    int visit(PrintStatement *e);

    int visit(Program *p);
};

class ASTEvaluator : public ASTVisitor {
public:
    int eval(Exp *e);

    int visit(BinaryExp *e);

    int visit(NumberExp *e);

    int visit(IdExp *e);

    int visit(ParenthExp *e);

    int visit(AssignStatement *e);

    int visit(PrintStatement *e);

    int visit(Program *p);
};

class Exp {
public:
    virtual ~Exp() = 0;

    static char binopToChar(BinaryOp op);

    virtual int accept(ASTVisitor *visitor) = 0;
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    ~BinaryExp() override;

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }
};

class NumberExp : public Exp {
public:
    int value;

    NumberExp(int v);

    ~NumberExp() override;

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }
};

class IdExp : public Exp {
public:
    string id;

    IdExp(string id) : id(id) {
    }

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }

    ~IdExp() {
    }
};

class ParenthExp : public Exp {
public:
    Exp *exp;

    ParenthExp(Exp *exp) : exp(exp) {
    }

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }

    ~ParenthExp() {
    }
};

class Stm {
public:
    virtual int accept(ASTVisitor *visitor) = 0;

    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    string id;
    Exp *rhs;

    AssignStatement(string id, Exp *e): id(id), rhs(e) {
    }

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }

    ~AssignStatement() {
    }
};

class PrintStatement : public Stm {
public:
    Exp *e;

    PrintStatement(Exp *e): e(e) {
    }

    int accept(ASTVisitor *visitor) override {
        return visitor->visit(this);
    }

    ~PrintStatement() {
    }
};


class Program {
public:
    list<Stm *> slist;

    Program() {
    }

    void addStatement(Stm *s) {
        slist.push_back(s);
    }

    int accept(ASTVisitor *visitor) {
        return visitor->visit(this);
    }
};

class Parser {
private:
    Scanner *scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);

    bool check(Token::Type ttype);

    bool advance();

    bool isAtEnd();

    Exp *parseExpression();

    Exp *parseTerm();

    Exp *parseFactor();

    Program *parseProgram();

    Stm *parseStatement();

    bool tokenToOp(Token *tk, BinaryOp &op);

public:
    Parser(Scanner *scanner);

    Exp *parse();
};

Token::Token(Type type): type(type) { text = ""; }

Token::Token(Type type, char c): type(type) { text = c; }

Token::Token(Type type, const string &source, int first, int last): type(type) {
    text = source.substr(first, last);
}

std::ostream &operator <<(std::ostream &outs, const Token &tok) {
    if (tok.text.empty())
        return outs << tok.type;
    else
        return outs << "TOK" << "(" << tok.text << ")";
}

std::ostream &operator <<(std::ostream &outs, const Token *tok) {
    return outs << *tok;
}

// SCANNER //

Scanner::Scanner(const char *s): input(s), first(0), current(0) {
}

Token *Scanner::nextToken() {
    Token *token;
    while (input[current] == ' ') current++;
    if (input[current] == '\0') return new Token(Token::END);
    char c = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    } else if (isalpha(c)) {
        current++;
        while (isalnum(input[current])) {
            current++;
        }
        if (input.substr(first, current - first) == "print") {
            token = new Token(Token::PRINT, input, first, current - first);
        } else {
            token = new Token(Token::ID, input, first, current - first);
        }
    } else if (strchr("+-*/();=", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS, c);
                break;
            case '-': token = new Token(Token::MINUS, c);
                break;
            case '*': token = new Token(Token::MUL, c);
                break;
            case '/': token = new Token(Token::DIV, c);
                break;
            case '(': token = new Token(Token::PI, c);
                break;
            case ')': token = new Token(Token::PD, c);
                break;
            case ';': token = new Token(Token::PC, c);
                break;
            case '=': token = new Token(Token::ASSIGN, c);
                break;
            default: cout << "No deberia llegar aca" << endl;
        }
        current++;
    } else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

Scanner::~Scanner() {
}

void ASTPrinter::print(Exp *exp) {
    cout << "expression: ";
    exp->accept(this);
    cout << '\n';
}

int ASTPrinter::visit(BinaryExp *e) {
    e->left->accept(this);
    cout << ' ' << Exp::binopToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}

int ASTPrinter::visit(NumberExp *e) {
    cout << e->value;
    return 0;
}

int ASTPrinter::visit(IdExp *e) {
    return 0;
}

int ASTPrinter::visit(ParenthExp *e) {
    return 0;
}

int ASTPrinter::visit(AssignStatement *e) {
    return 0;
}

int ASTPrinter::visit(PrintStatement *e) {
    return 0;
}

int ASTPrinter::visit(Program *p) {
    for (auto s: p->slist) {
        p->accept(this);
    }
    return 0;
}

int ASTEvaluator::visit(BinaryExp *e) {
    int v1 = e->left->accept(this);
    int v2 = e->right->accept(this);
    int result = 0;
    switch (e->op) {
        case PLUS: result = v1 + v2;
            break;
        case MINUS: result = v1 - v2;
            break;
        case MUL: result = v1 * v2;
            break;
        case DIV: result = v1 / v2;
            break;
    }
    return result;
}

int ASTEvaluator::visit(NumberExp *e) {
    return e->value;
}

int ASTEvaluator::visit(IdExp *e) {
    return 0;
}

int ASTEvaluator::visit(ParenthExp *e) {
    return 0;
}

int ASTEvaluator::visit(AssignStatement *e) {
    return 0;
}

int ASTEvaluator::visit(PrintStatement *e) {
    return 0;
}

int ASTEvaluator::visit(Program *p) {
    for (auto s: p->slist) {
        s->accept(this);
    }
    return 0;
}


// PARSER //

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token *temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Parse error, unrecognised character: " << current->text << endl;
            exit(0);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner *sc): scanner(sc) {
    previous = current = NULL;
    return;
};

Exp *Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Exp *exp = parseExpression();
    if (current) delete current;
    return exp;
}

Exp *Parser::parseExpression() {
    Exp *left = parseTerm();

    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS) {
            op = PLUS;
        } else if (previous->type == Token::MINUS) {
            op = MINUS;
        }
        Exp *right = parseTerm();
        left = new BinaryExp(left, right, op);
    }

    return left;
}

Exp *Parser::parseTerm() {
    Exp *left = parseFactor();

    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL) {
            op = MUL;
        } else if (previous->type == Token::DIV) {
            op = DIV;
        }
        Exp *right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp *Parser::parseFactor() {
    Exp *e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    if (match(Token::PI)) {
        e = parseExpression();
        if (!match(Token::PD)) {
            cout << "Falta parentesis derecho" << endl;
        }
        return e;
    }
    cout << "Error: se esperaba un número." << endl;
    exit(0);
}

Program *Parser::parseProgram() {
}

Stm *Parser::parseStatement() {
}

char Exp::binopToChar(BinaryOp op) {
    char c = ' ';
    switch (op) {
        case PLUS: c = '+';
            break;
        case MINUS: c = '-';
            break;
        case MUL: c = '*';
            break;
        case DIV: c = '/';
            break;
        default: c = '$';
    }
    return c;
}

// AST //


BinaryExp::BinaryExp(Exp *l, Exp *r, BinaryOp op): left(l), right(r), op(op) {
}

NumberExp::NumberExp(int v): value(v) {
}

Exp::~Exp() {
}

BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

NumberExp::~NumberExp() {
}


void test_scanner(Scanner *scanner) {
    Token *current;
    current = scanner->nextToken();
    while (current->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - caracter invalido: " << current->text << endl;
            break;
        } else
            cout << current << endl;
        current = scanner->nextToken();
    }
    exit(1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }

    Scanner scanner(argv[1]);

    //test_scanner(&scanner);

    Parser parser(&scanner);

    Exp *exp = parser.parse();
    ASTPrinter printer;
    printer.print(exp);
    ASTEvaluator evaluator;
    cout << evaluator.eval(exp);
    delete exp;
}
