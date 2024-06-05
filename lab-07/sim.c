#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL 16
#define FISICA 8
#define SWAP 16
#define MAX_PROCESOS 100

int lru_index=0;
struct Pagina
{
    int pid;
    int indice;
    int nroEnMemoria;
    int memoria; // 1 es memoria ram, 2 es memoria secundaria
    int lru_indice; //indice para el remplazo lru
};
typedef struct Pagina pagina_t;

struct Proceso
{
    int pid;
    pagina_t paginas[VIRTUAL];
    int activo;
};
typedef struct Proceso process_t;

struct MemSecundaria
{
    pagina_t paginas[SWAP];
};
typedef struct MemSecundaria memSecundaria_t;

struct RAM
{
    pagina_t paginas[FISICA];
    int fifo_index; // Índice para el reemplazo FIFO
};
typedef struct RAM ram_t;

void usage(char *programa){
    printf("Uso: %s -f | -l \n",programa);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if(argc!= 2){
        usage(argv[0]);
        }

    char modo=argv[1][1];

    ram_t ram;
    memSecundaria_t memSecundaria;
 process_t procesos[MAX_PROCESOS];

    int i;
    for (i = 0; i < MAX_PROCESOS; i++)
    {
     	procesos[i].activo = -1;
    }

    for (i = 0; i < FISICA; i++)
    {
     	ram.paginas[i].pid = -1;
    }

    for (i = 0; i < SWAP; i++)
    {
     	memSecundaria.paginas[i].pid = -1;
  }
    ram.fifo_index = 0; // Inicializa el índice FIFO
    int encontroHueco;
    int pid;
    int pagina;
    int paginaRemplazoLru;
    int paginaRemplazo;
    while (scanf("%d\n%d", &pid, &pagina) != EOF)
    {
     	encontroHueco=0;
        paginaRemplazo=0;
        if (procesos[pid - 1].activo == -1)
        {
            procesos[pid - 1].activo = 1;
            procesos[pid - 1].pid = pid;
            for (i = 0; i < VIRTUAL; i++)
            {
             	procesos[pid - 1].paginas[i].pid = pid;
                procesos[pid - 1].paginas[i].nroEnMemoria = -1;
                procesos[pid - 1].paginas[i].indice = i;
                procesos[pid - 1].paginas[i].lru_indice = -1;
            }
	}

    if(modo=='l' && procesos[pid - 1].activo==1){
                procesos[pid - 1].paginas[pagina].lru_indice=lru_index;
                lru_index++;

    }else {




	// accedo a la ram
        for (i = 0; i < FISICA; i++)
        {
            if (ram.paginas[i].pid == -1)
            {
             	procesos[pid - 1].paginas[pagina].nroEnMemoria = i;
                procesos[pid - 1].paginas[pagina].memoria = 1;
                procesos[pid - 1].paginas[pagina].lru_indice=lru_index;
                lru_index++;
                ram.paginas[i] = procesos[pid - 1].paginas[pagina];
                encontroHueco = 1;
                break;
            }
 }

	if (encontroHueco == 0){
            //accedo a la memoria secundaria
            if(modo=='f'){
                for (i = 0; i < SWAP; i++)
                    {
                    if (memSecundaria.paginas[i].pid == -1)
                {
                    procesos[pid - 1].paginas[pagina].nroEnMemoria = i;
                    procesos[pid - 1].paginas[pagina].memoria = 1;
                    procesos[pid - 1].paginas[pagina].lru_indice=lru_index;
                    lru_index++;
                    memSecundaria.paginas[i]=ram.paginas[ram.fifo_index];
                    break;
                }
            } 
            //si esta todo lleno tiene que terminar el programa y no deberia hacer nada
                ram.paginas[ram.fifo_index] = procesos[pid - 1].paginas[pagina];
                ram.fifo_index = (ram.fifo_index + 1) % FISICA; // Avanza el índice FIFO circularmente
            }
            if(modo=='l'){
                    paginaRemplazoLru=ram.paginas[0].lru_indice;
                for (i = 1; i < FISICA; i++){
                    if(paginaRemplazoLru>ram.paginas[i].lru_indice){
                    paginaRemplazo=i;
                    }
                      }
                for (i = 0; i < SWAP; i++)
                    {
                    if (memSecundaria.paginas[i].pid == -1)
                        {
                    procesos[pid - 1].paginas[pagina].nroEnMemoria = i;
                    procesos[pid - 1].paginas[pagina].memoria = 1;
                    procesos[pid - 1].paginas[pagina].lru_indice=lru_index;
                    lru_index++;
                    memSecundaria.paginas[i]=ram.paginas[paginaRemplazo];
                    break;
                    }
                ram.paginas[paginaRemplazo]=procesos[pid -1].paginas[pagina];
                  
	            }
            }
    }
    }
// Imprimo la tabla de paginacion de cada proceso
    int j;
    int k;
    for (j = 0; j < MAX_PROCESOS; j++) {

        if (procesos[j].activo == 1 && procesos[j].pid != -1) {
            printf("Proceso %d: ", procesos[j].pid);

            for (k = 0; k <VIRTUAL; k++) {
                if (procesos[j].paginas[k].nroEnMemoria == -1) {
                    printf("- ");
                } else {
                    printf("%d ", procesos[j].paginas[k].nroEnMemoria);
                }
             }
	    printf("\n");
        }
    }

    // Imprimo la memoria fisica
    printf("Memoria fisica: ");
    for (j = 0; j < FISICA; j++) {

        if (ram.paginas[j].pid == -1) {
            printf("- ");
        } else {
            printf("%d.%d ", ram.paginas[j].pid, ram.paginas[j].indice);
        }
    }
    printf("\n");
  // Imprimo la memoria secundaria
    printf("Memoria secundaria: ");
    for (i = 0; i < SWAP; i++) {

        if (memSecundaria.paginas[i].pid == -1) {
            printf("- ");
        } else {
            printf("%d.%d ", memSecundaria.paginas[i].pid, memSecundaria.paginas[i].indice);
        }
    }
    printf("\n");



    exit(EXIT_SUCCESS);

}
    
