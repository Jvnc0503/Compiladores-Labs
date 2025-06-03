#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "exp.h"

class Parser {
    Scanner *scanner;
    Token *current, *previous;

    bool isAtFinish() const;

    bool check(Token::Type type) const;

    bool match(Token::Type type);

    bool advance();

    VarBlock *parseVarBlock();

    ModelBlock *parseModelBlock();

    Equation *parseEquation();

    Exp *parseExp();

    Exp *parseTerm();

    Exp *parseFactor();

public:
    explicit Parser(Scanner *scanner);

    Program *parseProgram();
};


#endif //PARSER_H
