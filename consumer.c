#include <stdio.h>
#include<stdlib.h>
int main(int argc, int argv[])
{
    int readlen =0;
    char * inBuff ;
    size_t bufflen =0;
    readlen=getline(&inBuff,&bufflen,stdin);
    while(readlen!=-1)
    {
        int val = atoi(inBuff);
        if( val %2 ==0)
        {
            //printf("%s - %d\n",inBuff,readlen);
            printf("%d\n",val );
        }
        readlen=getline(&inBuff,&bufflen,stdin);
    }
    free(inBuff);
    return 0;
}

