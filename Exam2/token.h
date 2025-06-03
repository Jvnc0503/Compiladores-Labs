#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <iostream>

struct Token {
    enum Type {
        VAR, ID, COMMA, SEMICOLON, MODEL, END, EQUAL, PLUS, MINUS, MUL, NUM, SOLVE, FINISH, ERROR
    };

    Type type;
    std::string text;

    explicit Token(Type type);

    Token(Type type, char c);

    Token(Type type, const std::string &source, int first, int last);

    friend std::ostream &operator<<(std::ostream &outs, const Token &tok);

    friend std::ostream &operator<<(std::ostream &outs, const Token *tok);
};


#endif //TOKEN_H
