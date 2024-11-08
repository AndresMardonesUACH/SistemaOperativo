#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include <unordered_map>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <vector>
#include <thread>

using namespace std;


bool esNumero(const char* str) {
    if (str == nullptr) return false;
    for (size_t i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}


void validarParametros(char* portMotorDeBusqueda, char* topK, char* inverted_index) {

    if (portMotorDeBusqueda == nullptr) {
        cerr << "Error: No se encontró la variable de entorno PORT" << endl;
        exit(EXIT_FAILURE);
    }

    if (!topK) {
        cerr << "ERROR. Debe ingresar variable topK -t" << endl;
        exit(EXIT_FAILURE);
    }
    if(stoi(topK) < 1){
        cerr << "ERROR. Variable topK debe ser mayor a 1" << endl;
        exit(EXIT_FAILURE);
    }
    if (!inverted_index) {
        cerr << "ERROR. Debe ingresar path de inverted_index -x" << endl;
        exit(EXIT_FAILURE);
    }
}


vector<string> getBusqueda(string linea){
    string aux;
    vector<string> palabras;
    stringstream ss(linea);
    while(getline(ss, aux, ' ')){
        palabras.push_back(aux);
    }
    return palabras;
}

unordered_map<int, tuple<int,int>> getResultados(vector<string> palabras, const char* inverted_index){
    unordered_map<int, tuple<int, int>> resultados;
    ifstream archivo(inverted_index);
    int i = 0;
    string linea, palabra, aux, aux2;
    int id, cant;

    //linea = palabra1;(ID1,Cant1);(ID2,Cant2)
    while(getline(archivo, linea) && i < palabras.size()){
        stringstream ss(linea);
        
        getline(ss, palabra, ';');
        //palabra = palabra1
        int j = 0;
        //recorre el vector de palabras preguntando si es la palabra que se busca
        while(j < palabras.size() && strcmp(palabra.c_str(), palabras[j].c_str()) != 0) j++;
        if(j < palabras.size()){
            //la palabra fue encontrada
            i++;
            while(getline(ss, aux, ';')){
                //aux = (ID1, Cant1)
                stringstream ss2(aux);
                getline(ss2, aux2, ',');
                //id = 1
                id = stoi(aux2.substr(3, aux2.size()-3));
                getline(ss2, aux2, ',');
                //cant = Cant1
                cant = stoi(aux2.substr(0, aux2.size()-1));
                get<0>(resultados[id]) += cant;
                get<1>(resultados[id]) += 1;
            }
        }
    }
    
    return resultados;
}

bool cmp(tuple<int,int> a, tuple<int,int> b){
    return get<1>(a) > get<1>(b);
}

vector<tuple<int, int>> sortResults(unordered_map<int, tuple<int,int>> resultados, int n, int topK){
    vector<tuple<int,int>> sortedResults;
    for(auto i: resultados){
        if (get<1>(i.second) == n) sortedResults.push_back(make_tuple(i.first, get<0>(i.second))); 
    }
    sort(sortedResults.begin(), sortedResults.end(), cmp);
    if(sortedResults.size() > topK){
        vector<tuple<int,int>> firstTopKResults (sortedResults.begin(), sortedResults.begin() + topK);
        return firstTopKResults;
    }
    return sortedResults;
}

// Funcion para manejar cada cliente entrante
void handleClient(int clientSocket, char* inverted_index, char* topK) {
    char buffer[1024];
    ssize_t bytesRead;
    string respuesta;
    bool continua = true;
    // Bucle para manejar las opciones del cliente
    while (continua) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        if (strcmp(buffer, "salir ahora") == 0) {
            continua = false;
        } else {
            cout << "Buscando MOTOR" << endl;
            vector<string> busqueda = getBusqueda(buffer);
            unordered_map<int, tuple<int, int>> resultados = getResultados(busqueda, inverted_index);
            string resultMsg = "";
            vector<tuple<int,int>> sortedResults = sortResults(resultados, busqueda.size(), stoi(topK));
            if(sortedResults.size() == 0){
                resultMsg += "0";
            }
            else{
                busqueda.clear();
                busqueda.shrink_to_fit();
                resultados.clear();
                for(int i = 0; i < sortedResults.size(); i++){
                    resultMsg += to_string(i+1) + ") Puntaje: " + to_string(get<1>(sortedResults[i]));
                    resultMsg += ", " + to_string(get<0>(sortedResults[i])) + "/";
                }
            }
            respuesta = resultMsg;
            send(clientSocket, respuesta.c_str(), respuesta.length(), 0);
        }
    }
    cout << "Cliente desconectado " << endl;
    close(clientSocket); // Cerrar el socket del cliente al recibir la opción de salida
    exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]){
    int opt;
    char* portMotorDeBusqueda = nullptr;
    char* topK = nullptr;
    char* inverted_index = nullptr;

    while ((opt = getopt(argc, argv, "p:t:x:")) != -1){
        switch (opt){
        case 'p':
            portMotorDeBusqueda = optarg;
            break;
        case 't':
            topK = optarg;
            break;
        case 'x':
            inverted_index = optarg;
            break;
        default:
            break;
        }
    }

    validarParametros(portMotorDeBusqueda, topK, inverted_index);

    int PORT = std::stoi(portMotorDeBusqueda);
    
    // Crear socket de servidor
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error al configurar SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Puerto del servidor
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazamos socket a dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al enlazar el socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        perror("Error al escuchar");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    cout << "Esperando conexiones entrantes..." << endl;

    while (true) {
        // Aceptar conexion entrante
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error al aceptar la conexión");
            continue; // Continuar esperando conexiones
        }

        cout << "Cliente conectado" << endl;
        // Crear un nuevo hilo para manejar la conexión del cliente (solucion a desconexiones automaticas y caidas)
        thread(handleClient, clientSocket, inverted_index, topK).detach(); 
        
    }

    return 0;
}