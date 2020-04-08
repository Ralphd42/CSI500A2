#ifndef __PARSE_H__
#define __PARSE_H__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include    <ctype.h>

#include <stdbool.h>
#define cmdsize 100
#define prmsz    10
char * ltrim( char * str );
char * rtrim( char * str );
char * trim ( char * str );
int cntPipes( const char * input);
int cntDelim(const char * input , char delim);
int cntParams(const char * input);
bool isBackGroundProc(   char * input);
char ** allocateArr( int numPipes, char** arrPipes, int itmesz);
void SplitString(char ** paramArray, int cntPipes, char* cmdLine);
int SplitPrms(
    char ** paramArray, 
      
   const char* cmdLine );
char* infileRedir( char * prmstring);
bool hasInputRedir(const char * input);
bool  hasOutputRedir(const char * input);
char* outfileRedir( char * prmstring);
bool isRedirDiv ( const char * divStr);
char * getRedir( char * prmstring, char dir);




#endif