#ifndef UTILIDADES_H
#define UTILIDADES_H
#include <string.h>
#include <sys/stat.h>
#include <filesystem>
#include <thread>
using namespace std;

bool existeDirectorio(const char* ruta) {
    struct stat info;
    if (stat(ruta, &info) != 0) {
        return false;
    } else if (info.st_mode & S_IFDIR) {
        return true;
    } else {
        return false;
    }
}

bool existe_archivo(const string& ruta) {
    return filesystem::exists(ruta);
}

int esExtensionValida(const char* nombreArchivo, const char* extension) {
    const char *punto = strrchr(nombreArchivo, '.');
    if (!punto || punto == nombreArchivo) return 0;
    return strcmp(punto + 1, extension) == 0;
}

bool verificar_configuracion(string& pathEntrada, string& pathSalida, string& mapa_archivos, string& cantidad_thread_str, string& extension, string& archivo_stop_words, int& cantidad_thread) {
    // Verificar si los valores existen
    if (pathEntrada.empty()) {
        cerr << "Falta la variable de entorno: pathEntrada" << endl;
        return false;
    }
    if (!existeDirectorio(pathEntrada.c_str())) {
        cerr << "El directorio no existe: " << pathEntrada << endl;
        return false;
    }

    if (pathSalida.empty()) {
        cerr << "Falta la variable de entorno: pathSalida" << endl;
        return false;
    }
    if (!existeDirectorio(pathSalida.c_str())) {
        cerr << "El directorio no existe: " << pathSalida << endl;
        return false;
    }

    if(pathEntrada == pathSalida){
        cout << "ERROR. Carpeta de entrada debe ser diferente a la carpeta de salida" << endl;
        return false;
    }

    if (mapa_archivos.empty()) {
        cerr << "Falta la variable de entorno: mapa_archivos" << endl;
        return false;
    }

    if (cantidad_thread_str.empty()) {
        cerr << "Falta la variable de entorno: cantidad_thread" << endl;
        return false;
    }
    
    // Convertir cantidad_thread_str a int
    try {
        cantidad_thread = stoi(cantidad_thread_str);
    } catch (const invalid_argument& e) {
        cerr << "Error al convertir cantidad_thread a número: " << e.what() << endl;
        return false;
    }

    // Verificar que cantidad_thread sea > 0 y <= el máximo de threads de la máquina
    if (cantidad_thread <= 0 || cantidad_thread > thread::hardware_concurrency()) {
        cerr << "La cantidad de threads debe ser mayor que 0 y menor o igual al máximo de threads disponibles: " << thread::hardware_concurrency() << endl;
        return false;
    }

    if (extension.empty()) {
        cerr << "Falta la variable de entorno: extension" << endl;
        return false;
    }
    
    if (archivo_stop_words.empty()) {
        cerr << "Falta la variable de entorno: stop_word" << endl;
        return false;
    }
    // Verificar que el archivo de stop words exista
    if (!existe_archivo(archivo_stop_words.c_str())) {
        cerr << "El archivo de stop words no existe: " << archivo_stop_words << endl;
        return false;
    }

    return true;
}

#endif