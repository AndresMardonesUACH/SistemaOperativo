# Menú Sistema Operativo

## Descripción
Este proyecto consta de un sistema que presenta un menú de opciones que además maneja perfiles de usuario. Las opciones que presenta ejecutan funciones cuyos argumentos son ingresados por el usuario mediante los parámetros del programa.

## Requisitos
* Sistema Operativo: Ubuntu 22.04
* Compilador: g++ 11.4.0

## Instalación / Compilación
Para la compilación del programa basta con situarse en la carpeta raíz del proyecto y ejecutar en la terminal:
```bash
make
```
Una vez hecho esto, el programa está compilado y listo para ejecutarse.

## Ejecución
Para ejecutar el programa correctamente:
```bash
./main -u "[arg1]" -p "[arg2]" -t "[arg3]" -v "[arg4]" -n "[arg5]" 
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| u | El usuario |
| p | La contraseña del usuario |
| t | El texto |
| v | El vector de números enteros separados por ``,`` |
| n | El número |

### Ejemplo de ejecución
```bash
./main -u "user" -p "pass12" -t "ojo" -v "3,1,4,1" -n "2.4"
```
Luego se desplegará el siguiente menú (si usuario y contraseña son correctos):
```bash
Bienvenido user, sesión iniciada correctamente

Elija una Opción:
1. Detección de Palíndromos
2. Conteo de vocales
3. Conteo de letras
4. Promedio y sumatoria de un vector
5. Cálculo de f(x)
6. Salir
Opción:
```
Cuando ingrese una opción válida, se ejecutará el procedimiento según corresponda.

## Funciones
Las funciones que presenta el programa son:

1. Detección de palíndromo (argumento utilizado: ``-t``)
2. Conteo de vocales (argumento utilizado: ``-t``)
3. Conteo de letras (argumento utilizado: ``-t``)
4. Suma y promedio de vector (argumento utilizado: ``-v``)
5. Cálculo de la función: ``f(x) = 5x*x + (1/x)`` (argumento utilizado: ``-n``)
6. Salir del programa

## Autor y Créditos

* Andrés Mardones
* laserpants (Librería dotenv: https://github.com/laserpants/dotenv-cpp)
