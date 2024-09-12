#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
using namespace std;

bool existeDirectorio(const char* path) {
    struct stat info;
    if (stat(path, &info) != 0) {
        return false;
    } else if (info.st_mode & S_IFDIR) {
        return true;
    } else {
        return false;
    }
}

#define MAX_WORD_LEN 100
#define MAX_PATH_LEN 512

// Estructura para almacenar las palabras y su conteo
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int isValidExtension(const char* filename, const char* extension) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return 0;
    return strcmp(dot + 1, extension) == 0;
}

void processFile(const char* inputFile, const char* outputFile) {
    FILE *in = fopen(inputFile, "r");
    if (!in) {
        printf("No se pudo abrir el archivo de entrada: %s\n", inputFile);
        return;
    }

    // Para almacenar las palabras y sus cantidades
    WordCount words[10000]; // Esto es solo un tamaño arbitrario, se puede mejorar
    int wordCount = 0;
    
    char word[MAX_WORD_LEN];
    while (fscanf(in, "%s", word) != EOF) {
        // Convertir la palabra a minúsculas
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }

        // Buscar si la palabra ya está registrada
        int found = 0;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(words[i].word, word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }

        // Si no se encontró, agregarla
        if (!found) {
            strcpy(words[wordCount].word, word);
            words[wordCount].count = 1;
            wordCount++;
        }
    }
    fclose(in);

    // Crear el archivo de salida
    FILE *out = fopen(outputFile, "w");
    if (!out) {
        printf("No se pudo crear el archivo de salida: %s\n", outputFile);
        return;
    }

    // Escribir las palabras y sus cantidades en el archivo de salida
    for (int i = 0; i < wordCount; i++) {
        fprintf(out, "%s; %d\n", words[i].word, words[i].count);
    }
    fclose(out);

    // Mensaje final con el path y la cantidad de palabras distintas
    printf("archivo %s, %d palabras distintas\n", outputFile, wordCount);
}

bool procesarDatos(char* extension, char* pathProcesar, char* pathRespuesta) {
    struct dirent *entry;
    DIR *dp = opendir(pathProcesar);

    if (dp == NULL) {
        printf("No se pudo abrir el directorio: %s\n", pathProcesar);
        return false;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG && isValidExtension(entry->d_name, extension)) {
            char inputFile[MAX_PATH_LEN];
            char outputFile[MAX_PATH_LEN];

            // Construir los paths completos
            snprintf(inputFile, sizeof(inputFile), "%s/%s", pathProcesar, entry->d_name);
            snprintf(outputFile, sizeof(outputFile), "%s/%s", pathRespuesta, entry->d_name);

            // Procesar el archivo
            processFile(inputFile, outputFile);
        }
    }

    closedir(dp);
    return true;
}

bool validarDatos(char* extension, char* pathProcesar, char* pathRespuesta) {
    if (!extension || !pathProcesar || !pathRespuesta) {
        cout << "\033[31mError: Hacen falta datos por ingresar.\033[0m" << endl;
        return false;
    }

    if (!existeDirectorio(pathProcesar)) {
        cout << "\033[31mError: La carpeta a procesar no existe.\033[0m" << endl;
        return false;
    }

    if (!existeDirectorio(pathRespuesta)) {
        cout << "\033[31mError: La carpeta de respuesta no existe.\033[0m" << endl;
        return false;
    }

    cout << "\033[32mÉxito: Ambos directorios existen y la extensión fue proporcionada.\033[0m" << endl;
    return true;
}
