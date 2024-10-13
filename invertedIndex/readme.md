# Programa Inverted Index

## Descripción
Este proyecto ofrece un programa que es capaz de transformar los archivos de salida obtenidos del programa ``conteoPalabrasThreads`` en un único archivo ``inverted_index.INDEX`` donde es posible visualizar de mejor forma la información resultante. 

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
Antes de ejecutar este programa es necesario completar con éxito ``conteoPalabrasThreads``

Para ejecutar el programa correctamente:
```bash
./invertedIndex -e "[arg1]" -i "[arg2]" -o "[arg3]" -m "[arg4]" -x "[arg5]" 
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| e | Extensión de los libros a procesar |
| i | Ruta de carpeta de entrada donde se encuentran los libros a procesar |
| o | Ruta de carpeta de salida donde se generarán los archivos de salida |
| m | Ruta donde se generará el archivo ``mapa_archivos.txt`` |
| x | Ruta donde se generará el archivo ``inverted_index.INDEX``|

### Ejemplo de ejecución
```bash
Archivo inverted_index.INDEX creado correctamente
```
Si el programa se ejecuta correctamente, entrega este mensaje y además se genera el archivo ``inverted_index.INDEX``:
```bash
palabra1;(ID1,Cant1);(ID2,Cant2);
palabra2;(ID3,Cant3);(ID4,Cant4);(ID5,Cant5);
```
    