#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int cantidadSegundos=0;

void *funcionDelHilo(void* p){
 long id = (long)p;
 printf("Soy el hilo (%ld) y voy a dormir %d segundos\n", id, cantidadSegundos);
 int tiempoDormido=rand() % (cantidadSegundos+1);
    sleep(tiempoDormido); 
    printf("El hilo (%ld) ha terminado de dormir.\n", id);
    pthread_exit(NULL);
}





int main(int argc, char* argv[])
{
    if(argc!=3){
        printf("formato incorrecto");
        exit(EXIT_FAILURE);
        }
    long i;
    cantidadSegundos=atoi(argv[2]);
    int cantidadDeHilos=atoi(argv[1]);


    pthread_t hilos[cantidadDeHilos];

    for(i=0;i < cantidadDeHilos;i++){
        pthread_create(&hilos[i],NULL,funcionDelHilo,(void *) i);    
    }

    for(i=0;i < cantidadDeHilos;i++){
        pthread_join(hilos[i],NULL);
    }
    exit(EXIT_SUCCESS);
}
