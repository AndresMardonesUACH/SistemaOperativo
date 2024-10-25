#include <filesystem>
#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>
using namespace std;

void verificarParametros(char* rutaProcesos, char* cantidadCoresStr, char* rutaResultados, char* rutaCore, char* rutaDistribuidor) {
    if (!rutaProcesos) {
        cout << "ERROR. Debe ingresar path de procesos -o" << endl;
        exit(EXIT_FAILURE);
    }
    if (!cantidadCoresStr) {
        cout << "ERROR. Debe ingresar cantidad de cores -n" << endl;
        exit(EXIT_FAILURE);
    }
    if (!rutaResultados) {
        cout << "ERROR. Debe ingresar path de resultados -r" << endl;
        exit(EXIT_FAILURE);
    }
    if (!rutaCore) {
        cout << "ERROR. Debe ingresar path de cores -c" << endl;
        exit(EXIT_FAILURE);
    }
    if (!rutaDistribuidor) {
        cout << "ERROR. Debe ingresar path de distribuidor -d" << endl;
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

    if (!filesystem::exists(rutaCore)) {
        cout << "ERROR. El ejecutable de cores no existe." << endl;
        exit(EXIT_FAILURE);
    }

    if (!filesystem::exists(rutaDistribuidor)) {
        cout << "ERROR. El ejecutable de distribuidor no existe." << endl;
        exit(EXIT_FAILURE);
    }

    int cantidadCores = stoi(cantidadCoresStr);
    if (cantidadCores <= 0) {
        cout << "ERROR. La cantidad de cores debe ser mayor a 0." << endl;
        exit(EXIT_FAILURE);
    }
}

string seleccionarCore(string rutaCores) {
    for (const auto& entry : filesystem::directory_iterator(rutaCores)) {
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

void limpiarCores(string rutaCores){
    for (const auto& entry : filesystem::directory_iterator(rutaCores)) {
        filesystem::remove(entry.path());
    }
}

void iniciarCores(int cantidadCores, string rutaCores) {
    limpiarCores(rutaCores);

    for (int i = 0; i < cantidadCores; ++i) {
        string archivoCore = rutaCores + "/" + to_string(i) + ".txt";
        ofstream archivo(archivoCore);
        archivo << '0';
        archivo.close();
    }
}

void distribuir(string mensaje, string pathResultados, string rutaDistribuidor, string rutaCore, string rutaCores){
    string comando = rutaDistribuidor;
    comando += " -m \"" + mensaje + "\"";
    comando += " -p \"" + pathResultados + "\"";
    comando += " -c \"" + rutaCore + "\"";
    comando += " -s \"" + rutaCores + "\"";
    system(comando.c_str());
}

int main(int argc, char* argv[]){
    int opt;
    char* rutaProcesos = nullptr;
    char* cantidadCoresStr = nullptr;
    char* rutaResultados = nullptr;
    char* rutaCore = nullptr;
    char* rutaCores = nullptr;
    char* rutaDistribuidor = nullptr;

    while ((opt = getopt(argc, argv, "o:n:r:c:d:s:")) != -1){
        switch (opt){
        case 'o':
            rutaProcesos = optarg;
            break;
        case 'n':
            cantidadCoresStr = optarg;
            break;
        case 'r':
            rutaResultados = optarg;
            break;
        case 'c':
            rutaCore = optarg;
            break;
        case 'd':
            rutaDistribuidor = optarg;
            break;
        case 's':
            rutaCores = optarg;
            break;
        default:
            break;
        }
    }

    verificarParametros(rutaProcesos, cantidadCoresStr, rutaResultados, rutaCore, rutaDistribuidor);

    ofstream archivo(rutaResultados, ios::trunc);

    vector<string> procesos = leerProcesos(rutaProcesos);

    iniciarCores(stoi(cantidadCoresStr), rutaCores);

    string operacion, core;
    pid_t pid;
    bool mardones = true;
    int hijos = 0;

    cout << "Procesando..." << endl;

    while (!procesos.empty()) {
        do {
            core = seleccionarCore(rutaCores); 
        } while (core == "");

        operacion = procesos.back();
        procesos.pop_back();

        pid = fork();
        hijos++;
        
        if (pid == 0) {
            distribuir(core + ":" + operacion, rutaResultados, rutaDistribuidor, rutaCore, rutaCores);
            return EXIT_SUCCESS;
        }        
    }

    if (pid != 0){
        for (int i = 0; i < hijos; i++){
            wait(NULL);
        }
        limpiarCores(rutaCores);
    }

    cout << "\033[32mPlanificador ejecutado correctamente." << endl << "Salida de resultados en " << rutaResultados << "\033[0m" << endl;
    cout << "Presione ENTER para volver...";
    cin.get();
    return EXIT_SUCCESS;
}
