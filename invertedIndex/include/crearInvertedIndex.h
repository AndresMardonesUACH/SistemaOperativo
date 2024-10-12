#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <tuple>
#include <sys/stat.h>
#include <unordered_map>
using namespace std;

string getId(string linea){
    int i = linea.length() - 1;
    while(linea[i] != ',' && linea[i] != ' ') i--;
    return linea.substr(i+1, linea.length());
}

bool escribirInvertedIndex(unordered_map<string, vector<tuple<int, int>>> mapPalabras, const char* inverted_index){
    ofstream archivo(inverted_index);
    if(!archivo){
        cout << "ERROR. No se pudo crear/acceder archivo inverted_index" << endl;
        return false;
    }
    for(auto x: mapPalabras){
        archivo << x.first << ";";
        for (int i = 0; i < x.second.size(); i++){
            archivo << "(ID" << get<0>(x.second[i]) << "," << get<1>(x.second[i]) << ");";
        }
        archivo << endl;
    }
    cout << "\033[32mArchivo inverted_index.txt creado correctamente\033[0m" << endl;
    archivo.close();
    return true;
}


bool crearInvertedIndex(string salida, const char* mapa_archivos, const char* inverted_index, string extension){
    ifstream archivo(mapa_archivos);
    if(!archivo){
        cout << "ERROR. No se pudo abrir mapa_archivos" << endl;
        return false;
    }
    string linea;
    //hashmap -> palabra: [(id1, cant), (id2, cant)...]
    unordered_map<string, vector<tuple<int, int>>> mapPalabras;
    while (getline(archivo, linea)){
        string id = getId(linea);
        ifstream libro(salida + "/" + getId(linea) + ".txt");
        if(!libro){
            cout << "ERROR. No se pudo leer libro " << id << ".txt" << endl;
            return false;
        }
        string lineaPalabra;
        while(getline(libro, lineaPalabra)){
            string palabra = "";
            int i = 0;
            while(lineaPalabra[i] != ';'){
                palabra += lineaPalabra[i];
                i++;
            }

            string cantidad;
            if(lineaPalabra[i+1] == ' ') cantidad = lineaPalabra.substr(i+2, lineaPalabra.length());
            else cantidad = lineaPalabra.substr(i+1, lineaPalabra.length());
            mapPalabras[palabra].push_back(make_tuple(stoi(id), stoi(cantidad)));
        }
    }
    archivo.close();
    escribirInvertedIndex(mapPalabras, inverted_index);
    return true;


}