#include "parser.h"


bool Parser::isAtFinish() const {
    return current->type == Token::FINISH;
}

bool Parser::check(const Token::Type type) const {
    return isAtFinish() ? false : current->type == type;
}

bool Parser::match(const Token::Type type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::advance() {
    if (!isAtFinish()) {
        Token *temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR)) {
            std::cout << "Analysis error, unrecognized character: " << current->text << '\n';
            exit(1);
        }
        return true;
    }
    return false;
}

Parser::Parser(Scanner *scanner) : scanner(scanner),
                                   current(scanner->nextToken()),
                                   previous(nullptr) {
    if (current->type == Token::ERROR) {
        std::cout << "Error on the first token: " << current->type << '\n';
        exit(0);
    }
}

Program *Parser::parseProgram() {
    return nullptr;
}

VarBlock *Parser::parseVarBlock() {
    return nullptr;
}

ModelBlock *Parser::parseModelBlock() {
    return nullptr;
}

Equation *Parser::parseEquation() {
    return nullptr;
}

Exp *Parser::parseExp() {
    return nullptr;
}

Exp *Parser::parseTerm() {
    return nullptr;
}

Exp *Parser::parseFactor() {
    return nullptr;
}
