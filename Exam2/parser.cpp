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
        exit(1);
    }
}

Program *Parser::parseProgram() {
    VarBlock *varBlock = parseVarBlock();
    ModelBlock *modelBlock = parseModelBlock();
    return new Program(varBlock, modelBlock);
}

VarBlock *Parser::parseVarBlock() {
    if (!match(Token::VAR)) {
        std::cout << "Error: expected 'var' at the beginning of the variable block.\n";
        exit(1);
    }
    std::list<std::string> vars;
    while (match(Token::ID) && !match(Token::SEMICOLON)) {
        vars.push_back(previous->text);
        if (!match(Token::COMMA) && !match(Token::SEMICOLON)) {
            std::cout << "Error: expected ',' or ';' after variable identifier.\n";
            exit(1);
        }
    }
    return new VarBlock(vars);
}

ModelBlock *Parser::parseModelBlock() {
    if (!match(Token::MODEL)) {
        std::cout << "Error: expected 'model' at the beginning of the model block.\n";
        exit(1);
    }
    if (!match(Token::SEMICOLON)) {
        std::cout << "Error: expected ';' after 'model'.\n";
        exit(1);
    }
    std::list<Equation *> equations;
    while (!match(Token::END)) {
        Equation *e = parseEquation();
        if (!match(Token::SEMICOLON)) {
            std::cout << "Error: expected ';' after equation.\n";
            exit(1);
        }
        equations.push_back(e);
    }
    if (!match(Token::SEMICOLON)) {
        std::cout << "Error: expected ';' after end.\n";
        exit(1);
    }
    return new ModelBlock(equations);
}

Equation *Parser::parseEquation() {
    Exp *left = parseExp();
    if (!match(Token::EQUAL)) {
        std::cout << "Error: expected '=' after left-hand side expression.\n";
        exit(1);
    }
    Exp *right = parseExp();
    return new Equation(left, right);
}

Exp *Parser::parseExp() {
    Exp *left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        Exp::BinaryOp op;
        switch (previous->type) {
            case Token::PLUS: op = Exp::PLUS_OP;
                break;
            case Token::MINUS: op = Exp::MINUS_OP;
                break;
            default: std::cout << "Error: unrecognized operator: " << previous->text << '\n';
                exit(1);
        }
        Exp *right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp *Parser::parseTerm() {
    Exp *left = parseFactor();
    while (match(Token::MUL)) {
        Exp *right = parseFactor();
        left = new BinaryExp(left, right, BinaryExp::MUL_OP);
    }
    return left;
}

Exp *Parser::parseFactor() {
    if (match(Token::NUM)) {
        return new NumberExp(std::stoi(previous->text));
    }
    if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    if (match(Token::LP)) {
        Exp *e = parseExp();
        if (!match(Token::RP)) {
            std::cout << "Error: expected ')' after '(' and expression.\n";
            exit(1);
        }
        return e;
    }
    std::cout << "Error: expected a number, identifier, or '('. Found: " << current->text << '\n';
    exit(1);
}
