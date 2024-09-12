#include <iostream>
#include <fstream>
#include "dotenv.h"
#include <cstring>
#include "funcionesLogin.h"
using namespace std;

bool validaRol(string role){
    if(role != "0" && role != "1"){
        cout << "Rol ingresado no es válido" << endl;
        return false;
    }
    return true;
}

bool buscaUsuario(string usuario){
    dotenv::init();
    ifstream archivo(dotenv::getenv("pathUsuarios"));
    string texto;
    string aux = "";
    bool userEncontrado = false; 
    if(archivo.is_open()){
        while(getline(archivo, texto) && !userEncontrado){
            aux = "";
            int i = 0;
            while(texto[i] != ';'){
                aux.push_back(texto[i]);
                i++;
            }
            if(usuario == aux) userEncontrado = true;
        }
        if(userEncontrado){
            cout << "Usuario ya se encuentra ingresado" << endl;
            return true;
        }
        else return false;
    }
    archivo.close();
    return false;
}

bool reintentar(){
    string opcion;
    do{
        cout << "¿Desea intentar de nuevo? (s/n): ";
        cin >> opcion;
        cout << "--------------------------------------------------------------" << endl;
        switch (opcion[0]){
        case 'n':
            return false;
            break;
        case 's':
            return true;
            break;
        default:
            cout << "Opción Invalida" << endl;
            system("clear");
            cout << "--------------------------------------------------------------" << endl;
            break;
        }
    }while(opcion[0] != 's' && opcion[0] != 'n');
    return false;
}

void volver(){
    system("clear");
    cout << "--------------------------------------------------------------" << endl;
    cout << "Ha vuelto al menú correctamente" << endl;
}

void ingresarUsuario(){
    ofstream archivo(dotenv::getenv("pathUsuarios"), ios::app);
    string newUser = "";
    string newPass = "";
    string newRole = "";

    bool seguir = true;
    bool valido = false;
    do{
        cout << "Ingrese Nombre de usuario: ";
        cin >> newUser;
        if(!validaUsuario(newUser) || buscaUsuario(newUser)) seguir = reintentar();
        else{
            seguir = false;
            valido = true;
        };
    }while(seguir);
    if(!valido){
        volver();
        return;
    }
    seguir = true;
    valido = false;

    do{
        cout << "Ingrese Contraseña: ";
        cin >> newPass;
        if(!validaContrasena(newPass)) seguir = reintentar();
        else{
            seguir = false;
            valido = true;
        }
    }while(seguir);
    if(!valido){
        volver();
        return;
    }
    seguir = true;
    valido = false;

    do{
        cout << "Ingrese Rol: ";
        cin >> newRole;
        if(!validaRol(newRole)) seguir = reintentar();
        else{
            seguir = false;
            valido = true;
        }
    }while(seguir);
    if(!valido){
        volver();
        return;
    }
    
    if(archivo.is_open()){
        archivo << "\n" << newUser << ";" << newPass << ";" << newRole;
        system("clear");
        cout << "--------------------------------------------------------------" << endl;
        if(newRole == "0") cout << "Nuevo Usuario " << newUser << ", Admin ingresado correctamente" << endl;
        else cout << "Nuevo Usuario " << newUser << ", Genérico ingresado correctamente" << endl;
    }
    archivo.close();
}

void listarUsuarios(){
    dotenv::init();
    ifstream archivo(dotenv::getenv("pathUsuarios"));
    string texto;
    system("clear");
    cout << "--------------------------------------------------------------" << endl;
    cout << "\tLista de Usuarios" << endl;
    if(archivo.is_open()){
        while(getline(archivo, texto)){
            string aux = "";
            int i = 0;
            while(texto[i] != ';'){
                aux.push_back(texto[i]);
                i++;
            }
            if(texto[texto.size()-1] == '0') cout << "\e[2mUsuario: \e[0m" << aux << "\e[2m Rol: \e[0mAdmin\e[0m" << endl;
            else  cout << "\e[2mUsuario: \e[0m" << aux << "\e[2m Rol: \e[0mGenérico\e[0m" << endl;
        }
    }
}


bool actualizaUsuarios(string usuario){
    dotenv::init();
    ifstream archivo(dotenv::getenv("pathUsuarios"));
    string texto;
    string textoActualizado = "";
    bool userEncontrado = false;

    if(archivo.is_open()){
        int i = 0;
        while(getline(archivo, texto)){
            if(texto.substr(0, usuario.size()) != usuario) {
                if (i == 0){
                    textoActualizado = texto[0] + texto;
                    i++;
                }
                else textoActualizado += ('\n' + texto);
            }
            else{
                userEncontrado = true;
                if(texto[texto.size()-1] == '0'){
                    cout << "ERROR. No es posible eliminar usuarios Admin" << endl;
                    return false;
                }
            }
        }
    }

    archivo.close();
    ofstream archivo1(dotenv::getenv("pathUsuarios"));
    archivo1.clear();
    if(textoActualizado.size() > 1) archivo1 << textoActualizado.substr(1, textoActualizado.size());

    if(userEncontrado) return true;
    cout << "ERROR. El usuario no existe" << endl;
    return false;
}


void eliminarUsuario(){
    string user = "";
    bool seguir = true;
    bool eliminado = false;
    do{
        cout << "Ingrese Nombre de usuario: ";
        cin >> user;        
        system("clear");
        cout << "--------------------------------------------------------------" << endl;
        if(!validaUsuario(user) || !actualizaUsuarios(user)){
            seguir = reintentar();
        }
        else{
            seguir = false;
            eliminado = true;
        }
    }while(seguir);

    if(eliminado){
        cout << "Usuario " << user << " se ha eliminado correctamente" << endl;
    }
    else{
        volver();
    }
}