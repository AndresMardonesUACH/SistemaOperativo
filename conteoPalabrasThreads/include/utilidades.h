#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string.h>
#include <sys/stat.h>
#include <filesystem>
#include <thread>
#include <iostream>

using namespace std;

// Verifica si un directorio existe
bool existeDirectorio(const char* ruta) {
    struct stat info;
    if (stat(ruta, &info) != 0) {
        return false;  // Error al acceder a la ruta
    }
    return (info.st_mode & S_IFDIR) != 0;
}

// Verifica si un archivo existe
bool existeArchivo(const string& ruta) {
    return filesystem::exists(ruta);
}

// Verifica si la extensión del archivo coincide con la extensión esperada
bool esExtensionValida(const char* nombreArchivo, const char* extension) {
    const char* punto = strrchr(nombreArchivo, '.');
    if (!punto || punto == nombreArchivo) return false;
    return strcmp(punto + 1, extension) == 0;
}

// Verifica la configuración inicial, asegurando que los directorios y archivos necesarios existan
bool verificar_configuracion(string& pathEntrada, string& pathSalida, string& mapaArchivos, string& cantidadThreadsStr, string& extension, string& archivoStopWords, int& cantidadThreads) {
    // Verificación de pathEntrada
    if (pathEntrada.empty()) {
        cerr << "Falta la variable de entorno: pathEntrada" << endl;
        return false;
    }
    if (!existeDirectorio(pathEntrada.c_str())) {
        cerr << "El directorio no existe: " << pathEntrada << endl;
        return false;
    }

    // Verificación de pathSalida
    if (pathSalida.empty()) {
        cerr << "Falta la variable de entorno: pathSalida" << endl;
        return false;
    }
    if (!existeDirectorio(pathSalida.c_str())) {
        cerr << "El directorio no existe: " << pathSalida << endl;
        return false;
    }

    // Verificar que los directorios de entrada y salida sean diferentes
    if (pathEntrada == pathSalida) {
        cerr << "ERROR. La carpeta de entrada debe ser diferente a la carpeta de salida." << endl;
        return false;
    }

    // Verificación de mapa_archivos
    if (mapaArchivos.empty()) {
        cerr << "Falta la variable de entorno: mapa_archivos" << endl;
        return false;
    }

    // Verificación de cantidad de threads
    if (cantidadThreadsStr.empty()) {
        cerr << "Falta la variable de entorno: cantidad_thread" << endl;
        return false;
    }

    try {
        cantidadThreads = stoi(cantidadThreadsStr);
    } catch (const invalid_argument& e) {
        cerr << "Error al convertir cantidad_thread a número: " << e.what() << endl;
        return false;
    }

    // Verificar si la cantidad de threads está en el rango permitido
    unsigned int maxThreads = thread::hardware_concurrency();
    if (cantidadThreads <= 0 || cantidadThreads > maxThreads) {
        cerr << "La cantidad de threads debe ser mayor que 0 y menor o igual al máximo de threads disponibles: " << maxThreads << endl;
        return false;
    }

    // Verificación de la extensión de archivo
    if (extension.empty()) {
        cerr << "Falta la variable de entorno: extension" << endl;
        return false;
    }

    // Verificación del archivo de stop words
    if (archivoStopWords.empty()) {
        cerr << "Falta la variable de entorno: stop_word" << endl;
        return false;
    }
    if (!existeArchivo(archivoStopWords)) {
        cerr << "El archivo de stop words no existe: " << archivoStopWords << endl;
        return false;
    }

    return true;
}

#endif
