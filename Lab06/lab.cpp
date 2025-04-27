#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include <list>
#include <unordered_map>
using namespace std;
unordered_map<string, int> memoria;

class Token {
public:
    enum Type { PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, END, PC, ID, PRINT, ASSIGN, PLUSASSIGN, POW, SQRT };

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

enum BinaryOp { PLUS, MINUS, MUL, DIV, POW };

class Exp {
public:
    virtual void print() = 0;

    virtual int eval() = 0;

    virtual ~Exp() = 0;

    static char binopToChar(BinaryOp op);
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp *l, Exp *r, BinaryOp op);

    void print();

    int eval();

    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;

    NumberExp(int v);

    void print();

    int eval();

    ~NumberExp();
};

class IdExp : public Exp {
public:
    string id;

    IdExp(string v);

    void print();

    int eval();

    ~IdExp();
};

IdExp::IdExp(string v) { id = v; }

IdExp::~IdExp() {
}

void IdExp::print() {
}

int IdExp::eval() {
    return memoria[id];
}

class SqrtExp : public Exp {
    Exp *e;

public:
    ~SqrtExp();

    SqrtExp(Exp *e);

    void print();

    int eval();
};

SqrtExp::SqrtExp(Exp *e) {
    this->e = e;
}

SqrtExp::~SqrtExp() {
    delete e;
}

void SqrtExp::print() {
}

int SqrtExp::eval() {
    int v = e->eval();
    if (v < 0) {
        cout << "Error: raiz de un numero negativo" << endl;
        exit(0);
    }
    return sqrt(v);
}

class Stm {
public:
    virtual void execute() =0;

    virtual ~Stm() = 0;
};

Stm::~Stm() = default;

class AssignStatement : public Stm {
    string id;
    Exp *rhs;

public:
    void execute();

    AssignStatement(string id, Exp *e);

    ~AssignStatement();
};

void AssignStatement::execute() {
    memoria[id] = rhs->eval();
}

AssignStatement::AssignStatement(std::string ide, Exp *e) {
    id = ide;
    rhs = e;
}

AssignStatement::~AssignStatement() {
    delete rhs;
}

class PlusAssignStatement : public Stm {
    string id;
    Exp *rhs;

public:
    void execute();

    PlusAssignStatement(string id, Exp *e);

    ~PlusAssignStatement();
};

PlusAssignStatement::PlusAssignStatement(std::string ide, Exp *e) {
    id = ide;
    rhs = e;
}

PlusAssignStatement::~PlusAssignStatement() {
    delete rhs;
}

void PlusAssignStatement::execute() {
    if (memoria.find(id) == memoria.end()) {
        cout << "Variable no declarada: " << id << '\n';
        exit(0);
    }
    memoria[id] += rhs->eval();
}

class PrintStatement : public Stm {
    Exp *e;

public:
    void execute();

    PrintStatement(Exp *e);

    ~PrintStatement();
};

void PrintStatement::execute() {
    cout << e->eval() << endl;
}

PrintStatement::PrintStatement(Exp *ee) {
    e = ee;
}

PrintStatement::~PrintStatement() {
    delete e;
}

class Program {
    list<Stm *> slist;

public:
    Program();

    void add(Stm *s);

    void interprete();

    ~Program();
};

void Program::interprete() {
    for (auto it: slist) {
        it->execute();
    }
}

Program::Program() {
}

Program::~Program() {
    for (auto it = slist.begin(); it != slist.end(); ++it) {
        delete *it;
    }
    slist.clear();
}

void Program::add(Stm *s) {
    slist.push_back(s);
}

////////////////////////////////


class Parser {
    Scanner *scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);

    bool check(Token::Type ttype);

    bool advance();

    bool isAtEnd();

    Exp *parseExpression();

    Exp *parseTerm();

    Exp *parseFexp();

    Exp *parseFactor();

    Stm *parseStm();

    Program *parseProgram();

    bool tokenToOp(Token *tk, BinaryOp &op);

public:
    Parser(Scanner *scanner);

    Program *parse();
};

////////////////////////////////////////////////


Token::Token(Type type): type(type) { text = ""; }

