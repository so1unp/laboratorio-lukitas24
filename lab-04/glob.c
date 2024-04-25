#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variable global que incrementan los hilos.
static long glob = 0;

void *increment_glob(void *p)
{
    int loc;
    long loops=(long)p;
    long j;
    // incrementa glob
    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
	glob = loc;

    }
 pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    long loops;

    // Controla numero de argumentos.
    if (argc != 2) {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // Verifica argumentos.
    if (loops < 1) {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_t hilo1;
    pthread_t hilo2;
    pthread_create(&hilo1,NULL,increment_glob,(void *) loops);
    pthread_create(&hilo2,NULL,increment_glob,(void *) loops);

	pthread_join(hilo1,NULL);
    pthread_join(hilo2,NULL);

    printf("%ld\n", glob);

    exit(EXIT_SUCCESS);
}




