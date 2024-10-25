# Programa Planificador

## Descripción
Este proyecto ofrece un programa que es capaz de realizar una serie de operaciones simulando el funcionamiento de cores, mediante distribuidores.

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
./planificador -o "[arg1]" -n "[arg2]" -r "[arg3]" -c "[arg4]" -d "[arg5]" -s "[arg6]" 
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| o | Ruta de archivo que presenta operaciones |
| n | Número que representa la cantidad de cores a utilizar |
| r | Ruta de archivo donde se guardarán los resultados de las operaciones |
| c | Ruta de programa ``core`` |
| d | Ruta de programa ``distribuidor``|
| s | Ruta de carpeta de archivos de comunicación entre distribuidor y cores |

### Ejemplo de ejecución
```bash
Análisis de Planificador ejecutado correctamente.
Salida de resultados en ./planificador/resultados.txt
Presione tecla ENTER para volver al menú...
```
Si el programa se ejecuta correctamente, entrega este mensaje y además se genera el archivo ``resultados.txt``:
```bash
(idCore:idOperacion;operacion;valor1,valor2) => resultado
```

    