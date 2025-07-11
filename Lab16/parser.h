#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "exp.h"

class Parser {
    Scanner *scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);

    bool check(Token::Type ttype);

    bool advance();

    bool isAtEnd();

    Exp *parseCExp();

    Exp *parseExpression();

    Exp *parseTerm();

    Exp *parseFactor();

public:
    Parser(Scanner *scanner);

    Program *parseProgram();

    Stm *parseStatement();

    StatementList *parseStatementList();

    VarDec *parseVarDec();

    VarDecList *parseVarDecList();

    Body *parseBody();
};

#endif // PARSER_H
