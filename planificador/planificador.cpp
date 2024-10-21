#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <filesystem>
#include <sstream>
using namespace std;

void liberarCore(int* cores, int core){
    cores[core] = 0;
}

int seleccionarCore(int* cores, int cantidad_cores) {
    for (int i = 0; i < cantidad_cores; i++) {
        if (cores[i] == 0){
            cores[i] = 1;
            return i;
        }
    }
    return -1;
}

string core(string operacion, float x, float y) {
    if (operacion == "suma") {
        return to_string(x+y);
    } else if (operacion == "resta") {
        return to_string(x-y);
    } else if (operacion == "division") {
        if (y == 0) return "Error, no se puede dividir por 0.";
        return to_string(x/y);
    } else if (operacion == "multiplicacion") {
        return to_string(x*y);
    }
    return "La operación no está definida.";
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

void distribuidor(string mensaje, int* cores, string pathrutaResultados) {
    stringstream ss(mensaje);
    string token;
    int idCore, idOperacion;
    string operacion;
    float valor1, valor2;

    getline(ss, token, ';');
    stringstream tokenStream(token);
    getline(tokenStream, token, ':');
    idCore = stoi(token);
    getline(tokenStream, token, ':');
    idOperacion = stoi(token);

    getline(ss, operacion, ';'); 

    getline(ss, token, ',');
    valor1 = stof(token);
    getline(ss, token, ',');
    valor2 = stof(token);

    string resultado = core(operacion, valor1, valor2);
    liberarCore(cores, idCore);

    ofstream archivo(pathrutaResultados, ios::app); 
    if (archivo.is_open()) {
        archivo << "(" << mensaje << ") => " << resultado << endl;
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo." << endl;
    }
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

    int cantidad_cores = stoi(cantidadCoresStr);
    int* cores = new int[cantidad_cores];

    for (int i = 0; i < cantidad_cores; i++) {
        cores[i] = 0;
    }

    int core;
    string operacion;

    while (!operaciones.empty()) {
        vector<string> llamadasAcumuladas;

        int core = seleccionarCore(cores, cantidad_cores);
        
        while (core != -1 && !operaciones.empty()) {
            string operacion = operaciones.back();
            operaciones.pop_back();
            llamadasAcumuladas.push_back(to_string(core) + ":" + operacion);
            core = seleccionarCore(cores, cantidad_cores);
        }

        // Ejecuta todas las llamadas acumuladas
        for (const auto& llamada : llamadasAcumuladas) {
            distribuidor(llamada, cores, rutaResultados);
        }
    }

    return 0;
}