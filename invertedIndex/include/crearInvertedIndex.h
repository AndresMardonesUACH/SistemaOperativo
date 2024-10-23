#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <tuple>
#include <sys/stat.h>
#include <unordered_map>
using namespace std;

// Función para obtener el ID desde una línea
string getId(const string& linea) {
    int i = linea.length() - 1;
    while (i >= 0 && linea[i] != ',' && linea[i] != ' ') {
        i--;
    }
    return linea.substr(i + 1);
}

// Función para escribir el índice invertido en un archivo
bool escribirInvertedIndex(const unordered_map<string, vector<tuple<int, int>>>& mapPalabras, const char* inverted_index) {
    ofstream archivo(inverted_index);
    if (!archivo) {
        cerr << "ERROR. No se pudo crear/acceder archivo inverted_index" << endl;
        return false;
    }

    for (const auto& x : mapPalabras) {
        archivo << x.first << ";";
        for (const auto& par : x.second) {
            archivo << "(ID" << get<0>(par) << "," << get<1>(par) << ");";
        }
        archivo << endl;
    }
    cout << "\033[32mProceso de inverted index terminado." << endl;
    cout << "Archivo " << inverted_index << " creado correctamente\033[0m" << endl;
    archivo.close();
    return true;
}

// Función para crear el índice invertido a partir del archivo de mapeo
bool crearInvertedIndex(const string& salida, const char* mapa_archivos, const char* inverted_index, const string& extension) {
    ifstream archivo(mapa_archivos);
    if (!archivo) {
        cerr << "ERROR. No se pudo abrir mapa_archivos" << endl;
        return false;
    }

    string linea;
    // Mapa que almacena palabra y sus respectivos IDs y cantidades
    unordered_map<string, vector<tuple<int, int>>> mapPalabras;

    while (getline(archivo, linea)) {
        string id = getId(linea);
        ifstream libro(salida + "/" + id + ".txt");
        if (!libro) {
            cerr << "ERROR. No se pudo leer libro " << id << ".txt" << endl;
            return false;
        }

        string lineaPalabra;
        while (getline(libro, lineaPalabra)) {
            string palabra = "";
            int i = 0;
            while (lineaPalabra[i] != ';' && i < lineaPalabra.length()) {
                palabra += lineaPalabra[i++];
            }

            string cantidad = (lineaPalabra[i + 1] == ' ') 
                              ? lineaPalabra.substr(i + 2) 
                              : lineaPalabra.substr(i + 1);
            mapPalabras[palabra].push_back(make_tuple(stoi(id), stoi(cantidad)));
        }
        libro.close();  // Cerrar el libro después de leerlo
    }
    archivo.close();

    return escribirInvertedIndex(mapPalabras, inverted_index);
}
