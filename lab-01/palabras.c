#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
while(1){
	char caracter;
	caracter=getchar();
	if(caracter!='\n'){
		if(caracter==' '){
			printf("\n");
				}
		else{
			printf("%c",caracter);
			}

					}
else{

	printf("\n");
}	
}
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
