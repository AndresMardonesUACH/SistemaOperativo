# Menú Sistema Operativo

## Descripción
Este proyecto consta de un sistema que presenta un menú de opciones que además maneja perfiles de usuario (con roles Admin y Genérico). Las opciones que presenta ejecutan funciones cuyos argumentos son ingresados por el usuario mediante los parámetros del programa.

## Requisitos
* Sistema Operativo: Ubuntu 22.04
* Compilador: g++ 11.4.0
* Python 3.10 o superior
* Pandas y Matplotlib

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

Además se requieren de un grupo de variables de entorno. Estas deben estar indicadas en el archivo ``.env`` en la carpeta raíz, de la siguiente forma:
```bash
pathUsuarios = "[ruta]"
pathConteoPalabras = "[ruta]"
pathConteoPalabrasThreads = "[ruta]"
extension = "[extensión]"
pathEntrada = "[ruta]"
pathSalida = "[ruta]"
cantidad_thread = "[número]"
mapa_archivos = "[ruta]"
stop_word = "[ruta]"
inverted_index = "[ruta]"
pathInvertedIndex = "[ruta]"
array_threads = "[números separados por ,]"
repeticiones = "[número]"
datos = "[ruta]"
grafico = "[ruta]"
ejecutador = "[ruta]"
analizador = "[ruta]"
```
Donde estas variables representan:

| Variable de entorno | Descripción |
| -------- | ----------- |
| pathUsuarios | Ruta donde se encuentra el archivo ``usuarios.txt`` con los usuarios del programa |
| pathConteoPalabras | Ruta donde se encuentra el programa ``conteoPalabras``  |
| pathConteoPalabrasThreads | Ruta donde se encuentra el programa ``conteoPalabrasThreads`` |
| extension | Extensión con la cual se ejecutará el programa ``conteoPalabrasThreads`` |
| pathEntrada | Ruta donde se encuentran los libros de entrada para ``conteoPalabrasThreads``  |
| pathSalida | Ruta donde se generarán los archivos de salida para ``conteoPalabrasThreads`` |
| cantidad_thread | Cantidad de threads con los que se ejecutará ``conteoPalabrasThreads`` |
| mapa_archivos | Ruta donde se generará el archivo ``mapa_archivos.txt`` |
| stop_word | Ruta donde se encuentra el archivo ``stop_words.txt`` |
| inverted_index | Ruta donde se generará el archivo ``inverted_index.INDEX`` |
| pathInvertedIndex | Ruta donde se encuentra el programa ``invertedIndex`` |
| array_threads | Números separados por coma que representan los threads que serán analizados en ``Análisis de Performance``|
| repeticiones | Número que indica la cantidad de repeticiones por cada thread en ``Análisis de Performance`` |
| datos | Ruta donde se encuentra el archivo ``datos.txt`` donde se guardarán las mediciones |
| grafico | Ruta donde se encuentra el png ``grafico.png`` con el gráfico resultante |
| ejecutador | Ruta donde se encuentra el programa ``ejecutador`` |
| analizador | Ruta donde se encuentra el programa ``analizador`` (python que genera el gráfico) |

### Ejemplo de ejecución
```bash
./main -u "user" -p "pass12" -t "ojo" -v "3,1,4,1" -n "2.4"
```
Luego se desplegará el siguiente menú (si usuario y contraseña son correctos):
```bash
Bienvenido user, sesión iniciada correctamente

Elija una Opción:
0. Salir
1. Detección de Palíndromos
2. Conteo de vocales
3. Conteo de letras
4. Promedio y sumatoria de un vector
5. Cálculo de f(x)
6. Conteo de palabras
Opción:
```
Si el usuario que ha ingresado es administrador, se ofrecen adicionalmente estas 5 opciones: 
```bash
7. Ingresar Usuarios
8. Listar Usuarios
9. Eliminar Usuarios
10. Conteo Paralelo con Threads
11. Crear Índice Invertido
12. Análisis de Performance
```
Cuando ingrese una opción válida, se ejecutará el procedimiento según corresponda.

## Funciones
Las funciones que presenta el programa son:

0. Salir del programa
1. Detección de palíndromo (argumento utilizado: ``-t``)
2. Conteo de vocales (argumento utilizado: ``-t``)
3. Conteo de letras (argumento utilizado: ``-t``)
4. Suma y promedio de vector (argumento utilizado: ``-v``)
5. Cálculo de la función: ``f(x) = 5x*x + (1/x)`` (argumento utilizado: ``-n``)
6. Se ejecuta el programa ``conteoPalabras`` (para más información consultar su propio readme.md)
7. Ingresar usuario. En esta opción se pide que se ingrese el nombre del nuevo usuario, la contraseña y su rol (0: admin, 1: genérico).
8. Listar usuarios. En esta opción se listan todos los usuarios que se encuentran ingresados en la base de datos del programa, indicando nombre y rol.
9. Eliminar usuarios. En esta opción se pide que se ingrese el nombre del usuario a eliminar (no es posible eliminar administradores).
10. Conteo Paralelo con Threads. En esta opción se ejecuta el programa ``conteoPalabrasThreads`` (para más información consultar su propio readme.md)
11. Crear Inverted Index. Se ejecuta el programa ``invertedIndex`` (para más información consultar su propio readme.md)
12. Análisis de Performance. Se ejecuta el programa ``ejecutador`` (para más información consultar su propio readme.md)

## Autor y Créditos

* Andrés Mardones
* Martín Maza
* laserpants (Librería dotenv: https://github.com/laserpants/dotenv-cpp)
    