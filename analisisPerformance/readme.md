# Programa Análisis de Performance

## Descripción
Este proyecto ofrece un programa que es capaz de analizar los tiempos que demora el programa conteoPalabrasThreads.

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
./ejecutador -p "[arg1]" -e "[arg2]" -i "[arg3]" -o "[arg4]" -s "[arg5]" -m "[arg6]" -t "[arg7]" -r "[arg8]" -d "[arg9]" -g "[arg10]" -a "[arg11]"
```
(Todos los argumentos deben ser ingresados mediante las comillas dobles: ``"argumento"``)

Para ejecutar el programa python por separado:
```bash
./python3 analizador.py --datos="[ruta]" --grafico="[ruta]"
```

Donde los argumentos representan:

| Argumento | Descripción |
| -------- | ----------- |
| p | Ruta donde se encuentra el programa ``conteoPalabrasThreads`` |
| e | Extensión de los libros a procesar |
| i | Ruta de carpeta de entrada donde se encuentran los libros a procesar |
| o | Ruta de carpeta de salida donde se generarán los archivos de salida |
| s | Ruta donde se encuentra el archivo ``stop_words.txt``|
| m | Ruta donde se generará el archivo ``mapa_archivos.txt`` |
| t | Array de threads con los que se realizarán las mediciones |
| r | Repeticiones con las que se realizarán las mediciones |
| d / datos | Ruta donde se generará el archivo ``datos.txt`` |
| g / grafico | Ruta donde se generará el archivo ``grafico.png`` |
| a | Ruta donde se encuentra el programa python ``analizador.py`` donde se genera el gráfico |

### Ejemplo de ejecución
```bash
Ejecutando conteo con 1 threads, repetición: 1
Ejecutando conteo con 1 threads, repetición: 2
Ejecutando conteo con 4 threads, repetición: 1
Ejecutando conteo con 4 threads, repetición: 2
Ejecutando conteo con 8 threads, repetición: 1
Ejecutando conteo con 8 threads, repetición: 2
Ejecutando conteo con 12 threads, repetición: 1
Ejecutando conteo con 12 threads, repetición: 2
Análisis de Performance realizado correctamente.
Salida de gráfico en ./analisisPerformance/grafico.png
```
Si el programa se ejecuta correctamente, entrega este mensaje y además se genera el archivo ``datos.txt``:
```bash
thread1,tiempo1,tiempo2
thread2,tiempo1,tiempo2
thread3,tiempo1,tiempo2
```
además del grafico  ``grafico.png``.

    