#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include "validaciones.h"
#include <vector>
#include <chrono>
using namespace std;

void ejecutarRepeticion(string comando, int thread, const char* datos, int repeticiones){
    ofstream archivo(datos, ios::app);
    archivo << thread << ",";
    
    for(int i = 0; i < repeticiones; i++){    
        cout << "Ejecutando conteo con " << thread << " threads, repetición: " << (i+1) << endl; 
        auto start = chrono::high_resolution_clock::now();
        system(comando.c_str());
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        archivo << duration.count();
        if(i != repeticiones-1) archivo << ",";
    }
    archivo << endl;
    archivo.close();
}


int main(int argc, char* argv[]) {
    int opt;
    char* pathCPT = nullptr;
    char* extension = nullptr;
    char* entrada = nullptr;
    char* salida = nullptr;
    char* stop_words = nullptr;
    char* mapa_archivos = nullptr;
    char* array_threads = nullptr;
    char* repeticiones = nullptr;
    char* datos = nullptr;
    char* grafico = nullptr;
    char* analizador = nullptr;

    // Procesar argumentos de línea de comandos
    while ((opt = getopt(argc, argv, "p:e:i:o:s:m:t:r:d:g:a:")) != -1) {
        switch (opt) {
            case 'p':
                pathCPT = optarg;
                break;
            case 'e':
                extension = optarg;
                break;
            case 'i':
                entrada = optarg;
                break;
            case 'o':
                salida = optarg;
                break;
            case 's':
                stop_words = optarg;
                break;
            case 'm':
                mapa_archivos = optarg;
                break;
            case 't':
                array_threads = optarg;
                break;
            case 'r':
                repeticiones = optarg;
                break;
            case 'd':
                datos = optarg;
                break;
            case 'g':
                grafico = optarg;
                break;
            case 'a':
                analizador = optarg;
                break;
            default:
                break;
        }
    }

    // Validar que todos los argumentos necesarios hayan sido ingresados
    if (!pathCPT) {
        cerr << "ERROR. Debe ingresar pathConteoPalabrasThreads -p" << endl;
        exit(EXIT_FAILURE);
    }
    if (!extension) {
        cerr << "ERROR. Debe ingresar extensión -e" << endl;
        exit(EXIT_FAILURE);
    }
    if (!entrada) {
        cerr << "ERROR. Debe ingresar path de entrada -i" << endl;
        exit(EXIT_FAILURE);
    }
    if (!salida) {
        cerr << "ERROR. Debe ingresar path de salida -o" << endl;
        exit(EXIT_FAILURE);
    }
    if (!stop_words) {
        cerr << "ERROR. Debe ingresar path de stop_words -s" << endl;
        exit(EXIT_FAILURE);
    }
    if (!mapa_archivos) {
        cerr << "ERROR. Debe ingresar path de mapa_archivos -m" << endl;
        exit(EXIT_FAILURE);
    }
    if (!array_threads) {
        cerr << "ERROR. Debe ingresar path de array_threads -t" << endl;
        exit(EXIT_FAILURE);
    }
    if (!repeticiones) {
        cerr << "ERROR. Debe ingresar path de repeticiones -r" << endl;
        exit(EXIT_FAILURE);
    }
    if (!datos) {
        cerr << "ERROR. Debe ingresar path de datos -d" << endl;
        exit(EXIT_FAILURE);
    }
    if (!grafico) {
        cerr << "ERROR. Debe ingresar path de grafico -g" << endl;
        exit(EXIT_FAILURE);
    }
    if (!analizador) {
        cerr << "ERROR. Debe ingresar path de analizador -a" << endl;
        exit(EXIT_FAILURE);
    }
    if(!validaArchivo(datos)){
        cerr << "ERROR. Carpeta de datos no existe" << endl;
        exit(EXIT_FAILURE);
    }
    if(!validaArchivo(grafico)){
        cerr << "ERROR. Carpeta de gráfico no existe" << endl;
        exit(EXIT_FAILURE);
    }
    if(stoi(repeticiones) < 2){
        cerr << "ERROR. Repeticiones debe ser igual o mayor a 2" << endl;
        exit(EXIT_FAILURE);
    }
    if(!esInt(string(repeticiones))){
        cerr << "ERROR. Repeticiones debe ser un número entero" << endl;
        exit(EXIT_FAILURE);
    }

    vector<int> threads;
    if (!creaArrayThreads(threads, array_threads)){
        exit(EXIT_FAILURE);
    }

    string comando;
    ofstream archivo(datos);
    archivo.close();
    
    for(int i = 0; i < threads.size(); i++){
        comando = pathCPT;
        comando += " -e" + string(extension);
        comando += " -i" + string(entrada);
        comando += " -o" + string(salida);
        comando += " -s" + string(stop_words);
        comando += " -m" + string(mapa_archivos);
        comando += " -t" + to_string(threads[i]);
        comando += " > /dev/null 2>&1";

        ejecutarRepeticion(comando, threads[i], datos, stoi(repeticiones));  
    }

    comando = "python3 " + string(analizador);
    comando += " --datos=" + string(datos);
    comando += " --grafico=" + string(grafico);
    system(comando.c_str());
    cout << "\033[32mAnálisis de Performance realizado correctamente." << endl << "Salida de gráfico en " << grafico << "\033[0m" << endl;
    cout << "Presione ENTER para volver...";
    cin.get();
    return 0;
}
