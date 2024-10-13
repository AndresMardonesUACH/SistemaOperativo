# Programa Conteo de Palabras con Threads

## Descripción
Este proyecto ofrece un programa que es capaz de contar palabras de libros de forma paralela usando una cantidad de threads indicada por el usuario.

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
./conteoPalabrasThreads -e "[arg1]" -i "[arg2]" -o "[arg3]" -s "[arg4]" -m "[arg5]" 
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| e | Extensión de los libros a procesar |
| i | Ruta de carpeta de entrada donde se encuentran los libros a procesar |
| o | Ruta de carpeta de salida donde se generarán los archivos de salida |
| s | Ruta donde se encuentra el archivo ``stop_words.txt``|
| m | Ruta donde se generará el archivo ``mapa_archivos.txt`` |

### Ejemplo de ejecución
```bash
Archivo mapa_archivos.txt creado correctamente
```
Si el programa se ejecuta correctamente, entrega este mensaje y además se genera el archivo ``mapa_archivos.txt``:
```bash
libro1;id1
libro2;id2
libro3;id3
```
además de los archivos de salida ``[id].[extensión]``:
```bash
palabra1;cantidad1
palabra2;cantidad2
palabra3;cantidad3
```

    