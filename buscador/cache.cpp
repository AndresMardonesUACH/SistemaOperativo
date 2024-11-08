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


void validarParametros(char* portMotorDeBusqueda, char* portCache, char* ipServer, char* tamano) {

    if (portMotorDeBusqueda == nullptr) {
        cerr << "Error: Debe ingresar el valor del puerto para el motor de búsqueda." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (!esNumero(portMotorDeBusqueda)) {
        cerr << "Error: El puerto del motor de búsqueda debe ser un valor numérico válido." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (portCache == nullptr) {
        cerr << "Error: Debe ingresar el valor del puerto para la caché." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (!esNumero(portCache)) {
        cerr << "Error: El puerto de caché debe ser un valor numérico válido." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (ipServer == nullptr) {
        cerr << "Error: Debe ingresar la dirección IP del servidor." << endl;
        exit(EXIT_FAILURE);
    }

    if (tamano == nullptr) {
        cerr << "Error: Debe ingresar el valor de tamaño de la caché." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (!esNumero(tamano)) {
        cerr << "Error: El tamaño de caché debe ser un valor numérico válido." << endl;
        exit(EXIT_FAILURE);
    }

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

// Funcion para manejar cada cliente entrante
void handleClient(int clientSocket, int clientSocketServer, int tamano_cache) {
    unordered_map<string, string> cache;
    int busquedas = 0;
    int i = 0;
    char buffer[1024];
    ssize_t bytesRead;
    string respuesta;
    bool continua = true;
    // Bucle para manejar las opciones del cliente
    while (continua) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        for (auto i: cache){
            cout << i.first << " ";
        }
        cout << endl;
        if (strcmp(buffer, "salir ahora") == 0) {
            continua = false;
            sendMessage(clientSocketServer, buffer);
            receiveMessage(clientSocketServer);
        } else {
            auto it = cache.find(buffer);
            if (it != cache.end()) {
                respuesta = cache[buffer];
            } else {
                sendMessage(clientSocketServer, buffer);
                respuesta = receiveMessage(clientSocketServer);
                if (busquedas == tamano_cache) {
                    auto it = cache.begin();
                    advance(it, i);
                    cache.erase(it);
                    if (i == (tamano_cache-1)) i = 0;
                    else i++;
                } else {
                    busquedas++;
                }
                cache[buffer] = respuesta;
            }
            send(clientSocket, respuesta.c_str(), respuesta.length(), 0);
        }
    }
    cout << "Cliente desconectado " << endl;
    close(clientSocketServer); // Cerrar el socket del cliente al recibir la opción de salida
    close(clientSocket);
    exit(EXIT_SUCCESS);
}


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


int main(int argc, char* argv[]) {
    int opt;
    char* portMotorDeBusqueda = nullptr;
    char* portCache = nullptr;
    char* ipServer = nullptr;
    char* tamano = nullptr;
    unordered_map<string, string> cache;
    
    while ((opt = getopt(argc, argv, "b:c:i:t:")) != -1){
        switch (opt){
        case 'b':
            portMotorDeBusqueda = optarg;
            break;
        case 'c':
            portCache = optarg;
            break;
        case 'i':
            ipServer = optarg;
            break;
        case 't':
            tamano = optarg;
            break;
        default:
            break;
        }
    }

    validarParametros(portMotorDeBusqueda, portCache, ipServer, tamano);

    int PORT_CACHE = stoi(portCache);
    int tamano_cache = stoi(tamano);
    
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
    serverAddr.sin_port = htons(PORT_CACHE); // Puerto del servidor
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

    int PORT_MOTOR = stoi(portMotorDeBusqueda);
    const char* IP_SERVER = ipServer;

    int clientSocketServer = connectServer(IP_SERVER, PORT_MOTOR); // Conectar al servidor

    while (true) {
        // Aceptar conexion entrante
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error al aceptar la conexión");
            continue; // Continuar esperando conexiones
        }

        cout << "Cliente conectado" << endl;
        // Crear un nuevo hilo para manejar la conexión del cliente (solucion a desconexiones automaticas y caidas)
        thread(handleClient, clientSocket, clientSocketServer, tamano_cache).detach(); 
        
    }

    return 0;
}