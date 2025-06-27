#include "preprocessor.h"
#include <iostream>
#include <filesystem>

bool Preprocessor::match(const Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Preprocessor::check(const Token::Type ttype) const {
    return !isAtEnd() && current->type == ttype;
}

bool Preprocessor::advance() {
    if (!isAtEnd()) {
        Token *temp = current;
        delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            std::cerr << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Preprocessor::isAtEnd() const {
    return current->type == Token::END;
}

#define filename "../processed.txt"

Preprocessor::Preprocessor(Scanner *scanner) : scanner(scanner), current(scanner->nextToken()), previous(nullptr) {
    if (current->type == Token::ERR) {
        std::cerr << "Error en el primer token: " << current->text << '\n';
        exit(1);
    }
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
    out = std::ofstream(filename);
}

Program *Preprocessor::processProgram() {
    auto *program = new Program();
    program->vardecs = parseVarDecList();
    program->fundecs = parseFunDecList();
    return program;
}

VarDecList *Preprocessor::parseVarDecList() {
    auto *list = new VarDecList();
    auto *temp = parseVarDec();
    while (temp) {
        list->add(temp);
        temp = parseVarDec();
    }
    return list;
}

VarDec *Preprocessor::parseVarDec() {
    VarDec *vd = nullptr;
    if (match(Token::VAR)) {
        out << "var ";
        if (!match(Token::ID)) {
            cerr << "Error: se esperaba un identificador después de 'var'.\n";
            exit(1);
        }
        string type = previous->text;
        out << previous->text << ' ';
        list<string> ids;
        if (!match(Token::ID)) {
            cerr << "Error: se esperaba un identificador después de 'var'.\n";
            exit(1);
        }
        ids.push_back(previous->text);
        out << previous->text;
        while (match(Token::COMA)) {
            out << ", ";
            if (!match(Token::ID)) {
                cerr << "Error: se esperaba un identificador después de ','.\n";
                exit(1);
            }
            ids.push_back(previous->text);
            out << previous->text;
        }
        if (!match(Token::PC)) {
            cerr << "Error: se esperaba un ';' al final de la declaración.\n";
            exit(1);
        }
        out << ";\n";
        vd = new VarDec(type, ids);
    }
    return vd;
}

FunDecList *Preprocessor::parseFunDecList() {
    auto *list = new FunDecList();
    auto *temp = parseFunDec();
    while (temp) {
        out << '\n';
        list->add(temp);
        temp = parseFunDec();
    }
    return list;
}

FunDec *Preprocessor::parseFunDec() {
    FunDec *vd = nullptr;
    if (match(Token::FUN)) {
        out << "fun ";
        auto *fu = new FunDec();
        match(Token::ID);
        fu->tipo = previous->text;
        out << previous->text << ' ';
        match(Token::ID);
        fu->nombre = previous->text;
        out << previous->text;
        match(Token::PI);
        out << '(';
        while (match(Token::ID)) {
            fu->tipos.push_back(previous->text);
            out << previous->text << ' ';
            match(Token::ID);
            fu->parametros.push_back(previous->text);
            out << previous->text;
            if (!match(Token::COMA)) break;
            out << ", ";
        }
        match(Token::PD);
        out << ")\n";
        fu->cuerpo = parseBody();
        match(Token::ENDFUN);
        out << "endfun";
        vd = fu;
    }
    return vd;
}

Body *Preprocessor::parseBody() {
    auto *vdl = parseVarDecList();
    auto *sl = parseStatementList();
    return new Body(vdl, sl);
}

StatementList *Preprocessor::parseStatementList() {
    auto *sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        out << ";\n";
        sl->add(parseStatement());
    }
    out << '\n';
    return sl;
}

Stm *Preprocessor::parseStatement() {
    Stm *s = nullptr;
    Exp *e = nullptr;
    Body *tb = nullptr;
    Body *fb = nullptr;

    if (current == nullptr) {
        cerr << "Error: Token actual es NULL" << endl;
        exit(1);
    }
    if (match(Token::ID)) {
        string lex = previous->text;
        out << previous->text << ' ';
        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        out << "= ";
        buffer.emplace("");
        e = parseCExp();
        out << buffer.top();
        buffer.pop();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        out << "print";
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        out << '(';
        buffer.emplace("");
        e = parseCExp();
        out << buffer.top();
        buffer.pop();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        out << ')';
        s = new PrintStatement(e);
    } else if (match(Token::RETURN)) {
        out << "return";
        ReturnStatement *rs = new ReturnStatement();
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de 'return'." << endl;
            exit(1);
        }
        out << '(';
        if (check(Token::PD)) {
            advance();
            out << ')';
            rs->e = nullptr;
        } else {
            buffer.emplace("");
            rs->e = parseCExp();
            out << buffer.top();
            buffer.pop();
            if (!match(Token::PD)) {
                cout << "Error: se esperaba ')' después de la expresión de return." << endl;
                exit(1);
            }
            out << ')';
        }
        return rs;
    } else if (match(Token::IF)) {
        out << "if ";
        buffer.emplace("");
        e = parseCExp();
        out << buffer.top();
        buffer.pop();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        out << " then\n";
        tb = parseBody();
        if (match(Token::ELSE)) {
            out << "else\n";
            fb = parseBody();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'endif' al final de la declaración de if." << endl;
            exit(1);
        }
        out << "endif\n";
        s = new IfStatement(e, tb, fb);
    } else if (match(Token::WHILE)) {
        out << "while ";
        buffer.emplace("");
        e = parseCExp();
        out << buffer.top();
        buffer.pop();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        out << " do\n";
        tb = parseBody();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        out << "endwhile\n";
        s = new WhileStatement(e, tb);
    } else {
        cout << "Error: Se esperaba un identificador, 'print', o estructura válida, pero se encontró: " << *current <<
                endl;
        exit(1);
    }
    return s;
}

Exp *Preprocessor::parseCExp() {
    buffer.emplace("");
    Exp *left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ)) {
        BinaryOp op;
        if (previous->type == Token::LT) {
            op = LT_OP;
        } else if (previous->type == Token::LE) {
            op = LE_OP;
        } else {
            op = EQ_OP;
        }
        buffer.top() += previous->text;
        Exp *right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    const std::string local = buffer.top();
    buffer.pop();
    buffer.top() += local;
    return left;
}

