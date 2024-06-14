#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s nombre_Archivo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *ruta_archivo = argv[1];
    struct stat info_archivo;

    if (stat(ruta_archivo, &info_archivo) < 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("ID del usuario propietario: %u\n", info_archivo.st_uid);

    printf("Tipo de archivo: ");
    switch (info_archivo.st_mode & S_IFMT) {
        case S_IFBLK:  printf("dispositivo de bloques\n");  break;
        case S_IFCHR:  printf("dispositivo de caracteres\n"); break;
        case S_IFDIR:  printf("directorio\n");               break;
        case S_IFIFO:  printf("FIFO\n");             	     break;
        case S_IFLNK:  printf("enlace simbólico\n");         break;
        case S_IFREG:  printf("archivo regular\n");          break;
        case S_IFSOCK: printf("socket\n");                   break;
        default:       printf("desconocido\n");             break;
    }

    printf("Número de i-nodo: %lu\n", info_archivo.st_ino);
    printf("Tamaño en bytes: %ld\n", info_archivo.st_size);
    printf("Número de bloques que ocupa en disco: %ld\n", info_archivo.st_blocks);
    printf("Tiempo de última modificación: %s", ctime(&info_archivo.st_mtime));
    printf("Tiempo de último acceso: %s", ctime(&info_archivo.st_atime));

    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
