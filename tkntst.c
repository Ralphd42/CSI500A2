#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include    <ctype.h>

#include <stdbool.h>
 
void ParseString( const char * thedata);
int main(int argc, char *argv[])
{

   // test();
    int readlen =0;
    char * inBuff ;
    size_t bufflen =0;
    readlen=getline(&inBuff,&bufflen,stdin);

    inBuff[strlen(inBuff)-1]='\0';
    while (readlen!=-1)
    {
        ParseString(inBuff);
        fflush(stdout);
        readlen=getline(&inBuff,&bufflen,stdin);
        inBuff[strlen(inBuff)-1]='\0';
    }






}

 

void ParseString( const char * thedata)
{
    char * mod = strdup(thedata);
    char * div ="<> ";
    char * tkn = strtok( mod, div);
    // get first divider
    printf("fIRST %ld \n",tkn-mod);



    while (tkn)
    {
        printf("\ntkn =%s\n",tkn);
        int from = tkn-mod+strlen(tkn);
        tkn = strtok(NULL,div );
        int to = tkn != NULL ? tkn-mod : strlen(thedata);

     	printf("\nDivider|%.*s|\n", to-from, thedata+from);
        char   divstr[to-from+1];
        memset(&divstr[0], 0, sizeof(divstr));
        strncpy(divstr,  thedata+from,(to-from)     );
        printf("\ndiv width %d - %s - %ld\n",(to-from), divstr, strlen(divstr)   );


    }



}