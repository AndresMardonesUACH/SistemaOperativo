# Programa Conteo de Palabras

## Descripción
Este proyecto ofrece un programa que es capaz de contar palabras de una extensión de archivos específica de una carpeta.

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
./conteoPalabras
```

### Ejemplo de ejecución
```bash
Programa contador de palabras
pid: [Pid]
--------------------------------------------------------------
Seleccione una opción:
(0) Salir.
(1) Extensión de archivos a procesar NO ASIGNADO
(2) Path de carpeta a procesar NO ASIGNADO
(3) Path de carpeta que contendrá la respuesta del proceso NO ASIGNADO
(4) Procesar.
Opción:
```
Al seleccionar una opción válida, debe ingresar la información pedida y al presionar '4', se contarán las palabras de los archivos con la extensión indicada en la opción '1' que se encuentren en la carpeta especificada en la opción '2', para luego generar archivos del tipo: 
```bash
palabra1; cantidad
palabra3; cantidad
palabra2; cantidad
```
en la carpeta indicada en la opción '3'.

    