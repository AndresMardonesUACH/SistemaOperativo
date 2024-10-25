#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace std;

void verificarParametros(const string& mensaje, const string& rutaResultados, const string& rutaCore, const string& rutaCores) {
    if (mensaje.empty()) {
        cerr << "Error: El mensaje no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }

    if (rutaResultados.empty()) {
        cerr << "Error: El path de resultados no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }

    if (rutaCore.empty()) {
        cerr << "Error: El path de core no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }

    if (rutaCores.empty()) {
        cerr << "Error: El path de cores no ha sido ingresado." << endl;
        exit(EXIT_FAILURE);
    }
}

void liberarCore(string core, string rutaCores) {
    string archivoCore = rutaCores + "/" + core + ".txt";

    ofstream archivo(archivoCore, ios::trunc);

    if (archivo.is_open()) {
        archivo << "0";
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo: " << archivoCore << endl;
    }
}

string distribuir(string mensaje, string rutaCore, string rutaCores){
    stringstream ss(mensaje);
    string idCore, idOperacion, operacion, valor1, valor2;

    getline(ss, idCore, ':');
    getline(ss, idOperacion, ';');
    getline(ss, operacion, ';');
    getline(ss, valor1, ',');
    getline(ss, valor2, ',');

    if (idCore.empty()) {
        cerr << "Error: id del core no identificado." << endl;
        exit(EXIT_FAILURE);
    }
    if (idOperacion.empty()) {
        cerr << "Error: id de la operación no identificada." << endl;
        exit(EXIT_FAILURE);
    }
    
    string comando = rutaCore;
    comando += " -o" + operacion;
    comando += " -p" + valor1;
    comando += " -s" + valor2;

    FILE* pipe = popen(comando.c_str(), "r");
    if (!pipe) {
        liberarCore(idCore, rutaCores);
        return "Error al ejecutar el comando";
    }

    string resultado;
    char buffer[128];
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        resultado = buffer;
    }

    pclose(pipe);
    liberarCore(idCore, rutaCores);

    if (!resultado.empty()) {
        return resultado;
    } else {
        return "Error al leer el resultado del comando";
    }
}

void comunicarResultado(string pathrutaResultados, string mensaje, string resultado){
    ofstream archivo(pathrutaResultados, ios::app);

    if (archivo.is_open()) {
        archivo << "(" << mensaje << ") => " << resultado << endl;
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo de resultados." << endl;
    }
}

int main(int argc, char* argv[]){
    int opt;
    char* mensaje = nullptr;
    char* rutaResultados = nullptr;
    char* rutaCore = nullptr;
    char* rutaCores = nullptr;

    while ((opt = getopt(argc, argv, "m:p:c:s:")) != -1){
        switch (opt){
        case 'm':
            mensaje = optarg;
            break;
        case 'p':
            rutaResultados = optarg;
            break;
        case 'c':
            rutaCore = optarg;
            break;
        case 's':
            rutaCores = optarg;
            break;
        default:
            break;
        }
    }

    verificarParametros(mensaje, rutaResultados, rutaCore, rutaCores);

    string resultado = distribuir(mensaje, rutaCore, rutaCores);

    comunicarResultado(rutaResultados, mensaje, resultado);
}