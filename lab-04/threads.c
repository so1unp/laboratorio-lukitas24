#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int cantidadDeSegundos;

void *funcion_hilo(void *p)
{
    long id = (long)p;
    int dormir = rand() % (cantidadDeSegundos + 1);
    printf("Hilo %ld: dormire %d\n", id, dormir);
    sleep(dormir);
    printf("Hilo %ld: termino %d\n", id, dormir);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error en la cantidad de argumentos\n");
        exit(EXIT_FAILURE);
    }
    int cantidadHilos = atoi(argv[1]);
    long i;
    cantidadDeSegundos = atoi(argv[2]);
    pthread_t hilos[cantidadHilos];
    for (i = 1; i <= cantidadHilos; i++)
    {
       
        pthread_create(&hilos[i], NULL, funcion_hilo, (void *)i);

    }

  /* for (i = 1; i < cantidadHilos-1; i++)
    {
        pthread_join(hilos[i], NULL);
    }*/

   return 0;
}
