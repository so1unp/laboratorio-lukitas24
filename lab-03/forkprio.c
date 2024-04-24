#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>

int cantidadHijos;
int hijos[100];

int busywork(void)
{
    struct tms buf;
    for (;;) {
	times(&buf);
    }
}

void terminarHijo(int signal){
(void)signal;
         struct rusage usage;
        getrusage(RUSAGE_SELF,&usage);
                 printf("Child %d (nice %2d):\t%3li\n",getpid(),getpriority(PRIO_PROCESS,0),usage.ru_utime.tv_sec+usage.ru_stime.tv_sec);
                exit(0);

}

void matar (){
        int i;
           for(i=0; i<cantidadHijos;i++){
                kill(hijos[i],SIGTERM);
                }
}

struct sigaction signalAction;



int main(int argc, char *argv[])
{
if(argc!=4){
    printf("cantidad de parametros invalidos.\n");
    printf("formato: forkprio [cantidad de hijos] [cantidad de segundos] [prioridad si o no (0 o 1)].\n");

 exit(EXIT_FAILURE);
}

if( (atoi(argv[1])<0) || (atoi(argv[2])<0) || (  (atoi(argv[3])) !=0 && (atoi(argv[3])!=1 ))   )  {
    printf("cantidad de parametros invalidos.\n");
    printf("formato: forkprio [cantidad de hijos] [cantidad de segundos] [prioridad si o no (0 o 1)].\n");
    exit(EXIT_FAILURE);
    }
 int i;
 cantidadHijos=atoi(argv[1]);
 int pid;


signalAction.sa_handler= terminarHijo;
sigaction(SIGTERM,&signalAction,NULL);


signalAction.sa_handler=SIG_IGN;
sigaction(SIGINT,&signalAction,NULL);
   for(i=0;i<atoi(argv[1]);i++){

        pid=fork();
        if(pid==0){
                if(atoi(argv[3])==1){
                        setpriority(PRIO_PROCESS,(id_t) pid,i);
                }
                busywork();

        }
	else{
             	hijos[i]=pid;
                       }

}//termina for

  
      int duerme=atoi(argv[2]);
 	signalAction.sa_handler=matar;
                sigaction(SIGINT,&signalAction,NULL);

        if(duerme>0){
                
                sleep((unsigned int)duerme);
                matar();
              
        }else{
                    pause();
        }
    exit(EXIT_SUCCESS);
}



