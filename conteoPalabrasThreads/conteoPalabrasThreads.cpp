#include <iostream>
#include <unistd.h>
#include <string.h>
#include "mapa_archivos.h"
#include "limpieza_archivo.h"
using namespace std;

void comunicarTermino(bool termino, const char* salida){
    string nuevaRuta = string(salida) + "/cPTEjecutado.txt";
    ofstream archivoTermino(nuevaRuta.c_str());
    if(termino) archivoTermino << "true";
    else archivoTermino << "false";
    archivoTermino.close();
}

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
    
    bool termino = false;

    if (verificar_configuracion(pathEntrada, pathSalida, mapa_archivos, cantidad_thread_str, extension, archivo_stop_words, cantidad_thread)) {
        if(escribirMapaArchivos(pathEntrada, mapa_archivos, extension)){
            if (existe_archivo(mapa_archivos.c_str())) {
                procesar_libros(pathEntrada, pathSalida, mapa_archivos, extension, archivo_stop_words, cantidad_thread);
                termino = true;
            } else {
                cerr << "El archivo de mapeo no existe: " << mapa_archivos << endl;
            }
        }
    }
    comunicarTermino(termino, pathSalida.c_str());

    return 0;
}
