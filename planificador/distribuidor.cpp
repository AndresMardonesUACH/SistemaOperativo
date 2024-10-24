#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include "dotenv.h"
using namespace std;

void liberarCore(string core) {
    dotenv::init();

    string archivoCore = dotenv::getenv("CORES") + "/" + core + ".txt";

    ofstream archivo(archivoCore, ios::trunc);

    if (archivo.is_open()) {
        archivo << "0";
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo: " << archivoCore << endl;
    }
}

string distribuir(string mensaje){
    stringstream ss(mensaje);
    string token;
    string idCore, idOperacion, operacion, valor1, valor2;

    getline(ss, token, ';');
    stringstream tokenStream(token);
    getline(tokenStream, token, ':');
    idCore = token;
    getline(tokenStream, token, ':');
    idOperacion = token;

    getline(ss, operacion, ';'); 

    getline(ss, token, ',');
    valor1 = token;
    getline(ss, token, ',');
    valor2 = token;

    dotenv::init();
    string comando = dotenv::getenv("CORE");
    comando += " -o" + operacion;
    comando += " -p" + valor1;
    comando += " -s" + valor2;

    FILE* pipe = popen(comando.c_str(), "r");
    if (!pipe) {
        liberarCore(idCore);
        return "Error al ejecutar el comando";
    }

    char buffer[128];
    string resultado;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        resultado += buffer;
    }
    pclose(pipe);

    liberarCore(idCore);

    return resultado;
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
    char* pathrutaResultados = nullptr;

    while ((opt = getopt(argc, argv, "m:p:")) != -1){
        switch (opt){
        case 'm':
            mensaje = optarg;
            break;
        case 'p':
            pathrutaResultados = optarg;
            break;
        default:
            break;
        }
    }

    string resultado = distribuir(mensaje);
    comunicarResultado(pathrutaResultados, mensaje, resultado);
}