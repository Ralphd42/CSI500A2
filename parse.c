#include "parse.h"
char * rtrim ( char * str)
{
    int len = strlen(str)-1;
    while(   isspace(* (str+len))  && len>=0)
    {
       *(str+len) ='\0';
       len--;
    }
    return str;
}

char * ltrim( char * str)
{
    int len  = strlen(str);
    int offset =0;
    char * str_tmp = str;
    while( offset<len &&isspace(*str_tmp)  ){
        ++offset;
        ++ str_tmp;
    }
    memmove(str, str+offset, len +1-offset );
    return str;
}

char * trim( char * str)
{
    return (ltrim(rtrim(str)));
}



int cntDelim(const char * input , char delim)
{
    int retval=0;
    int cnt   =strlen(input);
    for( int i=0; i<cnt; i++)
    {
        if( input[i]==delim)
        ++retval;
    }
    return retval;
}

int cntPipes(const char * input)
{
    return cntDelim(input, '|');
}

int cntParams(const char * input)
{
    return cntDelim(input, ' ') +2;
}

bool isBackGroundProc(  char * input)
{
    bool retval = false;
    int len = strlen(input);
    if( input[len-1] =='&'  )
    {
        retval =true;
        input[len-1]='\0';
    }
    return retval;
}


void SplitString(
    char ** paramArray, 
    int cntPipes, 
    char* cmdLine)
{
    int arrsz = cntPipes+1;
    int cnr =0;
    char delim[] ="|";
    char * tkn = strtok(cmdLine, delim);
    while (tkn!=NULL)
    {
        strcpy( paramArray[cnr], tkn      );
        tkn = strtok(NULL, delim);
        cnr++;
    }
}

/*
    this splits the parametes 
    parameters are returned to paramArray
    if there is a redirect it is returned in the retval
    if retval > 0 this is the 

*/
int SplitPrms_OLD( char ** paramArray,   char* cmdLine   )
{
    int retval =0;
     
    int cnr =0;
    char delim[] =" ";

    
    char * tkn = strtok(cmdLine, delim);
    char * out;
    char *in ;
    while (tkn!=NULL)
    {
        printf("\n tkn - %s",tkn );
        if( tkn[0]=='>')
        {
            tkn = strtok(NULL, delim);
            printf("\n tkn1 - %s",tkn );
            if( tkn!=NULL)
            {
                retval =1;
                out = tkn;
            }
        }
        else if( tkn!=NULL && tkn[0]=='<')
        {
            tkn = strtok(NULL, delim);
            printf("\n tkn2 - %s",tkn );
            if( tkn!=NULL)
            {
                retval +=2;
                in = tkn;
                 
            }
        }else
        if( tkn!=NULL)
        {
            strcpy( paramArray[cnr], tkn      );
            printf("\n PRM %d %s ",cnr,paramArray[cnr] );
            cnr++;
        }
        tkn = strtok(NULL, delim);
        printf("\n tknLast - %s",tkn );
    }
    paramArray[cnr] =NULL;
    if( retval == 1 || retval == 3)
    {
        strcpy (paramArray[++cnr],out);
        retval= cnr;
    }else
    {
        retval =-1;
    }
    return retval;
}

char ** allocateArr( int numPipes, char** arrPipes, int itmSZ)
{
    int arrsz = numPipes +1;
    arrPipes = malloc(arrsz * sizeof(char *));
    int i;
    for(i =0;i<arrsz;++i)
    {
        arrPipes[i] = (char *) malloc(itmSZ* sizeof(char));
        memset(arrPipes[i], '\0', sizeof(arrPipes[i]));
    }
    return arrPipes;
}

bool hasInputRedir(const char * input)
{
    bool retval = false;
    int cnt = cntDelim(input, '<');
    if (cnt>0)
    {
        retval =true;
    }else
    {
        retval =false;
    }
    return retval; 
}

bool hasOutputRedir(const char * input)
{
    bool retval = false;
    int cnt = cntDelim(input, '>');
    if (cnt>0)
    {
        retval =true;
    }else
    {
        retval =false;
    }
    return retval; 
}




char* infileRedir( char * prmstring)
{   
    return getRedir(prmstring,'i');
}

char* outfileRedir( char * prmstring)
{   
    return getRedir(prmstring,'o');
}

char * getRedir( char * prmstring, char dir)
{
    char d1;
    char d2;
    switch (dir)
    {
        case 'i':
        {
            d1 ='<';
            d2 ='>';
        }
        break;
        case 'o':
        {
            d1 ='>';
            d2 ='<';
        }
        break;
        default:
        fprintf(stderr,"Invalid redir direction");
        exit(1);
    }
    char tmp[strlen( prmstring ) +1];
    strcpy(tmp,prmstring);
    char * tmpP =ltrim(rtrim( tmp ) );
    char * fst = strchr( tmpP , d1 );
    fst ++;
    fst = rtrim(ltrim( fst));
    int len = strlen(fst);
    int start =0;
    char * retval = malloc(sizeof(char) * len);
    while( start < len && fst[start]!= d2 && fst[start]!=' ' )
    {
        retval[start] = fst[start];
        ++start;
    }
    return retval;
}

bool isRedirDiv ( const char * divStr)
{
    bool retval =false;
    char *dup = strdup(divStr);
    dup = trim(dup);
    if( strlen(dup)>0)
    {
        retval =true;    
    }

    free (dup);
    return retval;
}



void prntPrmsArr(char ** items)
{
    int i=0;
    while ( items[i]!=NULL)
    {
        printf("\n%s\n", items[i++]);
    }
}



/*
    this splits the parametes 
    parameters are returned to paramArray
    Num params are returned

*/
int SplitPrms( char ** paramArray,   const char* cmdline   )
{
    int cnr =0;
    char * div ="<> ";
    char * cmdLine = strdup(cmdline);
    char * tkn = strtok(cmdLine, div);
    char   divstr[tkn-cmdLine+1];
    memset(&divstr[0], 0, sizeof(divstr));
    if( !isRedirDiv (divstr))
    {
        strcpy( paramArray[cnr++], tkn);
    }
    while (tkn!=NULL)
    {
        int from = tkn-cmdLine+strlen(tkn);
        tkn = strtok(NULL,div );
        int to = tkn != NULL ? tkn-cmdLine : strlen(cmdline);
        char   divstr[to-from+1];
        memset(&divstr[0], 0, sizeof(divstr));
        strncpy(divstr,  cmdline+from,(to-from)     );
        if( !isRedirDiv (divstr)&&tkn!=NULL)
        {
            strcpy( paramArray[cnr++], tkn);
        }
    }
    paramArray[cnr] =NULL;
    return cnr;
}






 