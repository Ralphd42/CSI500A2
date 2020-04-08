#ifndef __SHELLCMD_H__
#define __SHELLCMD_H__
#include   <unistd.h>
#include <sys/stat.h> 
#include <sys/wait.h>

#include   <fcntl.h>
#include "parse.h"

void redirect( char * outputFile);
int pipe_proc(int currFROMapp, int NumPipes,  char *apps[], int fd[2]);
void FileInput(const char * infile);
void pipeouput (int fd[2]);






#endif
