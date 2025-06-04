#include <iostream>
#include <fstream>
#include "scanner.h"
#include "parser.h"
using namespace std;

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());
    scanner.test();

    Parser parser(&scanner);
    try {
        std::cout << "Iniciando parsing:\n";
        Program *program = parser.parseProgram();
        std::cout << "Parsing exitoso\n\n";
    } catch (const std::exception &e) {
        std::cout << "Error durante el parsing: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
