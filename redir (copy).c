#include <sys/types.h>
#include     <stdio.h>
#include    <unistd.h>
#include  <sys/stat.h>
#include     <fcntl.h>
#include    <stdlib.h>
#include  <sys/wait.h>
int main(int argc, char *argv[])
{/*

    pipe 
    fork
        proc arg 1
        set ouput to pipe
    fork 
        proc 2


*/
    int status;
    int fdpipe [2];
    int pipeStat =0;
    pid_t pid;
    pipeStat = pipe(fdpipe);

    if( pipeStat<0)
    {
        perror("Error creating pipe");
        exit(1);

    } 


    pid_t pid = fork();

    if(pid<0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid==0)
    {
        // child Work happens here
        printf("Child cmd - %s  file - %s",argv[1],argv[2]);
        int fd = creat(argv[2],0640);
        close(1);
        dup(fd);
        close(fd);
        char *cmd = "/usr/bin/ls.exe";
        char *args[3];
        args[0] = "ls";
        args[1] = "-la";
        args[2] = NULL;
        execv(cmd,args             );
        fprintf(stderr, "exec failed\n");
        exit(EXIT_FAILURE);

    }
    pid = wait(&status);
    printf("End parent status %d\n\n", status);


}


