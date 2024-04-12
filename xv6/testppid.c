#include "types.h"
#include "user.h"
#include "date.h"

int main(void)
{
    printf(1, "Mi PID es %d \n", getpid());
    int pid=fork();
    if (pid == 0)
    {
        printf(1, "El PID de mi padre es %d \n", getppid());
    }
    wait();
    exit();
}
