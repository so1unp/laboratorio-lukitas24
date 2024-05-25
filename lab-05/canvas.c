#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define HEIGHT 25 // Altura en caracteres de la pizarra
#define WIDTH 25  // Ancho en caracteres de la pizarra

struct canvas
{
    char canvas[HEIGHT * WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
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

    char option = argv[1][1];
    int shm_fd;
    int x;
    int y;
    int index;
    int i;
    int j;
    canvas_t *canvas;

    switch (option)
    {
    case 'w':
shm_fd = shm_open(argv[2], O_RDWR, 0666);
	
        if (shm_fd == -1)
        {
            perror("Error al abrir el canvas");
            exit(EXIT_FAILURE);
        }
        canvas = mmap(NULL, sizeof(canvas_t), PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (canvas == MAP_FAILED)
        {
            perror("Error en syscall mmap");
            close(shm_fd);
            exit(EXIT_FAILURE);
        }
        x = atoi(argv[4]);
        y = atoi(argv[5]);

        if (argv[6][0] == 'h')
        {
            for (i = 0; i < strlen(argv[3]); i++)
            {
                index = (x * WIDTH + (y + i)) % (HEIGHT * WIDTH);
                canvas->canvas[index] = argv[3][i];
            }
        }
        if (argv[6][0] == 'v')
        {
            for (i = 0; i < strlen(argv[3]); i++)
            {
                index = ((x + i) % HEIGHT) * WIDTH + y;
                canvas->canvas[index] = argv[3][i];
            }
        }
        munmap(canvas, sizeof(canvas_t));

        printf("Escribe %s en el canvas %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));
        close(shm_fd);
        break;
    case 'p':
        printf("Imprime canvas.\n");
        shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
        canvas = mmap(NULL, sizeof(canvas_t), PROT_READ, MAP_SHARED, shm_fd, 0);
        for (i = 0; i < HEIGHT; i++)
        {
            for (j = 0; j < WIDTH; j++)
            {
                if (canvas->canvas[i * WIDTH + j] == 0)
                {
                    printf("-");
                }
                else
                {
                    printf("%c", (canvas->canvas[i * WIDTH + j]));
                }
            }
            printf("\n");
        }
        printf("\n");

        close(shm_fd);
        break;
    case 'c':
        printf("Crea canvas.\n");
	umask(0000);
        shm_fd = shm_open(argv[2], O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1)
        {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }

        if (ftruncate(shm_fd, sizeof(canvas_t)) == -1)
        {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
        close(shm_fd);

        break;
    case 'd':
        printf("Borra canvas.\n");
        shm_unlink(argv[2]);
        break;
    case 'h':
        usage(argv);
        break;
    default:
        fprintf(stderr, "Comando desconocido\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
