# Definir compilador y opciones
CXX = g++
CXXFLAGS = -I include

# Definir los nombres de los ejecutables
MAIN_EXE = main
CONTEO_EXE = ./conteoPalabras/conteoPalabras

# Regla que compila ambos programas
all: $(MAIN_EXE) $(CONTEO_EXE)

# Regla para compilar el programa "main"
$(MAIN_EXE): main.cpp
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXE) main.cpp

# Regla para compilar el programa "conteoPalabras"
$(CONTEO_EXE): ./conteoPalabras/conteoPalabras.cpp ./conteoPalabras/procesar.h
	$(CXX) -o $(CONTEO_EXE) ./conteoPalabras/conteoPalabras.cpp

# Limpiar los ejecutables generados
clean:
	rm -f $(MAIN_EXE) $(CONTEO_EXE)
