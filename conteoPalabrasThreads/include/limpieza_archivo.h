#include <iostream>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <sstream>
#include <fstream>
#include <locale>

using namespace std;

// Cargar las palabras de stop desde un archivo
unordered_set<string> cargarStopWords(const string& archivoStopWords) {
    unordered_set<string> stopWords;
    ifstream archivo(archivoStopWords);
    archivo.imbue(locale("en_US.UTF-8"));
    string palabra;

    while (archivo >> palabra) {
        stopWords.insert(palabra);
    }

    return stopWords;
}

// Cargar los IDs de los archivos desde un archivo
unordered_map<string, string> cargarIdsArchivos(const string& archivoIdsArchivos) {
    unordered_map<string, string> idsArchivos;
    ifstream archivo(archivoIdsArchivos);
    archivo.imbue(locale("en_US.UTF-8"));

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string clave, valor;

        if (getline(ss, clave, ',') && getline(ss, valor)) {
            idsArchivos[clave] = valor;
        } else {
            cerr << "Error procesando línea: " << linea << endl;
        }
    }

    return idsArchivos;
}

// Limpiar una palabra eliminando caracteres no alfanuméricos
string limpiarPalabra(const string& palabra) {
    string palabraLimpia;

    for (char c : palabra) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (isalnum(uc) || (uc >= 0xC0 && uc <= 0xFF)) {
            palabraLimpia += tolower(uc);
        }
    }

    return palabraLimpia;
}

// Procesar un libro y contar las palabras, excluyendo stop words
void procesarLibro(const unordered_set<string>& stopWords, const string& archivoLibro, const string& pathSalida,
                   const string& idArchivo, const string& extension, mutex& mtx) {
    ifstream archivo(archivoLibro);
    archivo.imbue(locale("en_US.UTF-8"));

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivoLibro << endl;
        return;
    }

    unordered_map<string, int> conteoPalabras;
    string palabra;

    while (archivo >> palabra) {
        palabra = limpiarPalabra(palabra);

        if (!palabra.empty() && stopWords.find(palabra) == stopWords.end()) {
            conteoPalabras[palabra]++;
        }
    }

    archivo.close();

    // Escribir el conteo de palabras en el archivo de salida
    ofstream archivoSalida(pathSalida + "/" + idArchivo + "." + extension);
    archivoSalida.imbue(locale("en_US.UTF-8"));

    if (!archivoSalida.is_open()) {
        cerr << "No se pudo crear el archivo de salida para: " << idArchivo + "." + extension << endl;
        return;
    }

    for (const auto& [palabra, conteo] : conteoPalabras) {
        archivoSalida << palabra << "; " << conteo << endl;
    }

    archivoSalida.close();
}

// Función principal para procesar múltiples libros usando hilos
void procesarLibros(const string& pathEntrada, const string& pathSalida, const string& mapaArchivos,
                    const string& extension, const string& archivoStopWords, int cantidadThreads) {
    unordered_set<string> stopWords = cargarStopWords(archivoStopWords);
    unordered_map<string, string> idsArchivos = cargarIdsArchivos(mapaArchivos);

    // Mutex para proteger el acceso concurrente a los resultados
    mutex mtxResultado;

    vector<thread> hilos;
    int archivosPorHilo = idsArchivos.size() / cantidadThreads;
    int resto = idsArchivos.size() % cantidadThreads;

    auto it = idsArchivos.begin();

    for (int i = 0; i < cantidadThreads; ++i) {
        int cantidadActual = archivosPorHilo + (i < resto ? 1 : 0);

        hilos.emplace_back([&, cantidadActual, it]() mutable {
            for (int j = 0; j < cantidadActual; ++j) {
                const string& archivo = it->first;
                const string& id = it->second;
                ++it;
                procesarLibro(stopWords, pathEntrada + "/" + archivo, pathSalida, id, extension, mtxResultado);
            }
        });
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    cout << "\033[32mConteo de palabras paralelo con threads realizado correctamente." << endl << idsArchivos.size() << " archivos procesados." << endl << "Salida en " << pathSalida << "\033[0m" << endl;
}
