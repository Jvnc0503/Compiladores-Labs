#include "scanner.h"
#include "scanner.h"
#include <cstring>
#include <string>
#include "token.h"

Scanner::Scanner(const char *in_s): input(in_s), first(0), current(0) {
}

bool Scanner::is_white_space(const char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token *Scanner::nextToken() {
    while (current < input.length() && is_white_space(input[current])) current++;
    if (current >= input.length()) return new Token(Token::FINISH);
    const char c = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current])) {
            current++;
        }
        return new Token(Token::NUM, input, first, current - first);
    }
    if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current])) {
            current++;
        }
        const std::string word = input.substr(first, current - first);
        if (word == "var") {
            return new Token(Token::VAR, word, 0, word.length());
        }
        if (word == "model") {
            return new Token(Token::MODEL, word, 0, word.length());
        }
        if (word == "end") {
            return new Token(Token::END, word, 0, word.length());
        }
        if (word == "solve") {
            return new Token(Token::SOLVE, word, 0, word.length());
        }
        return new Token(Token::ID, word, 0, word.length());
    }
    if (strchr(",;=+-*", c)) {
        current++;
        switch (c) {
            case ',': return new Token(Token::COMMA, c);
            case ';': return new Token(Token::SEMICOLON, c);
            case '=': return new Token(Token::EQUAL, c);
            case '+': return new Token(Token::PLUS, c);
            case '-': return new Token(Token::MINUS, c);
            case '*': return new Token(Token::MUL, c);
            default:
                std::cout << "Should not reach here with operator: " << c << '\n';
                exit(0);
        }
    }
    current++;
    return new Token(Token::ERROR, c);
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() = default;

void Scanner::test() {
    Token *current;
    std::cout << "Starting Scanner:\n\n";
    while ((current = nextToken())->type != Token::FINISH) {
        if (current->type == Token::FINISH) {
            std::cout << "Scanner error - Invalid character: " << current->text << '\n';
            delete current;
            exit(0);
        }
        std::cout << *current << '\n';
        delete current;
    }
    delete current;
    std::cout << "TOKEN(FINISH)\n\nScanner finished successfully.\n";
    reset();
}
