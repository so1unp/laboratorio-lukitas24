#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int main (int argc,char *argv[]){

if(argc==3){
kill(atoi(argv[2]),atoi(argv[1]));
}
else{
printf("faltan parametros\n");
}

exit(EXIT_SUCCESS);
}
