#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX_CAPACIDAD 20

int main(int argc, char *argv[]) {
    int i,j,c, longitud;
    int palabras[MAX_CAPACIDAD];
    bool espacio=false;

    for (i = 0; i < MAX_CAPACIDAD; i++) {
        palabras[i] = 0;
    }

    longitud = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            espacio = false;
            if (longitud > 0 && longitud < MAX_CAPACIDAD) {
                palabras[longitud - 1]++;
            }
            longitud = 0;
        } else if (!espacio) {
            espacio = true;
            longitud = 1;
        } else {
            longitud++;
        }
    }

    printf("Histograma de Longitudes de Palabras:\n");
    for (i = 0; i < MAX_CAPACIDAD; i++) {
        if (palabras[i] > 0) {
            printf("%2d: ", i + 1);
            for (j = 0; j < palabras[i]; j++) {
                printf("*");
            }
            printf("\n");
        }
    }

 
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
