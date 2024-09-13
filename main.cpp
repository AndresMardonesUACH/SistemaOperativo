#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include "funcionesTexto.h"
#include "funcionesMat.h"
#include "funcionesLogin.h"
#include "funcionesAdmin.h"
using namespace std;

/**
 * Imprime las opciones de menu
 */
void generaInterfaz(int rol){
    cout << "\nElija una Opción: " << endl;
    cout << "0. Salir " << endl;
    cout << "1. Detección de Palíndromos" << endl;
    cout << "2. Conteo de vocales" << endl;
    cout << "3. Conteo de letras" << endl;
    cout << "4. Promedio y sumatoria de un vector" << endl;
    cout << "5. Cálculo de f(x)" << endl;
    cout << "6. Conteo de palabras" << endl;
    if(rol == 0){
        cout << "7. Ingresar Usuarios" << endl;
        cout << "8. Listar Usuarios" << endl;
        cout << "9. Eliminar Usuarios " << endl;
    }
}


int main(int argc, char* argv[]){
    // Declaración de variables que albergarán los argumentos del programa
    int opt;
    char* usuario = nullptr;
    char* contrasena = nullptr;
    char* texto = nullptr;
    char* vector = nullptr;
    char* numero = nullptr;
    
    // Se obtienen los argumentos del programa
    while ((opt = getopt(argc, argv, "u:p:t:v:n:")) != -1){
        switch (opt){
        case 'u':
            usuario = optarg;
            break;
        case 'p':
            contrasena = optarg;
            break;
        case 't':
            texto = optarg;
            break;
        case 'v':
            vector = optarg;
            break;
        case 'n':
            numero = optarg;
            break;
        default:
            break;
        }
    }

    // Se valida que todos los argumentos hayan sido ingresados
    if(!usuario){
        cout << "ERROR. Debe ingresar usuario -u" << endl;
        exit(EXIT_FAILURE);
    }
    if(!contrasena){
        cout << "ERROR. Debe ingresar contraseña -p" << endl;
        exit(EXIT_FAILURE);
    }
    if(!texto){
        cout << "ERROR. Debe ingresar el texto -t" << endl;
        exit(EXIT_FAILURE);
    }
    if(!vector){
        cout << "ERROR. Debe ingresar el vector -v" << endl;
        exit(EXIT_FAILURE);
    }
    if(!numero){
        cout << "ERROR. Debe ingresar el número -n" << endl;
        exit(EXIT_FAILURE);
    }

    int rol = validaLogin(usuario, contrasena);
    if (rol == -1){
        exit(EXIT_FAILURE);
    }

    // Se genera el menú y se pide el input de una opción
    // Dependiendo de la opción ingresada, se ejecuta la función correspondiente
    char opcion;
    system("clear");
    cout << "SISTEMA MENÚ (PID = " << getpid() << ")" << endl;
    cout << "Nombre de Usuario: " << usuario << endl;
    if(rol == 0) cout << "Rol: Admin" << endl;
    else cout << "Rol: Genérico" << endl;
    do{

        generaInterfaz(rol);
        
        cout << "Opción: ";
        cin >> opcion;

        pid_t pid;
        if(isdigit(opcion)){
            system("clear");
            cout << "SISTEMA MENÚ (PID = " << getpid() << ")" << endl;
            cout << "Nombre de Usuario: " << usuario << endl;
            if(rol == 0) cout << "Rol: Admin" << endl;
            else cout << "Rol: Genérico" << endl;

            cout << "--------------------------------------------------------------";
            cout << "\nOpción elegida: " << opcion << endl;
            switch (opcion){
            case '1':
                esPalindromo(texto);
                break;
            case '2':
                cuentaVocales(texto);
                break;
            case '3':
                cuentaLetras(texto);
                break;
            case '4':
                operaVector(vector);
                break;
            case '5':
                calculaFuncion(numero);
                break;
            case '6':
                pid = fork();

                if (pid == -1) {
                    cerr << "Error al crear el proceso hijo" << endl;
                    return 1;
                } else if (pid == 0) {
                    execl("./conteoPalabras/conteoPalabras", "conteoPalabras", NULL);
                    cerr << "Error al ejecutar conteoPalabras" << endl;
                    return 1;
                } else {
                    wait(NULL);
                    cout << "Ha salido del programa conteoPalabras exitosamente" << endl;
                }
                
                break;
            case '7':
                if(rol == 0) ingresarUsuario();
                else cout << "Opción Invalida, intente de nuevo" << endl;
                break;
            case '8':
                if(rol == 0) listarUsuarios();
                else cout << "Opción Invalida, intente de nuevo" << endl;
                break;
            case '9':
                if(rol == 0) eliminarUsuario();
                else cout << "Opción Invalida, intente de nuevo" << endl;
                break;
            case '0':
                cout << "Ha salido del programa exitosamente" << endl;
                break;
            default:
                cout << "Opción Invalida, intente de nuevo" << endl;
                break;
            }
        }
        else{
            system("clear");
            cout << "--------------------------------------------------------------" << endl;
            cout << "Debe ingresar un número, intente de nuevo" << endl;
        }
        cout << "--------------------------------------------------------------" << endl;
    } while (opcion != '0');


    exit(EXIT_SUCCESS);
}