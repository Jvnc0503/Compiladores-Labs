#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include "token.h"

class Scanner {
    std::string input;
    int first, current;

public:
    explicit Scanner(const char *in_s);

    static bool is_white_space(char c);

    Token *nextToken();

    void reset();

    ~Scanner();

    void test();
};


#endif //SCANNER_H
