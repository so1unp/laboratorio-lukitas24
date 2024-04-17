#include "types.h"
#include "user.h"
#include "date.h"

#define NUM_CHILDREN 5

int main() {
int i;   
 printf(1,"Número de procesos en ejecucuion: %d\n", pscnt());
    


    // Crear procesos hijos
    for (i = 0; i < NUM_CHILDREN; i++) {
        if (fork() == 0) {
            // Proceso hijo
            printf(1, "Soy el proceso hijo %d con PID %d\n", i+1, getpid());
            exit();
        }
    }
     printf(1, "Número de procesos en el sistema: %d\n", pscnt());
 
    // Esperar a que todos los hijos terminen
    for (i = 0; i < NUM_CHILDREN; i++) {
        wait();
}

 printf(1,"Número de procesos en ejecucuion: %d\n", pscnt());


    exit();
}

