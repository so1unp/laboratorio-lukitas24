#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>

#define ITEMS 15
#define MAX_WORD 50
#define PERMISOS_ESCRIBIR_Y_LEER 0666
#define PERMISOS_ESCRIBIR 0662

struct wordstack
{
    int free;
    int items;
    int max_word;
    pthread_mutex_t mutex;
    sem_t full;
    sem_t empty;
    char heap[ITEMS][MAX_WORD];
};

typedef struct wordstack wordstack_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w pila palabra: agrega palabra en la pila\n");
    fprintf(stderr, "\t-r pila pos: elimina la palabra de la pila.\n");
    fprintf(stderr, "\t-p pila: imprime la pila de palabras.\n");
    fprintf(stderr, "\t-c pila: crea una zona de memoria compartida con el nombre indicado donde almacena la pila.\n");
    fprintf(stderr, "\t-d pila: elimina la pila indicada.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-')
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    int i;
    int shm_fd;
    char option = argv[1][1];
    wordstack_t *wordstack;
    switch (option)
    {
    case 'w':
        shm_fd = shm_open(argv[2], O_RDWR, PERMISOS_ESCRIBIR_Y_LEER);
        wordstack = mmap(NULL, sizeof(wordstack_t), PROT_WRITE, MAP_SHARED, shm_fd, 0);

        sem_wait(&wordstack->empty);
        pthread_mutex_lock(&wordstack->mutex);
        strncpy((wordstack->heap[wordstack->free]), argv[3], strlen(argv[3]));
        wordstack->free = wordstack->free - 1;
        wordstack->items= wordstack->items +1;
        pthread_mutex_unlock(&wordstack->mutex);
        sem_post(&wordstack->full);

        printf("Se envió %s a pila %s\n", argv[3], argv[2]);        
        break;
    case 'r':
        shm_fd = shm_open(argv[2], O_RDWR, PERMISOS_ESCRIBIR);
        wordstack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    

        
          sem_wait(&wordstack->full);
            pthread_mutex_lock(&wordstack->mutex);
            printf("%s\n", wordstack->heap[wordstack->free + 1]);
            wordstack->free = wordstack->free + 1;
            bzero(&(wordstack->heap[wordstack->free]), 50);
                    wordstack->items= wordstack->items -1;
            pthread_mutex_unlock(&wordstack->mutex);

        sem_post(&wordstack->empty);
        

        break;
    case 'p':
     shm_fd = shm_open(argv[2], O_RDWR, PERMISOS_ESCRIBIR);
        wordstack = mmap(NULL, sizeof(wordstack_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (wordstack->items == 0)
        {
            printf("no hay nada para imprimir\n");
		exit(0);
        }
        else{
            pthread_mutex_lock(&wordstack->mutex);
            for(i=wordstack->free+1;i<ITEMS;i++ ){
            printf("%s\n", wordstack->heap[i]);
            }
            pthread_mutex_unlock(&wordstack->mutex);

        }
        break;
    case 'c':
        printf("Crea pila %s.\n",argv[2]);
	umask(0000);
        shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, PERMISOS_ESCRIBIR);
        wordstack = mmap(NULL, sizeof(wordstack_t), PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if (shm_fd == -1)
        {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }

        if (ftruncate(shm_fd, sizeof(wordstack_t)) == -1)
        {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }

        wordstack->free = ITEMS-1;
        wordstack->items = 0;
        wordstack->max_word = MAX_WORD;
        pthread_mutex_init(&(wordstack->mutex), NULL);
        sem_init(&(wordstack->full), 1, 0);
        sem_init(&(wordstack->empty), 1, ITEMS-1);
        close(shm_fd);
        break;
    case 'd':
        shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, PERMISOS_ESCRIBIR);
        wordstack = mmap(NULL, sizeof(wordstack_t), PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if (shm_fd == -1)
        {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }


        pthread_mutex_destroy(&(wordstack->mutex));
        sem_destroy(&(wordstack->full));
        sem_destroy(&(wordstack->empty));
         
        shm_unlink(argv[2]);
        break;
    case 'h':
        usage(argv);
        break;
    default:
        fprintf(stderr, "-%c: opciÃ³n desconocida.\n", option);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
