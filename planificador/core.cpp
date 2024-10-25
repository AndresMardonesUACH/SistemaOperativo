#include <iostream>
#include <unistd.h>
using namespace std;

void verificarParametros(const string& operacion, const string& valor1, const string& valor2) {
    if (operacion.empty()) {
        cerr << "Error: La operación no ha sido ingresada." << endl;
        exit(EXIT_FAILURE);
    }

    if (valor1.empty()) {
        cerr << "Error: El valor1 no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }

    if (valor2.empty()) {
        cerr << "Error: El valor2 no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }

    try {
        stof(valor1);
    } catch (const invalid_argument& e) {
        cerr << "Error: valor1 no es un número flotante válido." << endl;
        exit(EXIT_FAILURE);
    }

    try {
        stof(valor2);
    } catch (const invalid_argument& e) {
        cerr << "Error: valor2 no es un número flotante válido." << endl;
        exit(EXIT_FAILURE);
    }
}

void core(string operacion, float x, float y) {
    if (operacion == "suma") {
        cout << (x+y);
    } else if (operacion == "resta") {
        cout << (x-y);
    } else if (operacion == "division") {
        if (y == 0) cout << "Error, no se puede dividir por 0.";
        else cout << (x/y);
    } else if (operacion == "multiplicacion") {
        cout << (x*y);
    } else {
        cout << "La operación no está definida.";
    }
}

int main(int argc, char* argv[]){
    int opt;
    char* operacion = nullptr;
    char* valor1 = nullptr;
    char* valor2 = nullptr;

    while ((opt = getopt(argc, argv, "o:p:s:")) != -1){
        switch (opt){
        case 'o':
            operacion = optarg;
            break;
        case 'p':
            valor1 = optarg;
            break;
        case 's':
            valor2 = optarg;
            break;
        default:
            break;
        }
    }
    
    verificarParametros(operacion, valor1, valor2);

    core(operacion, stof(valor1), stof(valor2));

    return EXIT_SUCCESS;
}