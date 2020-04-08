#include     <stdio.h>
#include    <unistd.h>
#include  <sys/stat.h> 
#include    <stdlib.h>
#include  <sys/wait.h>
int main(int argc, char *argv[])
{
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
        close (fd[0]);
        int fdesc =dup2(fd[1], STDOUT_FILENO);
        if(fdesc<0)
        {
            fprintf(stderr, "C1 Failed to DUP");
            exit(EXIT_FAILURE);
        }
        int status =execl(argv[1], argv[1], NULL);
        fprintf(stderr,"C1  Failed to run %s %d",argv[1], status);
        exit (EXIT_FAILURE);
    }
    if(pid>0)
    {
        pid = wait(&status);
        close(fd[1]);
        int d =dup2(fd[0], STDIN_FILENO);
        if( d<0)
        {
            fprintf(stderr,"Failed to dup\n\n");
            exit(1);
        }
        int stat =execl(argv[2], argv[2], NULL);
        fprintf(stderr,"Failed to Start %s %d \n\n", argv[2],stat);
        exit(1);
    }
}


