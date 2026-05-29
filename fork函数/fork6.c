#include <unistd.h>
#include <sys/types.h>
#include <stdio.h> 

main()
{
    pid_t pid;
    int  m = 5;
    pid = -100;
    printf("Before fork:m==%d;pid==%d\n", m, pid);
    pid = fork();
    if (pid < 0)
        printf("error in fork!");
    else if (pid == 0)
    {
        printf("I am the child process, my process id is %d\n", getpid());
        printf("m is %d now\n", m);
        m = 10;
        printf("m is changed to %d now\n", m);
    }
    else
    {
        printf("I am the parent process, my process id is %d\n", getpid());
        printf("m is %d now\n", m);
        m = 100;
        printf("m is changed to %d now\n", m);
    }
}
