#ifndef FUNCIONESLOGIN_H
#define FUNCIONESLOGIN_H
#include <cstring>
#include <iostream>
#include <vector>
#include "dotenv.h"
using namespace std;

/**
 * Función que valida si el usuario cumple con las condiciones
 * @param user Usuario a validar
 * @return True si es válido, False si no
 */
bool validaUsuario(string user){
    if(user.size() < 3){
        cout << "ERROR. El nombre de usuario debe tener 3 caracteres" << endl;
        return false;
    }
    int i = 0;
    while(i < user.size() && isalpha(user[i])) i++;
    if(i < user.size()){
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
bool validaContrasena(string contrasena){
    if(contrasena.size() < 6){
        cout << "ERROR. La contraseña debe tener 6 caracteres" << endl;
        return false;  
    } 
    int i = 0;
    while(i < contrasena.size() && isalnum(contrasena[i])) i++;
    if(i < contrasena.size()){
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
int validaLogin(char* usuario, char* contrasena){
    dotenv::init();
    if(validaUsuario(string(usuario)) && validaContrasena(string(contrasena))){
        ifstream archivo(dotenv::getenv("pathUsuarios"));
        string texto;
        vector <string> credentials(3);
        string aux = "";
        bool userEncontrado = false; 
        if(archivo.is_open()){
            while(getline(archivo, texto) && !userEncontrado){
                credentials.clear();
                aux = "";
                for(int i = 0; i < texto.size(); i++){
                    if(texto[i] != ';'){
                        aux.push_back(texto[i]);
                    }
                    else{
                        credentials.push_back(aux);
                        aux = "";
                    }
                }
                credentials.push_back(aux);
                if(usuario == credentials[0]) userEncontrado = true;
            }
            if(userEncontrado){
                if(contrasena == credentials[1]){
                    return stoi(credentials[2]);
                }
                else{
                    cout << "ERROR. Usuario o contraseña incorrectos" << endl;
                }
            }
            else{
                cout << "ERROR. Usuario o contraseña incorrectos" << endl;
            }
            archivo.close();
        }
        cout << "Falta el archivo: " << dotenv::getenv("pathUsuarios") << endl;
        return -1;
    }
    return -1;
}

#endif