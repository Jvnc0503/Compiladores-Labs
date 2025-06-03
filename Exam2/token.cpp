#include "token.h"
#include <iostream>

Token::Token(const Type type) : type(type) {
    text = "";
}

Token::Token(const Type type, char c) : type(type), text(1, c) {
}

Token::Token(Type type, const std::string &source, const int first, const int last) : type(type) {
    text = source.substr(first, last);
}

std::ostream &operator<<(std::ostream &outs, const Token &tok) {
    switch (tok.type) {
        case Token::VAR: outs << "TOKEN(VAR)";
            break;
        case Token::ID: outs << "TOKEN(ID)";
            break;
        case Token::COMMA: outs << "TOKEN(COMMA)";
            break;
        case Token::SEMICOLON: outs << "TOKEN(SEMICOLON)";
            break;
        case Token::MODEL: outs << "TOKEN(MODEL)";
            break;
        case Token::END: outs << "TOKEN(END)";
            break;
        case Token::EQUAL: outs << "TOKEN(EQUAL)";
            break;
        case Token::PLUS: outs << "TOKEN(PLUS)";
            break;
        case Token::MINUS: outs << "TOKEN(MINUS)";
            break;
        case Token::MUL: outs << "TOKEN(MUL)";
            break;
        case Token::INT: outs << "TOKEN(INT)";
            break;
        case Token::SOLVE: outs << "TOKEN(SOLVE)";
            break;
    }
    return outs;
}

std::ostream &operator<<(std::ostream &outs, const Token *tok) {
    return outs << *tok;
}
