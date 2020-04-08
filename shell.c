#include  "parse.h"
#include "shellcmd.h"

int main(int argc, char *argv[])
{
    char **params;
    int readlen =0;
    char * inBuff ;
    size_t bufflen =0;
    readlen=getline(&inBuff,&bufflen,stdin);
    //while(readlen!=-1)
    while( strncmp(inBuff,"exit",4)!=0)
    {
        if( readlen!=-1){
            char * trimmed = rtrim(ltrim(inBuff));
            bool isBG =isBackGroundProc(trimmed);
            int numpipes = cntPipes(trimmed);
            params=allocateArr(numpipes,params,cmdsize);
            SplitString(params,numpipes,trimmed);
            int fd[2];
            pid_t fkmn =fork();
            if( fkmn<0)
            {
                perror("FAILED INIT FORK");
            }
            if( fkmn==0){
                pipe_proc(0,numpipes,params,fd);
                exit(0);
            }
            if( fkmn>0)
            {
                int mnsta;
                if(!isBG){
                    pid_t t=wait(&mnsta);
                }
            }
            fflush(stdout);
        }
        fflush(stdout);
        readlen=getline(&inBuff,&bufflen,stdin);
    }
    return (EXIT_SUCCESS);
}