Exp *Preprocessor::parseExpression() {
    buffer.emplace("");
    Exp *left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        const BinaryOp op = (previous->type == Token::PLUS) ? PLUS_OP : MINUS_OP;
        buffer.top() += previous->text;
        Exp *right = parseTerm();

        const auto lnum = dynamic_cast<NumberExp *>(left);
        const auto rnum = dynamic_cast<NumberExp *>(right);
        if (lnum && rnum) {
            const int result = (op == PLUS_OP) ? lnum->value + rnum->value : lnum->value - rnum->value;
            delete left;
            delete right;
            left = new NumberExp(result);
            buffer.top() = std::to_string(result);
        } else {
            left = new BinaryExp(left, right, op);
        }
    }
    const std::string local = buffer.top();
    buffer.pop();
    buffer.top() += local;
    return left;
}

Exp *Preprocessor::parseTerm() {
    buffer.emplace("");
    Exp *left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        const BinaryOp op = (previous->type == Token::MUL) ? MUL_OP : DIV_OP;
        buffer.top() += previous->text;
        Exp *right = parseFactor();

        const auto lnum = dynamic_cast<NumberExp *>(left);
        const auto rnum = dynamic_cast<NumberExp *>(right);
        if (lnum && rnum) {
            const int result = (op == MUL_OP) ? lnum->value * rnum->value : lnum->value / rnum->value;
            delete left;
            delete right;
            left = new NumberExp(result);
            buffer.top() = std::to_string(result);
        } else {
            left = new BinaryExp(left, right, op);
        }
    }
    const std::string local = buffer.top();
    buffer.pop();
    buffer.top() += local;
    return left;
}

Exp *Preprocessor::parseFactor() {
    if (match(Token::TRUE)) {
        buffer.top() += "true";
        return new BoolExp(true);
    }
    if (match(Token::FALSE)) {
        buffer.top() += "false";
        return new BoolExp(false);
    }
    if (match(Token::NUM)) {
        buffer.top() += previous->text;
        return new NumberExp(stoi(previous->text));
    }
    if (match(Token::ID)) {
        string nombre = previous->text;
        buffer.top() += previous->text;
        if (match(Token::PI)) {
            buffer.top() += '(';
            auto *fc = new FCallExp();
            vector<Exp *> lista;
            lista.push_back(parseCExp());
            while (match(Token::COMA)) {
                buffer.top() += ',';
                lista.push_back(parseCExp());
            }
            if (!match(Token::PD)) {
                cerr << "Error: se esperaba un ')' después de la lista de argumentos.\n";
                exit(1);
            }
            buffer.top() += ')';
            fc->argumentos = lista;
            fc->nombre = nombre;
            return fc;
        }
        return new IdentifierExp(nombre);
    }
    if (match(Token::PI)) {
        buffer.top() += '(';
        Exp *e = parseCExp();
        if (!match(Token::PD)) {
            cerr << "Falta parentesis derecho\n";
            exit(1);
        }
        buffer.top() += ')';
        return e;
    }
    cerr << "Error: se esperaba un número o identificador.\n";
    exit(1);
}
