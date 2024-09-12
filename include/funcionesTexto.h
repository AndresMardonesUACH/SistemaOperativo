#include <iostream>
#include <cstring>
using namespace std;

/**
 * Función que valida si un texto es palindromo o no
 * @param texto Texto a validar
 * @return Mensaje que incluye el resultado
 */
void esPalindromo(char* texto){
    string cadena(texto);
    int i = 0, j = cadena.length() - 1;
    while (j > i && (char) toupper(cadena[i]) == (char) toupper(cadena[j])){
        i++;
        j--;
    }
    if(i >= j){
        cout << "El texto ingresado: " << texto << " ES PALÍNDROMO" << endl;
        return;
    }
    cout << "El texto ingresado: '"<< texto << "' NO ES PALÍNDROMO" << endl;
}

/**
 * Función que cuenta la cantidad de vocales de un texto
 * @param texto Texto a validar
 * @return Mensaje que incluye el resultado
 */
void cuentaVocales(char* texto){
    string cadena(texto);
    string vocales = "aeiouAEIOU";
    int cont = 0;
    for(int i = 0; i < cadena.length(); i++){
        if(vocales.find(cadena[i]) != -1) cont++;
    }
    cout << "El texto ingresado: '" << texto << "' posee: " << cont << " vocales" << endl;
}

/**
 * Función que cuenta las letras de un texto
 * @param texto Texto a validar
 * @return Mensaje que incluye el resultado
 */
void cuentaLetras(char* texto){
    int cont = 0;
    for(int i = 0; i < strlen(texto); i++){
        if(isalpha(texto[i])) cont++;
    }
    cout << "El texto ingresado: '" << texto << "' posee: " << cont << " letras" << endl;
}