#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "scanner.h"
#include "exp.h"
#include <fstream>

class Preprocessor {
    Scanner *scanner;
    Token *current, *previous;
    ofstream out;

    bool match(Token::Type ttype);

    bool check(Token::Type ttype) const;

    bool advance();

    bool isAtEnd() const;

    list<Stm *> parseStmList();

    Exp *parseExp();

    Exp *parseCExp();

    Exp *parseExpression();

    Exp *parseTerm();

    Exp *parseFactor();

    Stm *parseStatement();

    StatementList *parseStatementList();

    VarDec *parseVarDec();

    VarDecList *parseVarDecList();

    Body *parseBody();

    FunDecList *parseFunDecList();

    FunDec *parseFunDec();

public:
    explicit Preprocessor(Scanner *scanner);

    Program *processProgram();
};


#endif //PREPROCESSOR_H
