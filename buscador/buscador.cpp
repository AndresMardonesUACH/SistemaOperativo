#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

// Función para conectar al servidor
int connectServer(const string& serverIP, int serverPort) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }
    // Configurar la dirección del servidor
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    // Conectar al servidor y verificar errores
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al conectar al servidor");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    cout << "Conectado al servidor." << endl;
    // Retornar el socket del cliente al servidor
    return clientSocket;
}

// Función para enviar un mensaje al servidor
void sendMessage(int clientSocket, const string& message) {
    cout << "Enviando mensaje al servidor..." << endl;
    send(clientSocket, message.c_str(), message.length(), 0);
}

// Función para recibir y mostrar un mensaje del servidor
string receiveMessage(int clientSocket) {
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        perror("Error al recibir datos del servidor");
        close(clientSocket);
        exit(EXIT_FAILURE);
    } else if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        return buffer;
    }
    return "";
}


string escribirBusqueda(string busqueda){
    string busquedaLimpia;
    for(char c: busqueda){
        unsigned char uc = static_cast<unsigned char>(c);
        if(uc == ' ') busquedaLimpia += " ";
        if (isalnum(uc) || (uc >= 0xC0 && uc <= 0xFF)) {
            busquedaLimpia += tolower(uc);
        }
    }
    return busquedaLimpia;
}

unordered_map<int, string> getMapaArchivos(const char* rutaMapaArchivos){
    ifstream archivo(rutaMapaArchivos);
    string linea, aux;
    unordered_map<int, string> libros;
    while (getline(archivo, linea)){
        stringstream ss(linea);
        getline(ss, aux, ',');
        string nombre = aux.substr(0, aux.size()-4);
        getline(ss, aux, ',');
        libros[stoi(aux)] = nombre;
    }
    return libros;
}

// Función para verificar si el procesamiento ha terminado
bool invertedIndexEjecutado(string ruta) {
    string rutaArchivo = ruta + "/invertedIndexEjecutado.txt";
    ifstream archivoTermino(rutaArchivo);
    if (!archivoTermino) {
        cerr << "ERROR. No se pudo abrir el archivo de estado: " << rutaArchivo << endl;
        return false;
    }

    string linea;
    archivoTermino >> linea;
    return (linea == "true");
}

void escribirResultado(unordered_map<int, string> libros, string mensaje){
    string aux, aux2;
    if(mensaje == "0"){
        cout << "No se encontraron resultados" << endl;
        return;
    }
    vector<string> resultados;
    stringstream ss(mensaje);
    while(getline(ss, aux, '/')){
        stringstream ss2(aux);
        while(getline(ss2, aux2, ',')){
            resultados.push_back(aux2);
        }
    }
    
    for(int i = 0; i < resultados.size(); i++){
        if(i % 2 == 0) cout << resultados[i];
        else{
            resultados[i].replace(0, resultados[i].size(), libros[stoi(resultados[i])]);
            cout << ", " << resultados[i] << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    int opt;
    char* portCache = nullptr;
    char* ipServer = nullptr;
    char* inverted_index = nullptr;
    char* mapa_archivos = nullptr;

    while ((opt = getopt(argc, argv, "p:x:i:m:")) != -1){
        switch (opt){
        case 'p':
            portCache = optarg;
            break;
        case 'i':
            ipServer = optarg;
            break;
        case 'x':
            inverted_index = optarg;
            break;
        case 'm':
            mapa_archivos = optarg;
            break;
        default:
            break;
        }
    }

    if (portCache == nullptr) {
        cerr << "Error: No se encontró la variable de entorno PORT" << endl;
        return -1;
    }
    if (ipServer == nullptr) {
        cerr << "Error: No se encontró la variable de entorno IP_SERVER" << endl;
        return -1;
    }

    if (!inverted_index) {
        cerr << "ERROR. Debe ingresar path de inverted_index -x" << endl;
        exit(EXIT_FAILURE);
    }
    if (!mapa_archivos) {
        cerr << "ERROR. Debe ingresar path de mapa_archivos -m" << endl;
        exit(EXIT_FAILURE);
    }

    filesystem::path rutaCompleta(inverted_index);
    string rutaDatos = rutaCompleta.parent_path();
    if (!invertedIndexEjecutado(rutaDatos)) {
        cerr << "\033[31mERROR. Debe ejecutarse 'Crear Índice Invertido' antes\033[0m" << endl;
        exit(EXIT_FAILURE);
    }

    int PORT = stoi(portCache);
    const char* IP_SERVER = ipServer;

    int clientSocket = connectServer(IP_SERVER, PORT); // Conectar al servidor
    system("clear");
    cout << "Buscador (PID = " << getpid() << ")" << endl;
    cout << "--------------------------------------------------------------" << endl;
    unordered_map<int, string> libros = getMapaArchivos(mapa_archivos);
    string busqueda, respuesta;

    do{
        cout << "Ingrese búsqueda (o escriba 'salir ahora' para salir): ";
        getline(cin, busqueda);
        system("clear");
        cout << "Buscador (PID = " << getpid() << ")" << endl;
        cout << "--------------------------------------------------------------" << endl;
        if(busqueda[0] != ' ' && busqueda != ""){
            if (busqueda != "salir ahora") cout << "Buscando '" << busqueda << "'..." << endl;
            busqueda = escribirBusqueda(busqueda);
            sendMessage(clientSocket, busqueda);
            if (busqueda == "salir ahora") break;
            respuesta = receiveMessage(clientSocket);
            escribirResultado(libros, respuesta);
            cout << "--------------------------------------------------------------" << endl;
        }
        else{
            cout << "Búsqueda inválida, intente de nuevo" << endl;
        }
    } while (true);

    cout << "Cerrando la conexión." << endl;
    close(clientSocket);

    return 0;
}