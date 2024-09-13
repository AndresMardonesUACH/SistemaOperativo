# Nombre del compilador
CXX = g++

# Flags del compilador
CXXFLAGS = -Wall -std=c++11

# Objetivos
TARGET = conteoPalabras

# Archivos fuente
SRC = conteoPalabras.cpp

# Archivos objeto
OBJ = conteoPalabras.o

# Regla para compilar el programa principal
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Regla para compilar los archivos objeto
$(OBJ): $(SRC) procesar.h
	$(CXX) $(CXXFLAGS) -c $(SRC)

# Limpieza de archivos compilados
clean:
	rm -f $(OBJ) $(TARGET)
