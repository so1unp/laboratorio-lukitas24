#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Agregar código aquí.
	if(argc>1){
	int i;
	int suma=0;
	for (i=1;i<argc; i++){
	suma+=atoi(argv[i]);
} 
	printf("la suma es: %d\n", suma);
}
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
