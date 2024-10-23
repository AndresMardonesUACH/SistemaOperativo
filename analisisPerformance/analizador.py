import pandas as pd
import matplotlib.pyplot as plt
import argparse

# Define argumentos del programa
parser = argparse.ArgumentParser(description='Generar un gráfico de ventas.')
parser.add_argument('--datos', nargs='+', required=True, help='Path de archivo que incluye los datos de las mediciones')
parser.add_argument('--grafico', nargs='+', required=True, help='Path donde se guardará archivo png con el gráfico')
args = parser.parse_args()

# Crea dataframe a partir de los datos del archivo generado anteriormente
archivo = open(args.datos[0], "r")
lista = archivo.readlines()
lista = list(map(lambda x: x.strip().split(','), lista))
lista = [[float(x) for x in sublista] for sublista in lista]
columnas = ['Cant_Threads']
columnasRep = ["Repetición" + str(i) for i in range(1, len(lista[0]))]
columnas.extend(columnasRep)

df = pd.DataFrame(lista, columns = columnas)
# Genera gráfico
df.sort_values(by=['Cant_Threads']).plot(x="Cant_Threads", y=columnasRep, kind="bar", xlabel="Cantidad de Threads", ylabel="Tiempo(seg)", rot=0, title="Tiempos de ejecución de 'Conteo de Palabras Paralelo' según cantidad de threads utilizados", figsize=(9+(len(lista)*.5),6+(len(lista)*.5)))
#Guarda gráfico en el path correspondiente
plt.savefig(args.grafico[0], format="png")