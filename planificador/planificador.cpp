#include <filesystem>
#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include "dotenv.h"
using namespace std;

void verificarParametros(char* rutaProcesos, char* cantidadCoresStr, char* rutaResultados) {
    if (!rutaProcesos) {
        cout << "ERROR. Debe ingresar path de procesos -o" << endl;
        exit(EXIT_FAILURE);
    }
    if (!cantidadCoresStr) {
        cout << "ERROR. Debe ingresar cantidad de cores -c" << endl;
        exit(EXIT_FAILURE);
    }
    if (!rutaResultados) {
        cout << "ERROR. Debe ingresar path de resultados -r" << endl;
        exit(EXIT_FAILURE);
    }

    if (!filesystem::exists(rutaProcesos)) {
        cout << "ERROR. El archivo de procesos no existe." << endl;
        exit(EXIT_FAILURE);
    }

    if (!filesystem::exists(rutaResultados)) {
        cout << "ERROR. El archivo de resultados no existe." << endl;
        exit(EXIT_FAILURE);
    }

    int cantidadCores = stoi(cantidadCoresStr);
    if (cantidadCores <= 0) {
        cout << "ERROR. La cantidad de cores debe ser mayor a 0." << endl;
        exit(EXIT_FAILURE);
    }
}

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

vector<string> leerProcesos(const string& rutaProcesos) {
    ifstream archivo(rutaProcesos);

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << rutaProcesos << endl;
        return {};
    }

    string linea;
    vector<string> procesos;

    while (getline(archivo, linea)) {
        procesos.push_back(linea);
    }

    archivo.close();
    return procesos;
}

void limpiarCores(){
    dotenv::init();
    string ruta = dotenv::getenv("CORES");

    for (const auto& entry : filesystem::directory_iterator(ruta)) {
        filesystem::remove(entry.path());
    }
}

void iniciarCores(int cantidadCores) {
    dotenv::init();
    string ruta = dotenv::getenv("CORES");

    limpiarCores();

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
    char* rutaProcesos = nullptr;
    char* cantidadCoresStr = nullptr;
    char* rutaResultados = nullptr;

    while ((opt = getopt(argc, argv, "o:c:r:")) != -1){
        switch (opt){
        case 'o':
            rutaProcesos = optarg;
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

    verificarParametros(rutaProcesos, cantidadCoresStr, rutaResultados);

    ofstream archivo(rutaResultados, ios::trunc);

    vector<string> procesos = leerProcesos(rutaProcesos);

    iniciarCores(stoi(cantidadCoresStr));

    string operacion, core;
    pid_t pid;
    bool mardones = true;
    int hijos = 0;

    cout << "Procesando..." << endl;

    while (!procesos.empty()) {
        do {
            core = seleccionarCore(); 
        } while (core == "");

        operacion = procesos.back();
        procesos.pop_back();

        pid = fork();
        hijos++;
        
        if (pid == 0) {
            distribuir(core + ":" + operacion, rutaResultados);
            return EXIT_SUCCESS;
        }        
    }

    if (pid != 0){
        for (int i = 0; i < hijos; i++){
            wait(NULL);
        }
        limpiarCores();
    }

    cout << "\033[32mPlanificador ejecutado correctamente." << endl << "Salida de resultados en " << rutaResultados << "\033[0m" << endl;
    cout << "Presione ENTER para volver..." << endl;
    cin.get();
    return EXIT_SUCCESS;
}
