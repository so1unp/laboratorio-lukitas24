#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL 16
#define FISICA 8
#define SWAP 16
#define MAX_PROCESOS 100

int lru_index = 0;
struct Pagina
{
    int pid;
    int indice;
    int nroEnMemoria;
    int memoria;    // 1 es memoria ram, 2 es memoria secundaria
    int lru_indice; // indice para el remplazo lru
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

void usage(char *programa)
{
    printf("Uso: %s -f | -l \n", programa);
    exit(EXIT_FAILURE);
}

void imprimirTablaPaginacion(process_t procesos[])
{
    int j, k;
    printf("\n");
    printf("R indica memoria ram, S indica memoria secundaria. Se va a imprimir con el siguiente formato: memoria en la que esta.marco de la memoria\n");
    for (j = 0; j < MAX_PROCESOS; j++)
    {
        if (procesos[j].activo == 1 && procesos[j].pid != -1)
        {
            printf("Proceso %d: ", procesos[j].pid);
            for (k = 0; k < VIRTUAL; k++)
            {
                if (procesos[j].paginas[k].nroEnMemoria == -1)
                {
                    printf("- ");
                }
                else if (procesos[j].paginas[k].memoria == 1)
                {
                    printf("r.%d ", procesos[j].paginas[k].nroEnMemoria);
                }
                else if (procesos[j].paginas[k].memoria == 2)
                {
                    printf("s.%d ", procesos[j].paginas[k].nroEnMemoria);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

void imprimirMemorias(ram_t ram, memSecundaria_t memSecundaria)
{
    int i, j;
    printf("Las memorias se van a imprimir con el siguiente formato: Proceso.Pagina del proceso\n");
    // Imprimo la memoria fisica
    printf("Memoria fisica: ");
    for (j = 0; j < FISICA; j++)
    {
        if (ram.paginas[j].pid == -1)
        {
            printf("- ");
        }
        else
        {
            printf("%d.%d ", ram.paginas[j].pid, ram.paginas[j].indice);
        }
    }
    printf("\n");

    // Imprimo la memoria secundaria
    printf("Memoria secundaria: ");
    for (i = 0; i < SWAP; i++)
    {
        if (memSecundaria.paginas[i].pid == -1)
        {
            printf("- ");
        }
        else
        {
            printf("%d.%d ", memSecundaria.paginas[i].pid, memSecundaria.paginas[i].indice);
        }
    }
    printf("\n");
}

int gestionarProcesoEnMemoria(int modo, int pid, int pagina, process_t procesos[], ram_t *ram, memSecundaria_t *memSecundaria)
{
    if (modo == 'l')
    {
        if (procesos[pid - 1].paginas[pagina].memoria == 1)
        {

            printf("Actualizando lru_indice de pagina en RAM: Proceso %d, Pagina %d, lru_indice %d -> %d\n", pid, pagina, procesos[pid - 1].paginas[pagina].lru_indice, lru_index);
            procesos[pid - 1].paginas[pagina].lru_indice = lru_index;
            ram->paginas[procesos[pid - 1].paginas[pagina].nroEnMemoria].lru_indice = lru_index;
            lru_index++;
            return 0; // Indica que no se debe continuar con la siguiente iteración
        }
        if (procesos[pid - 1].paginas[pagina].memoria == 2)
        {
            int marcoSwap = procesos[pid - 1].paginas[pagina].nroEnMemoria;
            printf("La página %d del Proceso %d está en memoria secundaria en el marco %d\n", pagina, pid, marcoSwap);

            // Encontrar la página en RAM con el menor lru_indice
            int marcoRam = 0;
            int minLruIndice = ram->paginas[0].lru_indice;
            for (int i = 1; i < FISICA; i++)
            {
                if (ram->paginas[i].lru_indice < minLruIndice)
                {
                    minLruIndice = ram->paginas[i].lru_indice;
                    marcoRam = i;
                }
            }
            printf("Página a reemplazar en RAM: Proceso %d, Pagina %d, marco %d, lru_indice %d\n", ram->paginas[marcoRam].pid, ram->paginas[marcoRam].indice, marcoRam, minLruIndice);


            // Guarda la página en Swap
            pagina_t paginaEnSwap = memSecundaria->paginas[marcoSwap];
            pagina_t paginaEnRAM = ram->paginas[marcoRam];

            printf("Moviendo de MemSecundaria a RAM: Proceso %d, Pagina %d de marco %d en memoria secundaria a marco %d en RAM\n", pid, pagina, marcoSwap, marcoRam);

            // Intercambiamos las páginas
            memSecundaria->paginas[marcoSwap] = paginaEnRAM;
            memSecundaria->paginas[marcoSwap].memoria = 2;
            procesos[paginaEnRAM.pid - 1].paginas[paginaEnRAM.indice].nroEnMemoria = marcoSwap;
            procesos[paginaEnRAM.pid - 1].paginas[paginaEnRAM.indice].memoria = 2;

            printf("Guardando en memoria secundaria: Proceso %d, Pagina %d, lru_indice %d\n", paginaEnRAM.pid, paginaEnRAM.indice, paginaEnRAM.lru_indice);

            // Movemos la página desde swap a RAM
            ram->paginas[marcoRam] = paginaEnSwap;
            ram->paginas[marcoRam].memoria = 1;
            ram->paginas[marcoRam].lru_indice = lru_index;
            procesos[pid - 1].paginas[pagina].nroEnMemoria = marcoRam;
            procesos[pid - 1].paginas[pagina].memoria = 1;
            procesos[pid - 1].paginas[pagina].lru_indice = lru_index;

            lru_index++;
            return 0; // Indica que no se debe continuar con la siguiente iteración
        }
    }
    if (modo == 'f')
    {
        if (procesos[pid - 1].paginas[pagina].memoria == 1)
        {

            printf("La pagina:%d del Proceso: %d ya se encuentra en la memoria\n", pagina, pid);
            return 0;
        }
        if (procesos[pid - 1].paginas[pagina].memoria == 2)
        {
            int marcoSwap = procesos[pid - 1].paginas[pagina].nroEnMemoria;
            int marcoRam = ram->fifo_index;
            printf("Moviendo de MemSecundaria a RAM: Proceso %d, Pagina %d de marco %d en memoria secundaria a marco %d en RAM\n", pid, pagina, marcoSwap, marcoRam);
            pagina_t paginaEnSwap = memSecundaria->paginas[marcoSwap]; // Sacamos la página que estaba en RAM (FIFO) y la ponemos en swap
            pagina_t paginaEnRAM = ram->paginas[marcoRam];             // Guarda la página en Swap
            // Intercambiamos las páginas
            memSecundaria->paginas[marcoSwap] = paginaEnRAM;
            memSecundaria->paginas[marcoSwap].memoria = 2;
            procesos[paginaEnRAM.pid - 1].paginas[paginaEnRAM.indice].nroEnMemoria = marcoSwap;
            procesos[paginaEnRAM.pid - 1].paginas[paginaEnRAM.indice].memoria = 2;
            // Movemos la página desde swap a RAM
            ram->paginas[marcoRam] = paginaEnSwap;
            ram->paginas[marcoRam].memoria = 1;
            procesos[pid - 1].paginas[pagina].nroEnMemoria = marcoRam;
            procesos[pid - 1].paginas[pagina].memoria = 1;
            ram->fifo_index = (ram->fifo_index + 1) % FISICA;
            return 0;
        }
    }
    return 1;
}

void verificarMemoriaSecundaria(memSecundaria_t *memSecundaria) {
    for (int i = 0; i < SWAP; i++) {
        if (memSecundaria->paginas[i].pid == -1) {
            return;
        }
    }
    printf("Error: Memoria secundaria llena.\n");
    exit(EXIT_FAILURE);
}

void inicializarProcesoSiEsNecesario(process_t procesos[], int pid)
{
    if (procesos[pid - 1].activo == -1)
    {
        procesos[pid - 1].activo = 1;
        procesos[pid - 1].pid = pid;
        for (int i = 0; i < VIRTUAL; i++)
        {
            procesos[pid - 1].paginas[i].pid = pid;
            procesos[pid - 1].paginas[i].nroEnMemoria = -1;
            procesos[pid - 1].paginas[i].indice = i;
            procesos[pid - 1].paginas[i].lru_indice = -1;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
    }
    char modo = argv[1][1];
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
    ram.fifo_index = 0;
    int encontroHueco;
    int pid;
    int pagina;
    int paginaRemplazoLru;
    int paginaRemplazo;
    while (scanf("%d\n%d", &pid, &pagina) != EOF)
    {
        if(pid>MAX_PROCESOS-1 || pagina>VIRTUAL-1){
            printf("No se soporta el proceso %d pagina %d. procesos rango: 1-100. paginas rango:0-15 \n",pid,pagina);
            exit(EXIT_FAILURE);
        }
        verificarMemoriaSecundaria(&memSecundaria);

        encontroHueco = 0;
        paginaRemplazo = 0;
        inicializarProcesoSiEsNecesario(procesos, pid);

        if (gestionarProcesoEnMemoria(modo, pid, pagina, procesos, &ram, &memSecundaria) == 1)
        {
            // accedo a la ram
            for (i = 0; i < FISICA; i++)
            {
                if (ram.paginas[i].pid == -1)
                {
                    procesos[pid - 1].paginas[pagina].nroEnMemoria = i;
                    procesos[pid - 1].paginas[pagina].memoria = 1;
                    procesos[pid - 1].paginas[pagina].lru_indice = lru_index;
                    lru_index++;
                    ram.paginas[i] = procesos[pid - 1].paginas[pagina];
                    printf("Cargando en RAM: Proceso %d, Pagina %d en marco %d\n", pid, pagina, i);
                    encontroHueco = 1;
                    break;
                }
            }
            if (encontroHueco == 0)
            {
                // accedo a la memoria secundaria
                if (modo == 'f')
                {
                    printf("No se encontró hueco en RAM, se va a utilizar FIFO\n");
                    for (i = 0; i < SWAP; i++)
                    {
                        if (memSecundaria.paginas[i].pid == -1)
                        {
                            procesos[pid - 1].paginas[pagina].nroEnMemoria = ram.fifo_index;
                            procesos[pid - 1].paginas[pagina].memoria = 1;
                            procesos[pid - 1].paginas[pagina].lru_indice = lru_index;
                            lru_index++;
                            memSecundaria.paginas[i] = ram.paginas[ram.fifo_index];
                            memSecundaria.paginas[i].memoria = 2;
                            procesos[ram.paginas[ram.fifo_index].pid - 1].paginas[ram.paginas[ram.fifo_index].indice].memoria = 2;
                            printf("Moviendo de RAM a MemSecundaria: Proceso %d, Pagina %d de marco %d a marco %d en swap\n", ram.paginas[ram.fifo_index].pid, ram.paginas[ram.fifo_index].indice, ram.fifo_index, i);
                            break;
                        }
                    }
                    ram.paginas[ram.fifo_index] = procesos[pid - 1].paginas[pagina];
                    printf("Cargando en RAM: Proceso %d, Pagina %d en marco %d\n", pid, pagina, ram.fifo_index);
                    ram.fifo_index = (ram.fifo_index + 1) % FISICA;
                }
                if (modo == 'l')
                {
                    printf("No se encontró hueco en RAM, se va a utilizar LRU\n");

                    paginaRemplazo = 0;
                    paginaRemplazoLru = ram.paginas[0].lru_indice;
                    for (i = 1; i < FISICA; i++)
                    {
                        if (paginaRemplazoLru > ram.paginas[i].lru_indice)
                        {
                            paginaRemplazoLru = ram.paginas[i].lru_indice;
                            paginaRemplazo = i;
                        }
                    }
                    printf("Página a reemplazar: Proceso %d, Pagina %d, marco %d, lru_indice %d\n", ram.paginas[paginaRemplazo].pid, ram.paginas[paginaRemplazo].indice, paginaRemplazo, ram.paginas[paginaRemplazo].lru_indice);
                    for (i = 0; i < SWAP; i++)
                    {
                        if (memSecundaria.paginas[i].pid == -1)
                        {
                            procesos[pid - 1].paginas[pagina].nroEnMemoria = paginaRemplazo;
                            procesos[pid - 1].paginas[pagina].memoria = 1;
                            procesos[pid - 1].paginas[pagina].lru_indice = lru_index;
                            lru_index++;
                            memSecundaria.paginas[i] = ram.paginas[paginaRemplazo];
                            memSecundaria.paginas[i].memoria = 2;
                            procesos[ram.paginas[paginaRemplazo].pid - 1].paginas[ram.paginas[paginaRemplazo].indice].memoria = 2;
                            procesos[ram.paginas[paginaRemplazo].pid - 1].paginas[ram.paginas[paginaRemplazo].indice].nroEnMemoria = i;

                            printf("Moviendo de RAM a MemSecundaria: Proceso %d, Pagina %d de marco %d a marco %d en swap, lru_indice %d\n", ram.paginas[paginaRemplazo].pid, ram.paginas[paginaRemplazo].indice, paginaRemplazo, i, ram.paginas[paginaRemplazo].lru_indice);
                            break;
                        }
                    }
                    ram.paginas[paginaRemplazo] = procesos[pid - 1].paginas[pagina];
                    printf("Cargando en RAM: Proceso %d, Pagina %d en marco %d, lru_indice %d\n", pid, pagina, paginaRemplazo, lru_index - 1);
                }
            }
        }
    }
    imprimirTablaPaginacion(procesos);
    imprimirMemorias(ram, memSecundaria);
    exit(EXIT_SUCCESS);
}