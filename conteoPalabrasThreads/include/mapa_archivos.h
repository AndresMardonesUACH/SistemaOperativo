#include <iostream>
#include <fstream>
#include <dirent.h>
#include <utilidades.h>  // Asegúrate de tener este archivo disponible
using namespace std;

// Procesar archivos en el directorio que coincidan con la extensión
bool procesarDatos(const string& extension, const string& pathEntrada, const string& pathMapaArchivos) {
    DIR* directorio = opendir(pathEntrada.c_str());
    if (directorio == nullptr) {
        cout << "No se pudo abrir el directorio: " << pathEntrada << endl;
        return false;
    }

    bool archivoEncontrado = false;
    ofstream archivo(pathMapaArchivos);
    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo: " << pathMapaArchivos << endl;
        closedir(directorio);
        return false;
    }

    int contador = 0;
    struct dirent* entrada;
    while ((entrada = readdir(directorio))) {
        if (entrada->d_type == DT_REG && esExtensionValida(entrada->d_name, extension.c_str())) {
            archivoEncontrado = true;
            archivo << entrada->d_name << "," << contador << endl;
            ++contador;
        }
    }

    if (!archivoEncontrado) {
        cout << "\033[31mNo existen archivos con la extensión: " << extension << "\033[0m" << endl;
        archivo.close();
        closedir(directorio);
        return false;
    }

    archivo.close();
    closedir(directorio);
    return true;
}

// Escribir el archivo mapa_archivos si la carpeta existe y se encuentran archivos válidos
bool escribirMapaArchivos(const string& pathEntrada, const string& pathMapaArchivos, const string& extension) {
    if (!existeArchivo(pathMapaArchivos.c_str())) {
        cout << "ERROR. La carpeta para mapa_archivos no existe" << endl;
        return false;
    }

    string rutaMapaArchivos = pathMapaArchivos;
    if (procesarDatos(extension, pathEntrada, rutaMapaArchivos)) {
        cout << "\033[32mArchivo mapa_archivos.txt creado correctamente\033[0m" << endl;
        return true;
    }

    return false;
}