Token::Token(Type type, char c): type(type) { text = c; }

Token::Token(Type type, const string &source, int first, int last): type(type) {
    text = source.substr(first, last);
}

std::ostream &operator <<(std::ostream &outs, const Token &tok) {
    if (tok.text.empty())
        return outs << tok.type;
    else
        return outs << "TOK" << tok.type << "(" << tok.text << ")";
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
        while (isalnum(input[current]))
            current++;
        if ("print" == input.substr(first, current - first)) {
            token = new Token(Token::PRINT, input, first, current - first);
        } else if ("sqrt" == input.substr(first, current - first)) {
            token = new Token(Token::SQRT, input, first, current - first);
        } else {
            token = new Token(Token::ID, input, first, current - first);
        }
    } else if (strchr("+-*/();=", c)) {
        switch (c) {
            case '+': current++;
                if (input[current] == '=') {
                    token = new Token(Token::PLUSASSIGN, input, first, current - first);
                } else {
                    token = new Token(Token::PLUS, c);
                    current--;
                }
                break;
            case '-': token = new Token(Token::MINUS, c);
                break;
            case '*': current++;
                if (input[current] == '*') {
                    token = new Token(Token::POW, input, first, current - first);
                } else {
                    token = new Token(Token::MUL, c);
                    current--;
                }
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

Program *Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Program *p = parseProgram();
    if (current) delete current;
    return p;
}

Program *Parser::parseProgram() {
    Program *p = new Program();
    p->add(parseStm());
    while (match(Token::PC)) {
        p->add(parseStm());
    }
    return p;
}

Stm *Parser::parseStm() {
    Exp *e;
    string id;
    if (match(Token::ID)) {
        id = previous->text;
        if (match(Token::ASSIGN)) {
            e = parseExpression();
            return new AssignStatement(id, e);
        }
        if (match(Token::PLUSASSIGN)) {
            e = parseExpression();
            return new PlusAssignStatement(id, e);
        }
    } else if (match(Token::PRINT)) {
        match(Token::PI);
        e = parseExpression();
        match(Token::PD);
        return new PrintStatement(e);
    }
    cout << "ERROR";
    exit(0);
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
    Exp *left = parseFexp();

    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL) {
            op = MUL;
        } else if (previous->type == Token::DIV) {
            op = DIV;
        }
        Exp *right = parseFexp();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp *Parser::parseFexp() {
    Exp *left = parseFactor();
    if (match(Token::POW)) {
        Exp *right = parseFactor();
        return new BinaryExp(left, right, POW);
    }
    return left;
}

Exp *Parser::parseFactor() {
    Exp *e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    if (match(Token::PI)) {
        e = parseExpression();
        if (!match(Token::PD)) {
            cout << "Falta parentesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    if (match(Token::SQRT)) {
        match(Token::PI);
        e = parseExpression();
        match(Token::PD);
        return new SqrtExp(e);
    }
    cout << "Error: se esperaba un número." << endl;
    exit(0);
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


void BinaryExp::print() {
    left->print();
    char c = binopToChar(this->op);;
    cout << ' ' << c << ' ';
    right->print();
}


void NumberExp::print() {
    cout << value;
}

int BinaryExp::eval() {
    int result;
    int v1 = left->eval();
    int v2 = right->eval();
    switch (this->op) {
        case PLUS: result = v1 + v2;
            break;
        case MINUS: result = v1 - v2;
            break;
        case MUL: result = v1 * v2;
            break;
        case DIV:
            if (v2 == 0) {
                cout << "Error: división por cero\n";
                exit(0);
            }
            result = v1 / v2;
            break;
        case POW: result = pow(v1, v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int NumberExp::eval() {
    return value;
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
    ifstream infile(argv[1]);
    if (!infile) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input((istreambuf_iterator<char>(infile)),
                 istreambuf_iterator<char>());

    infile.close();

    Scanner scanner(input.c_str());

    //test_scanner(&scanner);

    Parser parser(&scanner);

    Program *p = parser.parse();

    cout << "Ejecucion:" << endl;
    p->interprete();
    cout << endl;

    delete p;
}
