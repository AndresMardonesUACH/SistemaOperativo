#include <iostream>
#include <fstream>
#include <dirent.h>
#include <utilidades.h>
using namespace std;

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
            archivo << entrada->d_name << "," << i << endl;
            i++;
        }
    }
    if(!archivoEncontrado){
        cout << "\033[31mNo existen archivos con la extensión: " << extension << "\033[0m" << endl;
        return false;
    }

    closedir(directorio);
    return true;
}

bool escribirMapaArchivos(string pathEntrada, string pathMapaArchivos, string extension){

    if (!existe_archivo(pathMapaArchivos.c_str())){ 
        cout << "ERROR. La carpeta para mapa_archivos no existe" << endl;
        return false;
    }
    string nuevaRuta = pathMapaArchivos + "/mapa_archivos.txt";
    
    if(procesarDatos(extension, pathEntrada, nuevaRuta)){
        cout << "\033[32mArchivo mapa_archivos.txt creado correctamente\033[0m" << endl;   
        return true;
    }
    return false;

}