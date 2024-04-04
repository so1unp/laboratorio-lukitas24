#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()

int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

int main(int argc, char *argv[])
{
 int i;
 int pid;
   for(i=1;i<=atoi(argv[2]);i++){
	pid=fork();
	if(pid==0){
	printf("soy hijo %d \n",pid);
	break;
}
else{
 printf("soy padre de: %d\n",pid);
//sleep(5);
}
sleep(5);
}
sleep(5);
    exit(EXIT_SUCCESS);
}
