#include <iostream>
#include <unistd.h>
using namespace std;

string core(string operacion, float x, float y) {
    if (operacion == "suma") {
        return to_string(x+y);
    } else if (operacion == "resta") {
        return to_string(x-y);
    } else if (operacion == "division") {
        if (y == 0) return "Error, no se puede dividir por 0.";
        return to_string(x/y);
    } else if (operacion == "multiplicacion") {
        return to_string(x*y);
    }
    return "La operación no está definida.";
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

    core(operacion, stof(valor1), stof(valor2));

    return EXIT_SUCCESS;
}