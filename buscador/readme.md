# Programa Buscador

## Descripción
Este proyecto ofrece un programa que es capaz de buscar cadenas de texto en los libros de entrada del programa. Algunas respuestas son guardadas en una caché, en el caso de no ser encontrada ahí, se procede a buscar mediante el motor de búsqueda. Toda la comunicación entre procesos ocurre mediante socket.

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
Para ejecutar el programa correctamente: (Motor de búsqueda se debe ejecutar antes que caché)
```bash
./buscador -p "[arg1]" -i "[arg2]" -m "[arg3]" -x "[arg4]"
```
```bash
./motorDeBusqueda -p "[arg1]" -t "[arg2]" -x "[arg4]"
```
```bash
./cache -b "[arg1]" -c "[arg2]" -i "[arg3]" -t "[arg4]"
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| c | Puerto de la caché |
| p | Puerto de la caché |
| i | Ip del servidor |
| m | Ruta donde se encuentra el archivo ``mapa_archivos.txt`` |
| x | Ruta donde se encuentra el archivo ``inverted_index.INDEX`` |
| b | Puerto del motor de búsqueda |
| t (motorDeBusqueda) | TopK, número máximo de resultados |
| t (cache) | Memory_Size, tamaño máximo de memoria caché |



### Ejemplo de ejecución
```bash
Ingrese búsqueda (o escriba 'salir ahora' para salir): hola mundo
```
Si el programa se ejecuta correctamente, entrega este mensaje:
```bash
--------------------------------------------------------------
Buscando 'hola mundo'...
1) Puntaje: 250, C. S. Lewis - Las Crónicas de Narnia
2) Puntaje: 248, George Norman Lippert - James Potter 2 - La Maldición del Guardián
3) Puntaje: 203, George Norman Lippert - James Potter 1 - La Encrucijada de los Mayores
4) Puntaje: 187, Dan Brown - El Símbolo Perdido
5) Puntaje: 142, George R. R. Martin - Canción de Hielo y Fuego 1 - Juego de Tronos
6) Puntaje: 138, Gabriel García Márquez - Vivir para contarla
7) Puntaje: 137, George R. R. Martin - Canción de Hielo y Fuego 2 - Choque de Reyes
8) Puntaje: 127, J.K. Rowling - Harry Potter 4 - El Cáliz de Fuego
9) Puntaje: 124, Dan Brown - La Conspiración
10) Puntaje: 103, Christopher Paolini - El Legado 2 - Eldest
--------------------------------------------------------------
```

    