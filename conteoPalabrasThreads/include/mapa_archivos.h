#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream> 
#include <sys/stat.h>
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

int esExtensionValida(const char* nombreArchivo, const char* extension) {
    const char *punto = strrchr(nombreArchivo, '.');
    if (!punto || punto == nombreArchivo) return 0;
    return strcmp(punto + 1, extension) == 0;
}

bool procesarDatos(string extension, string pathEntrada, string pathMapaArchivos) {
    struct dirent *entrada;
    DIR *directorio = opendir(pathEntrada.c_str());

    if (directorio == NULL) {
        cout << "No se pudo abrir el directorio: " << pathEntrada << endl;
        return false;
    }

    bool archivoEncontrado = false;
    int i = 0;
    ofstream archivo(pathMapaArchivos.c_str());
    while ((entrada = readdir(directorio))) {
        if (entrada->d_type == DT_REG && esExtensionValida(entrada->d_name, extension.c_str())) {
            archivoEncontrado = true;
            archivo << entrada->d_name << ", " << i << endl;
            i++;
        }
    }
    if(!archivoEncontrado){
        cout << "\033[31mNo existen archivos con la extensión: " << extension << "\033[0m" << endl;
        return false;
    }
    archivo.close();
    closedir(directorio);
    return true;
}

bool escribirMapaArchivos(string pathEntrada, string pathMapaArchivos, string extension){
    
    if(procesarDatos(extension, pathEntrada, pathMapaArchivos)){
        cout << "\033[32mArchivo mapa_archivos.txt creado correctamente\033[0m" << endl;   
        return true;
    }
    return false;

}