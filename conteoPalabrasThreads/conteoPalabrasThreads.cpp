#include <iostream>
#include <unistd.h>
#include <string.h>
#include "mapa_archivos.h"
using namespace std;

int main(int argc, char* argv[]){
    int opt;
    char* extension = nullptr;
    char* entrada = nullptr;
    char* salida = nullptr;
    char* stop_word = nullptr;
    char* mapa_archivos = nullptr;

    while ((opt = getopt(argc, argv, "e:i:o:s:m:")) != -1){
        switch (opt){
        case 'e':
            extension = optarg;
            break;
        case 'i':
            entrada = optarg;
            break;
        case 'o':
            salida = optarg;
            break;
        case 's':
            stop_word = optarg;
            break;
        case 'm':
            mapa_archivos = optarg;
            break;
        default:
            break;
        }
    }

    // Se valida que todos los argumentos hayan sido ingresados
    if(!extension){
        cout << "ERROR. Debe ingresar extensión -e" << endl;
        exit(EXIT_FAILURE);
    }
    if(!entrada){
        cout << "ERROR. Debe ingresar path de entrada -i" << endl;
        exit(EXIT_FAILURE);
    }
    if(!salida){
        cout << "ERROR. Debe ingresar path de salida -o" << endl;
        exit(EXIT_FAILURE);
    }
    if(!stop_word){
        cout << "ERROR. Debe ingresar path de stop_words -s" << endl;
        exit(EXIT_FAILURE);
    }
    if(!mapa_archivos){
        cout << "ERROR. Debe ingresar path de mapa_archivos -m" << endl;
        exit(EXIT_FAILURE);
    }

    if(strcmp(entrada, salida) == 0){
        cout << "ERROR. Carpeta de entrada debe ser diferente a la carpeta de salida" << endl;
    }

    escribirMapaArchivos(entrada, mapa_archivos, extension);

    return 0;
}