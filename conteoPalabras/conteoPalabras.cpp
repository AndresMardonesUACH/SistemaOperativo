#include <iostream>
#include <unistd.h>
#include <cstring>
#include <limits>
#include "procesar.h"

using namespace std;

pid_t pid = getpid();

void mostrarInterfaz(char* extension, char* pathProcesar, char* pathRespuesta) {
    const char *noAsignado = "\033[31mNO ASIGNADO\033[0m";
    const char *asignado = "\033[32mASIGNADO:\033[0m";

    cout << "Programa contador de palabras" << endl;
    cout << "pid: " << getpid() << endl;
    cout << "--------------------------------------------------------------";
    cout << endl << "Seleccione una opción:" << endl;
    cout << "(0) Salir." << endl;
    cout << "(1) Extensión de archivos a procesar " 
         << (extension ? asignado : noAsignado) << " " << (extension ? extension : "") << endl;
    cout << "(2) Path de carpeta a procesar " 
         << (pathProcesar ? asignado : noAsignado) << " " << (pathProcesar ? pathProcesar : "") << endl;
    cout << "(3) Path de carpeta que contendrá la respuesta del proceso " 
         << (pathRespuesta ? asignado : noAsignado) << " " << (pathRespuesta ? pathRespuesta : "") << endl;
    cout << "(4) Procesar." << endl;
}

void insertarValor(char*& variable, string texto_variable) {
    cout << "Introduzca " << texto_variable << ": ";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    string temp;
    getline(cin, temp);
    
    variable = new char[temp.length() + 1];
    strcpy(variable, temp.c_str());
}

int main() {
    char *extension = nullptr;
    char *pathProcesar = nullptr;
    char *pathRespuesta = nullptr;
    char opcion;

    do {
        mostrarInterfaz(extension, pathProcesar, pathRespuesta);
        cout << "Opción: ";
        cin >> opcion;
        system("clear");
        cout << "--------------------------------------------------------------";
        cout << "\nOpción elegida: " << opcion << endl;
        switch (opcion) {
            case '0':
                break;
            case '1':
                insertarValor(extension, "la extensión");
                break;
            case '2':
                if (!existeDirectorio(pathProcesar)) {
                    cout << "\033[31mError: La carpeta a procesar no existe o no ha sido asignada.\033[0m" << endl;
                } else {
                    cout << "\033[32mLa carpeta a procesar si existe.\033[0m" << endl;
                }
                insertarValor(pathProcesar, "el path de la carpeta a procesar");
                break;
            case '3':
                if (!existeDirectorio(pathRespuesta)) {
                    cout << "\033[31mError: La carpeta de respuesta no existe o no ha sido asignada.\033[0m" << endl;
                } else {
                    cout << "\033[32mLa carpeta de respuesta si existe.\033[0m" << endl;
                }
                insertarValor(pathRespuesta, "el path de la carpeta a procesar");
                break;
            case '4':
                if (validarDatos(extension, pathProcesar, pathRespuesta)) {
                    procesarDatos(extension, pathProcesar, pathRespuesta);
                }
                break;
            default:
                cout << "Opción inválida, intente de nuevo." << endl;
                break;
        }
        cout << "--------------------------------------------------------------" << endl;
    } while (opcion != '0');

    delete[] extension;
    delete[] pathProcesar;
    delete[] pathRespuesta;

    return 0;
}