#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
using namespace std;

#define MAX_LONGITUD_PALABRA 100
#define MAX_LONGITUD_RUTA 512

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

int esExtensionValida(const char* nombreArchivo, const char* extension) {
    const char *punto = strrchr(nombreArchivo, '.');
    if (!punto || punto == nombreArchivo) return 0;
    return strcmp(punto + 1, extension) == 0;
}

void procesarArchivo(const char* archivoEntrada, const char* archivoSalida) {
    ifstream entrada(archivoEntrada);
    if (!entrada) {
        cerr << "No se pudo abrir el archivo de entrada: " << archivoEntrada << endl;
        return;
    }

    unordered_map<string, int> mapaPalabras;

    string palabra;
    while (entrada >> palabra) {
        transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
        mapaPalabras[palabra]++;
    }
    entrada.close();

    ofstream salida(archivoSalida);
    if (!salida) {
        cerr << "No se pudo crear el archivo de salida: " << archivoSalida << endl;
        return;
    }

    for (const auto& entrada : mapaPalabras) {
        salida << entrada.first << "; " << entrada.second << endl;
    }
    salida.close();

    cout << "Archivo " << archivoSalida << ", " << mapaPalabras.size() << " palabras distintas" << endl;
}

bool procesarDatos(char* extension, char* rutaProcesar, char* rutaRespuesta) {
    struct dirent *entrada;
    DIR *directorio = opendir(rutaProcesar);

    if (directorio == NULL) {
        printf("No se pudo abrir el directorio: %s\n", rutaProcesar);
        return true;
    }

    while ((entrada = readdir(directorio))) {
        if (entrada->d_type == DT_REG && esExtensionValida(entrada->d_name, extension)) {
            char archivoEntrada[MAX_LONGITUD_RUTA];
            char archivoSalida[MAX_LONGITUD_RUTA];

            snprintf(archivoEntrada, sizeof(archivoEntrada), "%s/%s", rutaProcesar, entrada->d_name);
            snprintf(archivoSalida, sizeof(archivoSalida), "%s/%s", rutaRespuesta, entrada->d_name);

            procesarArchivo(archivoEntrada, archivoSalida);
        }
    }

    closedir(directorio);
    return false;
}

bool validarDatos(char* extension, char* rutaProcesar, char* rutaRespuesta) {
    if (!extension || !rutaProcesar || !rutaRespuesta) {
        cout << "\033[31mError: Hacen falta datos por ingresar.\033[0m" << endl;
        return false;
    }

    if (!existeDirectorio(rutaProcesar)) {
        cout << "\033[31mError: La carpeta a procesar no existe.\033[0m" << endl;
        return false;
    }

    if (!existeDirectorio(rutaRespuesta)) {
        cout << "\033[31mError: La carpeta de respuesta no existe.\033[0m" << endl;
        return false;
    }

    cout << "\033[32mÉxito: Ambos directorios existen y la extensión fue proporcionada.\033[0m" << endl;
    return true;
}
