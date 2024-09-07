#include <iostream>
#include <cstring>
using namespace std;

/**
 * Función que calcula promedio y sumatoria de un vector
 * @param vec Vector ingresado a través de los args del programa
 * @return Mensaje que incluye los resultados
 */
void operaVector(char* vec){
    double sumatoria = 0;
    double cont = 0;
    string aux= "";
    for(int i = 0; i < strlen(vec); i++){
        if(isdigit(vec[i])){
            aux.push_back(vec[i]);
            if(i == (strlen(vec) - 1)){
                sumatoria += stoi(aux);
                cont ++;
            }
        }
        else if (vec[i] == ','  && aux != "" && i != (strlen(vec) - 1)){
            sumatoria += stoi(aux);
            cont ++;
            aux = "";
        }
        else{
            cout << "El vector no ha sido ingresado correctamente" << endl;
            return;
        }
    }
    cout << "El vector ingresado: '" << vec << "' tiene PROMEDIO: " << (double) (sumatoria / cont) << ", SUMATORIA: " << sumatoria << endl;

}

/**
 * Función que calcula el valor de f(x)=5x*x + (1/x) 
 * @param numero El numero que se evaluará en la función
 * @return Mensaje que incluye el resultado
 */
void calculaFuncion(char* numero){
    double x = atof(numero);
    if (x != 0) cout << "El resultado de la función evaluada en: " << numero << " es: " << (5*x*x + (1/x)) << endl;
    else{
        cout << "No es posible calcular la función evaluada en el valor ingresado" << endl;
    }
}