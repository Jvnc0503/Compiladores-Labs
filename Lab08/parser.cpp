#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

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
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner *sc): scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program *Parser::parseProgram() {
    Program *p = new Program();
    try {
        while (!isAtEnd()) {
            p->add(parseStatement());
            if (!isAtEnd() && !match(Token::PC)) {
                throw runtime_error("Error: se esperaba ';' al final de la declaración.");
            }
        }
    } catch (const exception &e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        delete p;
        exit(1);
    }
    return p;
}

Stm *Parser::parseStatement() {
    Stm *s = NULL;
    Exp *e;

    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExpression();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExpression();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    } else if (match(Token::IF)) {
        e = parseCExpression();
        if (!match(Token::THEN)) {
            throw runtime_error("Error: se esperaba 'then' después de la expresión.");
        }
        std::list<Stm *> thenStms;
        while (!isAtEnd() && !check(Token::ELSE) && !check(Token::ENDIF)) {
            thenStms.push_back(parseStatement());
        }
        std::list<Stm *> elseStms;
        if (match(Token::ELSE)) {
            while (!isAtEnd() && !check(Token::ENDIF)) {
                elseStms.push_back(parseStatement());
            }
        }
        if (!match(Token::ENDIF)) {
            throw runtime_error("Error: se esperaba 'endif'.");
        }
        s = new IfStatement(e, thenStms, elseStms);
    } else {
        cout << "Error: Se esperaba un identificador, 'print' o 'if', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp *Parser::parseCExpression() {
    Exp *left = parseExpression();
    BinaryOp op;
    if (match(Token::LESS) || match(Token::LESSEQUAL) || match(Token::EQUAL)) {
        BinaryOp op;
        if (previous->type == Token::LESS) {
            op = LESS_OP;
        } else if (previous->type == Token::LESSEQUAL) {
            op = LESSEQUAL_OP;
        } else if (previous->type == Token::EQUAL) {
            op = EQUAL_OP;
        }
        Exp *right = parseExpression();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp *Parser::parseExpression() {
    Exp *left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS) {
            op = PLUS_OP;
        } else if (previous->type == Token::MINUS) {
            op = MINUS_OP;
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
            op = MUL_OP;
        } else if (previous->type == Token::DIV) {
            op = DIV_OP;
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
    } else if (match(Token::ID)) {
        return new IdentifierExp(previous->text);
    } else if (match(Token::PI)) {
        e = parseCExpression();
        if (!match(Token::PD)) {
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}

