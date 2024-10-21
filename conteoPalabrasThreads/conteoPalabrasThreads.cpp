#include <iostream>
#include <unistd.h>
#include <string.h>
#include "mapa_archivos.h"
#include <ctime>
#include "limpieza_archivo.h"
using namespace std;

// Función para comunicar el término de la ejecución escribiendo un archivo indicador
void comunicarTermino(bool termino, const char* salida) {
    string nuevaRuta = string(salida) + "/cPTEjecutado.txt";
    ofstream archivoTermino(nuevaRuta.c_str());
    archivoTermino << (termino ? "true" : "false");
    archivoTermino.close();
}

int main(int argc, char* argv[]) {
    int opt, cantidad_thread;
    string extension = "";
    string pathEntrada = "";
    string pathSalida = "";
    string archivo_stop_words = "";
    string mapa_archivos = "";
    string cantidad_thread_str = "";

    // Procesar los argumentos de línea de comandos usando getopt
    while ((opt = getopt(argc, argv, "e:i:o:s:m:t:")) != -1) {
        switch (opt) {
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
                cerr << "Opción no válida: " << opt << endl;
                return 1;
        }
    }

    bool termino = false;  // Variable para indicar el éxito de la operación
    
    
    // Verificar que la configuración sea correcta
    if (verificar_configuracion(pathEntrada, pathSalida, mapa_archivos, cantidad_thread_str, extension, archivo_stop_words, cantidad_thread)) {
        // Escribir el archivo de mapeo
        if (escribirMapaArchivos(pathEntrada, mapa_archivos, extension)) {
            // Verificar si el archivo de mapeo se generó correctamente
            if (existeArchivo(mapa_archivos.c_str())) {
                // Procesar los libros con los parámetros proporcionados
                procesarLibros(pathEntrada, pathSalida, mapa_archivos, extension, archivo_stop_words, cantidad_thread);
                termino = true;
            } else {
                cerr << "El archivo de mapeo no existe: " << mapa_archivos << endl;
            }
        }
    }

    // Comunicar si la ejecución fue exitosa o no
    comunicarTermino(termino, pathSalida.c_str());

    return termino ? 0 : 1;  // Retornar 0 si fue exitoso, 1 en caso de error
}
