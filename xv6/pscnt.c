#include "types.h"
#include "user.h"
#include "date.h"


#define SYS_pscnt 25 

int main() {
    int num_processes_before, num_processes_after, i;

    // Obtener el número de procesos antes de crear procesos hijos
    num_processes_before = pscnt();
    printf(1,"Número de procesos antes de crear hijos: %d\n", num_processes_before);

    // Crear procesos hijos
    for (i = 0; i < 5; i++) {
        if (fork() == 0) { // Proceso hijo
            // En el proceso hijo, solo imprimir un mensaje y salir
            printf(1,"Proceso hijo creado\n");
            exit();
        }
    }

    // Obtener el número de procesos después de crear procesos hijos
    num_processes_after = pscnt();
    printf(1,"Número de procesos después de crear hijos: %d\n", num_processes_after);

    return 0;
}

