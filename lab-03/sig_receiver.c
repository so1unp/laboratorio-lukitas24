#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Función para imprimir el número y el nombre de la señal recibida
void imprimir(int sig) {
    printf("Recibí la señal %d: %s\n", sig, strsignal(sig));
}

struct sigaction signalAction;

int main(void) {
    int pid = getpid();
    printf("Mi PID es: %d\n", pid);
    int i;
    signalAction.sa_handler= imprimir;
    // Configuración de manejo de señales para todas las señales posibles
    for ( i = 1; i < NSIG; i++) {
       sigaction(i,&signalAction,NULL);
    }

    // Bucle infinito para esperar señales
    while (1) {
        pause(); // Se suspende hasta que llegue una señal
    }

    exit(EXIT_SUCCESS);
}


