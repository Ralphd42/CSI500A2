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
    int fd [2];
    int pipeStat =0;
    pid_t pid;
    pipeStat = pipe(fd);

    if( pipeStat<0)
    {
        perror("Error creating pipe");
        exit(1);

    } 


    pid = fork();

    if(pid<0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid==0)
    {
        // write to pipe
        close (fd[0]);// close read end of pipe
        int fdesc =dup2(fd[1], STDOUT_FILENO);
        printf( "in child A dup2 %d \n", fdesc);
        if(fdesc<0)
        {
            printf( "C1 Failed to DUP");
            exit(1);


        }
        execlp(argv[1], argv[1], NULL);
        printf("C1  Failed to run PS");
        exit (1);
        }
    if(pid>0){
        pid = wait(&status);
        // close write end of pipe
        close(fd[1]);
        int d =dup2(fd[0], STDIN_FILENO);
        if( d<0)
        {
            printf("Failed to dup\n\n");
            exit(1);

        }
        execlp(argv[2], argv[2], NULL);
        printf("Failed to sort");
    }
    
    printf("End parent status %d\n\n", status);


}


