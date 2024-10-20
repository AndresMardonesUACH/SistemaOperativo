#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include "dotenv.h"
#include "funcionesTexto.h"
#include "funcionesMat.h"
#include "funcionesLogin.h"
#include "funcionesAdmin.h"
using namespace std;

/**
 * Imprime las opciones de menu según el rol del usuario
 * @param rol: 0 para admin, otro valor para usuario genérico
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
    // Opciones adicionales solo visibles para el admin
    if(rol == 0){
        cout << "7. Ingresar Usuarios" << endl;
        cout << "8. Listar Usuarios" << endl;
        cout << "9. Eliminar Usuarios " << endl;
        cout << "10. Conteo Paralelo con Threads " << endl;
        cout << "11. Crear Índice Invertido" << endl;
    }
}

/**
 * Verifica si una cadena representa un número
 * @param cadena: cadena a verificar
 * @return true si es un número, false en caso contrario
 */
bool esNumero(string cadena){
    for(int i = 0; i < cadena.length(); i++){
        if(!isdigit(cadena[i])) return false; // Retorna false si encuentra un carácter no numérico
    }
    return true; // Retorna true si todos los caracteres son dígitos
}

/**
 * Ejecuta el conteo de palabras en paralelo utilizando hilos
 */
void conteoPalabrasThreads(){
    dotenv::init(); // Inicializa dotenv para cargar variables de entorno
    string comando = dotenv::getenv("pathConteoPalabrasThreads"); // Obtiene el path del programa
    // Agrega argumentos al comando
    comando += " -e" + dotenv::getenv("extension");
    comando += " -i" + dotenv::getenv("pathEntrada");
    comando += " -o" + dotenv::getenv("pathSalida");
    comando += " -s" + dotenv::getenv("stop_word");
    comando += " -m" + dotenv::getenv("mapa_archivos");
    comando += " -t" + dotenv::getenv("cantidad_thread");
    system(comando.c_str()); // Ejecuta el comando en el sistema
}

/**
 * Ejecuta el proceso de creación de índice invertido
 */
void invertedIndex(){
    dotenv::init(); // Inicializa dotenv
    string comando = dotenv::getenv("pathInvertedIndex"); // Obtiene el path del programa
    // Agrega argumentos al comando
    comando += " -e" + dotenv::getenv("extension");
    comando += " -i" + dotenv::getenv("pathEntrada");
    comando += " -o" + dotenv::getenv("pathSalida");
    comando += " -m" + dotenv::getenv("mapa_archivos");
    comando += " -x" + dotenv::getenv("inverted_index");
    system(comando.c_str()); // Ejecuta el comando en el sistema
}

/**
 * Comunica el término de una operación mediante un archivo
 * @param termino: true si la operación terminó, false en caso contrario
 * @param salida: ruta del directorio de salida
 */
void comunicarTermino(bool termino, string salida){
    string nuevaRuta = salida + "/cPTEjecutado.txt"; // Ruta del archivo
    ofstream archivoTermino(nuevaRuta.c_str()); // Abre el archivo para escribir
    if(termino) archivoTermino << "true";
    else archivoTermino << "false"; 
    archivoTermino.close(); // Cierra el archivo
}

int main(int argc, char* argv[]){
    // Declaración de variables que albergarán los argumentos del programa
    int opt;
    char* usuario = nullptr; // Almacena el usuario
    char* contrasena = nullptr; // Almacena la contraseña
    char* texto = nullptr; // Almacena el texto
    char* vector = nullptr; // Almacena el vector
    char* numero = nullptr; // Almacena un número
    
    // Se obtienen los argumentos del programa
    while ((opt = getopt(argc, argv, "u:p:t:v:n:")) != -1){
        switch (opt){
        case 'u':
            usuario = optarg; // Argumento de usuario
            break;
        case 'p':
            contrasena = optarg; // Argumento de contraseña
            break;
        case 't':
            texto = optarg; // Argumento de texto
            break;
        case 'v':
            vector = optarg; // Argumento de vector
            break;
        case 'n':
            numero = optarg; // Argumento de número
            break;
        default:
            break; // Ignorar opciones no reconocidas
        }
    }

    // Validación de los argumentos
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

    int rol = validaLogin(usuario, contrasena); // Valida el login y obtiene el rol del usuario
    if (rol == -1){
        exit(EXIT_FAILURE); // Termina el programa si el login falla
    }

    // Generación del menú y gestión de la interacción del usuario
    string opcion; // Variable para almacenar la opción elegida
    system("clear"); // Limpia la pantalla
    cout << "SISTEMA MENÚ (PID = " << getpid() << ")" << endl; // Muestra el PID
    cout << "Nombre de Usuario: " << usuario << endl; // Muestra el usuario
    if(rol == 0) cout << "Rol: Admin" << endl; // Indica si el rol es admin
    else cout << "Rol: Genérico" << endl; // Indica si el rol es genérico

    do{
        generaInterfaz(rol); // Muestra las opciones del menú
        cout << "Opción: ";
        cin >> opcion; // Lee la opción ingresada

        // Si la opción es un número, se ejecuta la opción correspondiente
        if(esNumero(opcion)){
            system("clear"); // Limpia la pantalla
            cout << "SISTEMA MENÚ (PID = " << getpid() << ")" << endl;
            cout << "Nombre de Usuario: " << usuario << endl;
            if(rol == 0) cout << "Rol: Admin" << endl;
            else cout << "Rol: Genérico" << endl;

            cout << "--------------------------------------------------------------";
            cout << "\nOpción elegida: " << opcion << endl;

            // Ejecuta la función correspondiente según la opción seleccionada
            switch ((stoi(opcion))){
            case 0:
                cout << "Ha salido del programa exitosamente" << endl;
                break;
            case 1:
                esPalindromo(texto); // Verifica si el texto es un palíndromo
                break;
            case 2:
                cuentaVocales(texto); // Cuenta las vocales en el texto
                break;
            case 3:
                cuentaLetras(texto); // Cuenta las letras en el texto
                break;
            case 4:
                operaVector(vector); // Opera sobre el vector
                break;
            case 5:
                calculaFuncion(numero); // Calcula la función f(x)
                break;
            case 6:
                dotenv::init();
                system(dotenv::getenv("pathConteoPalabras").c_str()); // Ejecuta el conteo de palabras
                break;
            case 7:
                if(rol == 0) ingresarUsuario(); // Permite ingresar un nuevo usuario si es admin
                else cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            case 8:
                if(rol == 0) listarUsuarios(); // Lista usuarios si es admin
                else cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            case 9:
                if(rol == 0) eliminarUsuario(); // Permite eliminar un usuario si es admin
                else cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            case 10:
                if(rol == 0) conteoPalabrasThreads(); // Ejecuta conteo paralelo si es admin
                else cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            case 11:
                if(rol == 0) invertedIndex(); // Crea índice invertido si es admin
                else cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            case 12:
                if (rol == 0) analisisPerformance();
                else cout << "Opción Invalida, intente de nuevo" << endl;
            default:
                cout << "Opción Invalida, intente de nuevo" << endl; // Mensaje de error
                break;
            }
        } else {
            system("clear"); // Limpia la pantalla
            cout << "--------------------------------------------------------------";
            cout << "La opción no es un número, intente de nuevo." << endl; // Mensaje de error
        }
        cout << "--------------------------------------------------------------" << endl;
    } while (!esNumero(opcion) || stoi(opcion) != 0); // Continua hasta que el usuario elija salir

    dotenv::init();
    comunicarTermino(false, dotenv::getenv("pathSalida"));

    exit(EXIT_SUCCESS);
}