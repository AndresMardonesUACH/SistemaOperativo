#include <iostream>
#include <unistd.h>
#include <cstring>
#include <limits>
#include "procesar.h"

using namespace std;

pid_t pid = getpid();

void mostrarInterfaz(char* extension, char* pathProcesar, char* pathRespuesta, bool limpiar) {
    const char *noAsignado = "\033[31mNO ASIGNADO\033[0m";
    const char *invalido = "\033[31mINVÁLIDO\033[0m";
    const char *asignado = "\033[32mASIGNADO:\033[0m";
    
    cout << "Programa contador de palabras" << endl;
    cout << "pid: " << pid << endl;
    cout << "--------------------------------------------------------------";
    cout << endl << "Seleccione una opción:" << endl;
    cout << "(0) Salir." << endl;
    cout << "(1) Extensión de archivos a procesar " 
         << (extension ? asignado : noAsignado) << " " << (extension ? extension : "") << endl;
    cout << "(2) Path de carpeta a procesar ";
    if (pathProcesar) {
        if (!existeDirectorio(pathProcesar)) {
            cout << invalido;
        } else {
            cout << asignado;
        }
        cout << " " << pathProcesar;
    } else {
        cout << noAsignado;
    }
    cout << endl;
    cout << "(3) Path de carpeta que contendrá la respuesta del proceso ";
    if (pathRespuesta) {
        if (!existeDirectorio(pathRespuesta)) {
            cout << invalido;
        } else {
            cout << asignado;
        }
        cout << " " << pathRespuesta;
    } else {
        cout << noAsignado;
    }
    cout << endl;
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

void leerSeleccion (char& opcion) {
    cout << "Opción: ";
    cin >> opcion;
    cout << "--------------------------------------------------------------";
    cout << "\nOpción elegida: " << opcion << endl;
}

int ejecutarSeleccion (char opcion, char*& extension, char*& pathProcesar, char*& pathRespuesta) {
    system("clear");
    switch (opcion) {
        case '0':
            break;
        case '1':
            insertarValor(extension, "la extensión");
            break;
        case '2':
            if (!pathProcesar) {
                insertarValor(pathProcesar, "el path de la carpeta a procesar");
            } else if (!existeDirectorio(pathProcesar)) {
                cout << "\033[31mLa carpeta a procesar no existe.\033[0m" << endl;
                insertarValor(pathProcesar, "el path de la carpeta a procesar");
            } else {
                cout << "\033[32mLa carpeta a procesar si existe.\033[0m" << endl;
                cout << "¿Desea cambiar la ruta de la carpeta a procesar?" << endl;
                cout << "0. No" << endl;
                cout << "1. Si" << endl;
                char opcion;
                do {
                    cout << "Opción: ";
                    cin >> opcion;
                    cout << opcion << endl;
                    if(isdigit(opcion)){
                        switch (opcion){
                            case '0':
                                break;
                            case '1':
                                insertarValor(pathProcesar, "el path de la carpeta a procesar");
                                break;
                            default:
                                cout << "Opción Invalida, intente de nuevo" << endl;
                                break;
                        }
                    } else {
                        system("clear");
                        cout << "--------------------------------------------------------------" << endl;
                        cout << "Debe ingresar un número, intente de nuevo" << endl;
                    }
                } while (opcion != '0' && opcion != '1');
            }
            break;
        case '3':
            if (!pathRespuesta) {
                insertarValor(pathRespuesta, "el path de la carpeta a procesar");
            } else if (!existeDirectorio(pathRespuesta)) {
                cout << "\033[31mLa carpeta de respuesta no existe o no ha sido asignada.\033[0m" << endl;
                insertarValor(pathRespuesta, "el path de la carpeta a procesar");
            } else {
                cout << "\033[32mLa carpeta de respuesta si existe.\033[0m" << endl;
                cout << "¿Desea cambiar la ruta de la carpeta de respuesta?" << endl;
                cout << "0. No" << endl;
                cout << "1. Si" << endl;
                char opcion;
                do {
                    cout << "Opción: ";
                    cin >> opcion;
                    switch (opcion){
                        case '0':
                            break;
                        case '1':
                            insertarValor(pathRespuesta, "el path de la carpeta de respuesta");
                            break;
                        default:
                            cout << "Opción Invalida, intente de nuevo" << endl;
                            break;
                    }
                } while (opcion != '0' && opcion != '1');
            }
            break;
        case '4':
            if (validarDatos(extension, pathProcesar, pathRespuesta)) {
                return procesarDatos(extension, pathProcesar, pathRespuesta);
            }
            break;
        default:
            cout << "Opción inválida, intente de nuevo." << endl;
            break;
    }
    cout << "--------------------------------------------------------------" << endl;
    return 1;
}

int main() {
    char *extension = nullptr;
    char *pathProcesar = nullptr;
    char *pathRespuesta = nullptr;
    char opcion;
    int limpiar = 1;

    do {
        mostrarInterfaz(extension, pathProcesar, pathRespuesta, limpiar);
        leerSeleccion(opcion);
        limpiar = ejecutarSeleccion(opcion, extension, pathProcesar, pathRespuesta);
    } while (opcion != '0');

    delete[] extension;
    delete[] pathProcesar;
    delete[] pathRespuesta;

    cout << "Ha vuelto al menú principal" << endl;

    return 0;
    
}