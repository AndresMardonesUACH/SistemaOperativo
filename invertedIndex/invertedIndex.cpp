#include <iostream>
#include <unistd.h>
#include <string.h>
#include "crearInvertedIndex.h"
using namespace std;


bool cPTEjecutado(const char* salida){
    string ruta = string(salida) + "/cPTEjecutado.txt";  
    ifstream archivoTermino(ruta.c_str());
    string linea;
    archivoTermino >> linea;
    if(linea == "true") return true;
    return false;
}

int main(int argc, char* argv[]){
    int opt;
    char* extension = nullptr;
    char* entrada = nullptr;
    char* salida = nullptr;
    char* mapa_archivos = nullptr;
    char* inverted_index = nullptr;

    while ((opt = getopt(argc, argv, "e:i:o:m:x:")) != -1){
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
        case 'm':
            mapa_archivos = optarg;
            break;
        case 'x':
            inverted_index = optarg;
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
    if(!mapa_archivos){
        cout << "ERROR. Debe ingresar path de mapa_archivos -m" << endl;
        exit(EXIT_FAILURE);
    }
    if(!inverted_index){
        cout << "ERROR. Debe ingresar path de inverted_index -x" << endl;
        exit(EXIT_FAILURE);
    }
    if(!cPTEjecutado(salida)){
        cout << "\033[31mERROR. Debe ejecutarse 'Conteo Paralelo con Threads' antes\033[0m" << endl;
        exit(EXIT_FAILURE);
    }

    crearInvertedIndex(salida, mapa_archivos, inverted_index, extension);


    return 0;
}