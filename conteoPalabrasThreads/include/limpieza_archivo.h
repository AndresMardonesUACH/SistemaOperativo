#include <iostream>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <sstream>
using namespace std;

unordered_set<string> cargar_stop_words(const string& archivo_stop_words) {
    unordered_set<string> stop_words;
    ifstream archivo(archivo_stop_words);
    archivo.imbue(locale("en_US.UTF-8"));
    string palabra;

    while (archivo >> palabra) {
        stop_words.insert(palabra);
    }

    return stop_words;
}
unordered_map<string, string> cargar_ids_archivos(const string& archivo_ids_archivos) {
    unordered_map<string, string> ids_archivos;
    ifstream archivo(archivo_ids_archivos);
    archivo.imbue(locale("en_US.UTF-8"));

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string clave, valor;

        if (getline(ss, clave, ',') && getline(ss, valor)) {
            ids_archivos[clave] = valor;
        } else {
            cerr << "Error procesando línea: " << linea << endl;
        }
    }

    return ids_archivos;
}

string limpiar_palabra(const string& palabra) {
    string palabra_limpia;

    for (size_t i = 0; i < palabra.size(); ++i) {
        unsigned char c = palabra[i];

        if (isalnum(c) || (c >= 0xC0 && c <= 0xFF)) {
            palabra_limpia += tolower(c);
        }
    }
    
    return palabra_limpia;
}

void procesar_libro(const unordered_set<string>& stop_words, const string& archivo_libro, const string& path_salida, const string& id_archivo, const string& extension, mutex& mtx) {
    ifstream archivo(archivo_libro);
    archivo.imbue(locale("en_US.UTF-8"));  // Leer el archivo como UTF-8
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo_libro << endl;
        return;
    }

    unordered_map<string, int> conteo_palabras;
    string palabra;

    // Procesar cada palabra del archivo
    while (archivo >> palabra) { 
        palabra = limpiar_palabra(palabra);

        // Solo contar si no es una stopword y no está vacía
        if (!palabra.empty() && stop_words.find(palabra) == stop_words.end()) {
            conteo_palabras[palabra]++;
        }
    }

    archivo.close();

    // Crear archivo de salida
    ofstream archivo_salida(path_salida + "/" + id_archivo + "." + extension);
    archivo_salida.imbue(locale("en_US.UTF-8"));  // Aseguramos que la salida sea en UTF-8
    
    if (!archivo_salida.is_open()) {
        cerr << "No se pudo crear el archivo de salida para: " << id_archivo + "." + extension << endl;
        return;
    }

    // Escribir resultados en el archivo de salida
    for (const auto& [palabra, conteo] : conteo_palabras) {
        archivo_salida << palabra << "; " << conteo << endl;
    }

    archivo_salida.close();
}

// Función principal para procesar los libros
void procesar_libros(string pathEntrada, string pathSalida, string mapa_archivos, string extension, string archivo_stop_words, int cantidad_thread) {
    unordered_set<string> stop_words = cargar_stop_words(archivo_stop_words);
    unordered_map<string, string> ids_archivos = cargar_ids_archivos(mapa_archivos);

    // Obtener la lista de archivos en el directorio de entrada
    DIR* dir;
    struct dirent* entry;
    vector<string> rutas_libros;
    
    if ((dir = opendir(pathEntrada.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string archivo(entry->d_name);
            // Filtrar por extensión
            if (archivo.find("." + extension) != string::npos) {
                rutas_libros.push_back(archivo);
            }
        }
        closedir(dir);
    } else {
        cerr << "No se pudo abrir el directorio: " << pathEntrada << endl;
        return;
    }

    // Mutex para proteger el acceso concurrente al archivo de salida
    mutex mtx_resultado;

    // Crear y lanzar los threads
    vector<thread> hilos;
    int archivos_por_hilo = rutas_libros.size() / cantidad_thread;
    int resto = rutas_libros.size() % cantidad_thread;

    for (int i = 0; i < cantidad_thread; ++i) {
        int inicio = i * archivos_por_hilo;
        int fin = (i == cantidad_thread - 1) ? rutas_libros.size() : inicio + archivos_por_hilo;

        hilos.emplace_back([&, inicio, fin]() {
            for (int j = inicio; j < fin; ++j) {
                if (ids_archivos.find(rutas_libros[j]) != ids_archivos.end()) {
                    procesar_libro(stop_words, pathEntrada + "/" + rutas_libros[j], pathSalida, ids_archivos.at(rutas_libros[j]), extension, mtx_resultado);
                } else {
                    cerr << "ID no encontrado para el archivo: " << rutas_libros[j] << endl;
                }
            }
        });
    }

    // Unir los threads
    for (auto& hilo : hilos) {
        hilo.join();
    }
}