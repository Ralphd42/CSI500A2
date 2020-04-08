#include <sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    pid_t pid = fork();
    int status =0;
    if(pid==0)
    {
        printf("IN CHILD\n\n");
        int fd= creat("Fchild", 0640);
        printf("IN CHILD fd %d\n\n",fd);
        write(fd,"CHILD!!!!",12);
        /////////////////////////////////////////////////////////////////////////////////dup2
        close(fd);
    }else if(pid >0)
    {
        pid = wait(&status);
        printf("IN PARENT status = %d\n\n",status);

    }
    



    return 0;
}