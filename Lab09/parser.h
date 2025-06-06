#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner *scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);

    bool check(Token::Type ttype);

    bool advance();

    bool isAtEnd();

    list<Stm *> parseStmList();

    Exp *parseAExp();

    Exp *parseBExp();

    Exp *parseCExp();

    Exp *parseExpression();

    Exp *parseTerm();

    Exp *parseFactor();

public:
    Parser(Scanner *scanner);

    Program *parseProgram();

    Stm *parseStatement();
};

#endif // PARSER_H
