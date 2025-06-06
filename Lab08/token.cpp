#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type): type(type) { text = ""; }

Token::Token(Type type, char c): type(type) { text = string(1, c); }

Token::Token(Type type, const string &source, int first, int last): type(type) {
    text = source.substr(first, last);
}

std::ostream &operator <<(std::ostream &outs, const Token &tok) {
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)";
            break;
        case Token::MINUS: outs << "TOKEN(MINUS)";
            break;
        case Token::MUL: outs << "TOKEN(MUL)";
            break;
        case Token::DIV: outs << "TOKEN(DIV)";
            break;
        case Token::NUM: outs << "TOKEN(NUM)";
            break;
        case Token::ERR: outs << "TOKEN(ERR)";
            break;
        case Token::PD: outs << "TOKEN(PD)";
            break;
        case Token::PI: outs << "TOKEN(PI)";
            break;
        case Token::END: outs << "TOKEN(END)";
            break;
        case Token::ID: outs << "TOKEN(ID)";
            break;
        case Token::PRINT: outs << "TOKEN(PRINT)";
            break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)";
            break;
        case Token::PC: outs << "TOKEN(PC)";
            break;
        case Token::LESS: outs << "TOKEN(LESS)";
            break;
        case Token::LESSEQUAL: outs << "TOKEN(LESSEQUAL)";
            break;
        case Token::EQUAL: outs << "TOKEN(EQUAL)";
            break;
        case Token::IF: outs << "TOKEN(IF)";
            break;
        case Token::THEN: outs << "TOKEN(THEN)";
            break;
        case Token::ELSE: outs << "TOKEN(ELSE)";
            break;
        case Token::ENDIF: outs << "TOKEN(ENDIF)";
            break;
        default: outs << "TOKEN(UNKNOWN)";
            break;
    }
    return outs;
}

std::ostream &operator <<(std::ostream &outs, const Token *tok) {
    return outs << *tok;
}
