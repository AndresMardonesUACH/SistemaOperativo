#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <filesystem>
#include "include/crearInvertedIndex.h"
using namespace std;

// Función para verificar si el procesamiento ha terminado
bool cPTEjecutado(const char* salida) {
    string ruta = string(salida) + "/cPTEjecutado.txt";
    ifstream archivoTermino(ruta);
    if (!archivoTermino) {
        cerr << "ERROR. No se pudo abrir el archivo de estado: " << ruta << endl;
        return false;
    }

    string linea;
    archivoTermino >> linea;
    return (linea == "true");
}

// Función para comunicar el término de la ejecución escribiendo un archivo indicador
void comunicarTermino(bool termino, string ruta) {
    string nuevaRuta = ruta + "/invertedIndexEjecutado.txt";
    ofstream archivoTermino(nuevaRuta.c_str());
    archivoTermino << (termino ? "true" : "false");
    archivoTermino.close();
}

int main(int argc, char* argv[]) {
    int opt;
    char* extension = nullptr;
    char* entrada = nullptr;
    char* salida = nullptr;
    char* mapa_archivos = nullptr;
    char* inverted_index = nullptr;

    // Procesar argumentos de línea de comandos
    while ((opt = getopt(argc, argv, "e:i:o:m:x:")) != -1) {
        switch (opt) {
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

    // Validar que todos los argumentos necesarios hayan sido ingresados
    if (!extension) {
        cerr << "ERROR. Debe ingresar extensión -e" << endl;
        exit(EXIT_FAILURE);
    }
    if (!entrada) {
        cerr << "ERROR. Debe ingresar path de entrada -i" << endl;
        exit(EXIT_FAILURE);
    }
    if (!salida) {
        cerr << "ERROR. Debe ingresar path de salida -o" << endl;
        exit(EXIT_FAILURE);
    }
    if (!mapa_archivos) {
        cerr << "ERROR. Debe ingresar path de mapa_archivos -m" << endl;
        exit(EXIT_FAILURE);
    }
    if (!inverted_index) {
        cerr << "ERROR. Debe ingresar path de inverted_index -x" << endl;
        exit(EXIT_FAILURE);
    }
    if (!cPTEjecutado(salida)) {
        cerr << "\033[31mERROR. Debe ejecutarse 'Conteo Paralelo con Threads' antes\033[0m" << endl;
        exit(EXIT_FAILURE);
    }

    // Crear el índice invertido
    if (!crearInvertedIndex(salida, mapa_archivos, inverted_index, extension)) {
        cerr << "ERROR. Fallo al crear el índice invertido." << endl;
        exit(EXIT_FAILURE);
    }

    filesystem::path rutaCompleta(inverted_index);
        string rutaDatos = rutaCompleta.parent_path();
        comunicarTermino(true, rutaDatos);

    return 0;
}
