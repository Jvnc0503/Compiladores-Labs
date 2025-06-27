#include "preprocessor.h"
#include <iostream>
#include <filesystem>

bool Preprocessor::match(const Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Preprocessor::check(const Token::Type ttype) const {
    return !isAtEnd() && current->type == ttype;
}

bool Preprocessor::advance() {
    if (!isAtEnd()) {
        Token *temp = current;
        delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            std::cerr << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Preprocessor::isAtEnd() const {
    return current->type == Token::END;
}

#define filename "../processed.txt"

Preprocessor::Preprocessor(Scanner *scanner) : scanner(scanner), current(scanner->nextToken()), previous(nullptr) {
    if (current->type == Token::ERR) {
        std::cerr << "Error en el primer token: " << current->text << '\n';
        exit(1);
    }
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
    out = std::ofstream(filename);
}

Program *Preprocessor::processProgram() {
    auto *program = new Program();
    program->vardecs = parseVarDecList();
    program->fundecs = parseFunDecList();
    return program;
}

VarDecList *Preprocessor::parseVarDecList() {
    auto *list = new VarDecList();
    auto *temp = parseVarDec();
    while (temp) {
        list->add(temp);
        temp = parseVarDec();
    }
    return list;
}

VarDec *Preprocessor::parseVarDec() {
    VarDec *vd = nullptr;
    if (match(Token::VAR)) {
        out << "var ";
        if (!match(Token::ID)) {
            cerr << "Error: se esperaba un identificador después de 'var'.\n";
            exit(1);
        }
        string type = previous->text;
        out << previous->text << ' ';
        list<string> ids;
        if (!match(Token::ID)) {
            cerr << "Error: se esperaba un identificador después de 'var'.\n";
            exit(1);
        }
        ids.push_back(previous->text);
        out << previous->text;
        while (match(Token::COMA)) {
            out << ", ";
            if (!match(Token::ID)) {
                cerr << "Error: se esperaba un identificador después de ','.\n";
                exit(1);
            }
            ids.push_back(previous->text);
            out << previous->text;
        }
        if (!match(Token::PC)) {
            cerr << "Error: se esperaba un ';' al final de la declaración.\n";
            exit(1);
        }
        out << ";\n";
        vd = new VarDec(type, ids);
    }
    return vd;
}

FunDecList *Preprocessor::parseFunDecList() {
    auto *list = new FunDecList();
    auto *temp = parseFunDec();
    while (temp) {
        list->add(temp);
        temp = parseFunDec();
    }
    return list;
}

FunDec *Preprocessor::parseFunDec() {
    FunDec *vd = nullptr;
    if (match(Token::FUN)) {
        out << "fun ";
        auto *fu = new FunDec();
        match(Token::ID);
        fu->tipo = previous->text;
        out << previous->text << ' ';
        match(Token::ID);
        fu->nombre = previous->text;
        out << previous->text << ' ';
        match(Token::PI);
        out << '(';
        while (match(Token::ID)) {
            fu->tipos.push_back(previous->text);
            out << previous->text << ' ';
            match(Token::ID);
            fu->parametros.push_back(previous->text);
            out << previous->text;
            if (!match(Token::COMA)) break;
            out << ", ";
        }
        match(Token::PD);
        out << ")\n";
        fu->cuerpo = parseBody();
        match(Token::ENDFUN);
        out << "endfun \n";
        vd = fu;
    }
    return vd;
}

