#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include <string.h>
#include <thread>
#include <vector>
using namespace std;


// Verifica si un directorio existe
bool existeDirectorio(const char* ruta) {
    struct stat info;
    if (stat(ruta, &info) != 0) {
        return false;  // Error al acceder a la ruta
    }
    return (info.st_mode & S_IFDIR) != 0;
}

// Verifica si la extensiรณn del archivo coincide con la extensiรณn esperada
bool esExtensionValida(const char* nombreArchivo, const char* extension) {
    const char* punto = strrchr(nombreArchivo, '.');
    if (!punto || punto == nombreArchivo) return false;
    return strcmp(punto + 1, extension) == 0;
}

bool validaArchivo(const string pathDatos){
    filesystem::path rutaCompleta(pathDatos);
    string rutaDatos = rutaCompleta.parent_path();
    return existeDirectorio(rutaDatos.c_str());
}

bool esInt(string num){
    int i = 0;
    while (i < num.size()){
        if(!isdigit(num[i])) return false;
        i++;
    }
    return true;
}

bool threadValido(int cantidadThreads){
    unsigned int maxThreads = thread::hardware_concurrency();
    if (cantidadThreads <= 0 || cantidadThreads > maxThreads) {
        cerr << "La cantidad de threads debe ser mayor que 0 y menor o igual al máximo de threads disponibles: " << maxThreads << endl;
        return false;
    }
    return true;
}

bool creaArrayThreads(vector<int>& threads, const char* array_threads){
    int i = 0;
    string thread = "";
    while(i < string(array_threads).size()){
        if(array_threads[i] == ','){
            if(!threadValido(stoi(thread))) return false;
            threads.push_back(stoi(thread));
            thread = "";
        }
        else{
            if(isdigit(array_threads[i])){
                thread += array_threads[i];
                if(i == string(array_threads).size() - 1){
                    if(!threadValido(stoi(thread))) return false;
                    threads.push_back(stoi(thread));
                }
            }
            else{
                cerr << "ERROR. Array_threads posee un caracter erróneo" << endl;
                return false;
            } 
        }
        i++;
    }
    return true;
}

