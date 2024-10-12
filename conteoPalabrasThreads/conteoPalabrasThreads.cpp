#include <iostream>
#include <unistd.h>
#include <string.h>
#include "mapa_archivos.h"
#include "limpieza_archivo.h"
using namespace std;

int main(int argc, char* argv[]){
    int opt, cantidad_thread;
    string extension = "";
    string pathEntrada = "";
    string pathSalida = "";
    string archivo_stop_words = "";
    string mapa_archivos = "";
    string cantidad_thread_str= "";

    while ((opt = getopt(argc, argv, "e:i:o:s:m:t:")) != -1){
        switch (opt){
        case 'e':
            extension = optarg;
            break;
        case 'i':
            pathEntrada = optarg;
            break;
        case 'o':
            pathSalida = optarg;
            break;
        case 's':
            archivo_stop_words = optarg;
            break;
        case 'm':
            mapa_archivos = optarg;
            break;
        case 't':
            cantidad_thread_str = optarg;
            break;
        default:
            break;
        }
    }
    
    if (verificar_configuracion(pathEntrada, pathSalida, mapa_archivos, cantidad_thread_str, extension, archivo_stop_words, cantidad_thread)) {
        escribirMapaArchivos(pathEntrada, mapa_archivos, extension);
        
        if (existe_archivo(mapa_archivos.c_str())) {
            procesar_libros(pathEntrada, pathSalida, mapa_archivos, extension, archivo_stop_words, cantidad_thread);
        } else {
            cerr << "El archivo de mapeo no existe: " << mapa_archivos << endl;
        }
    }

    return 0;
}
