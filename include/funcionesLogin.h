#include <iostream>
#include <cstring>
#include "dotenv.h"
using namespace std;

/**
 * Función que valida si el usuario cumple con las condiciones
 * @param user Usuario a validar
 * @return True si es válido, False si no
 */
bool validaUsuario(char* user){
    if(strlen(user) < 3){
        cout << "ERROR. El nombre de usuario debe tener 3 caracteres" << endl;
        return false;
    }
    int i = 0;
    while(i < strlen(user) && isalpha(user[i])) i++;
    if(i < strlen(user)){
        cout << "ERROR. El nombre de usuario solo debe tener letras" << endl;
        return false;
    }
    return true;
}

/**
 * Función que valida si la contraseña cumple con las condiciones
 * @param contrasena Contraseña a validar
 * @return True si es válido, False si no
 */
bool validaContrasena(char* contrasena){
    if(strlen(contrasena) < 6){
        cout << "ERROR. La contraseña debe tener 6 caracteres" << endl;
        return false;  
    } 
    int i = 0;
    while(i < strlen(contrasena) && isalnum(contrasena[i])) i++;
    if(i < strlen(contrasena)){
        cout << "ERROR. La contraseña solo debe tener letras y números" << endl;
        return false;
    }
    return true;
}

/**
 * Función que valida si usuario y contraseña se encuentran
 * en el archivo .env
 * Además los valida con las funciones: validaUsuario y validaContrasena
 * @param user Usuario a validar
 * @param contrasena Contraseña a validar
 * @return True si es válido, False si no
 */
bool validaLogin(char* usuario, char* contrasena){
    // se utiliza la librería dotenv para buscar usuario y contraseña en .env
    dotenv::init();
    if(validaUsuario(usuario) && validaContrasena(contrasena)){
        if(dotenv::getenv(usuario) == contrasena){
            return true;
        } 
        else{
            cout << "ERROR. Usuario o contraseña incorrectos" << endl;
            return false;
        }
    }
    return false;
}