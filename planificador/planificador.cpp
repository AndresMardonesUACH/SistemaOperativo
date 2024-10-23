#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <filesystem>
#include <sstream>
#include "dotenv.h"
using namespace std;

string seleccionarCore() {
    dotenv::init();
    string ruta = dotenv::getenv("CORES");

    for (const auto& entry : filesystem::directory_iterator(ruta)) {
        string archivoCore = entry.path().string();

        ifstream archivo(archivoCore);

        if (archivo.is_open()) {
            char contenido;
            archivo >> contenido;

            if (contenido == '0') {
                archivo.close();

                ofstream archivoSalida(archivoCore);
                archivoSalida << '1';
                archivoSalida.close();

                string nombreArchivo = entry.path().stem().string();
                return nombreArchivo;
            }

            archivo.close();
        }
    }

    return "";
}

vector<string> leerOperaciones(const string& rutaOperaciones) {
    ifstream archivo(rutaOperaciones);

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << rutaOperaciones << endl;
        return {};
    }

    string linea;
    vector<string> operaciones;

    while (getline(archivo, linea)) {
        operaciones.push_back(linea);
    }

    archivo.close();
    return operaciones;
}

void iniciarCores(int cantidadCores) {
    dotenv::init();
    string ruta = dotenv::getenv("CORES");

    for (const auto& entry : filesystem::directory_iterator(ruta)) {
        filesystem::remove(entry.path());
    }

    for (int i = 0; i < cantidadCores; ++i) {
        string archivoCore = ruta + "/" + to_string(i) + ".txt";
        ofstream archivo(archivoCore);
        archivo << '0';
        archivo.close();
    }
}

void distribuir(string mensaje, string pathResultados){
    dotenv::init();
    string comando = dotenv::getenv("DISTRIBUIDOR");
    comando += " -m \"" + mensaje + "\"";
    comando += " -p \"" + pathResultados + "\"";
    system(comando.c_str());
}

int main(int argc, char* argv[]){
    int opt;
    char* rutaOperaciones = nullptr;
    char* cantidadCoresStr = nullptr;
    char* rutaResultados = nullptr;

    while ((opt = getopt(argc, argv, "o:c:r:")) != -1){
        switch (opt){
        case 'o':
            rutaOperaciones = optarg;
            break;
        case 'c':
            cantidadCoresStr = optarg;
            break;
        case 'r':
            rutaResultados = optarg;
            break;
        default:
            break;
        }
    }

    // Se valida que todos los argumentos hayan sido ingresados
    if(!rutaOperaciones){
        cout << "ERROR. Debe ingresar path de procesos -p" << endl;
        exit(EXIT_FAILURE);
    }
    if(!cantidadCoresStr){
        cout << "ERROR. Debe ingresar path de entrada -c" << endl;
        exit(EXIT_FAILURE);
    }
    if(!rutaResultados){
        cout << "ERROR. Debe ingresar path de resultados -r" << endl;
        exit(EXIT_FAILURE);
    }

    if(!filesystem::exists(rutaOperaciones)) {
        cout << "ERROR. El archivo de operaciones no existe." << endl;
        exit(EXIT_FAILURE);
    }

    ofstream archivo(rutaResultados, ios::trunc);

    vector<string> operaciones = leerOperaciones(rutaOperaciones);

    iniciarCores(stoi(cantidadCoresStr));

    string operacion, core;

    while (!operaciones.empty()) {
        do {
            core = seleccionarCore(); 
        } while (core != "");

        operacion = operaciones.back();
        operaciones.pop_back();

        distribuir(core + ":" + operacion, rutaResultados);
    }

    return 0;
}
