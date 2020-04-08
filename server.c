#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<ctype.h>

//function prototypes
int getFileSize( char * fname, char c);
void error(const char *msg);
void ToUpperCase( char * fName);
void sendFileToClient( int clientDesc, char * fName);
int main(int argc, char *argv[])
{
   int sockfd, newsockfd, portno;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   if (argc < 2) 
   {
      fprintf(stderr,"ERROR, no port provided\n");
      exit(1);
   }
   fprintf(stdout, "Run client by providing host and port\n");
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
      error("ERROR opening socket");
      bzero((char *) &serv_addr, sizeof(serv_addr));
      portno = atoi(argv[1]);
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = INADDR_ANY;
      serv_addr.sin_port = htons(portno);
      if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
      listen(sockfd,5);
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd, 
         (struct sockaddr *) &cli_addr, 
         &clilen);
      if (newsockfd < 0) 
         error("ERROR on accept");
      bzero(buffer,256);
      n = read(newsockfd,buffer,255);
      while( n>9)
      {
         int blen = strlen(buffer);
         if( buffer[blen-1] == '\n' )
         {   
            buffer[blen-1] = 0;
         }

         printf("Here is the message: %s\n",buffer);
         if ( strncmp(buffer, "count",5)==0 )
         {
            char * cntFile = (buffer+8);
            printf("Fname = %s\n", cntFile);
            char cntChar = buffer[6];
            int numchars =getFileSize(cntFile,cntChar);
            if( numchars<0)
            {
               write(newsockfd,"Failed to open File",19);
            }else
            {
               FILE* fpw = fdopen(dup(newsockfd),"w");
               if(fpw==NULL)
               {
                  error("FAILED TO OPEN FD");
               }
               fprintf(fpw,"FILE %s has %d of %c",cntFile , numchars, cntChar  );
               printf("\nSent Count to client\n");
               fclose(fpw);
            }
         }else if ( strncmp(buffer, "toUpper",7)==0)
         {
            char * cntFile = (buffer+8);
            //ToUpperCase(cntFile);  commented out because we dont want to make the file uppercase on server
            sendFileToClient(newsockfd,cntFile);

         }else{
            n = write(newsockfd,"Unknown input",13);
         }
         bzero(buffer,256);
         n = read(newsockfd,buffer,255);
         if (n < 0) 
            error("ERROR writing to socket");
      }
      close(newsockfd);
      close(sockfd);
      return 0; 
   }
 
/*
   gets character count
   returns -1 on error
*/
int getFileSize( char * fname, char ch)
{
   int retval =0;
   FILE* fp   ;
   
   fp = fopen(fname,"r");
   if( fp ==NULL)
   {
      perror("FaliedToOpenFile");
      return -1;
   }
   char c;
   c=getc(fp);
   while(c!=EOF)
   {
      if( c==ch)
      {
         ++retval;
      }
      c=getc(fp);
   }
   fclose(fp);
   return retval;
}

/*
   Converts a file to uppercase
   this won't be used.  We will keep server side file lowercase
*/
void ToUpperCase( char * fName)
{
   FILE *fp = fopen(fName, "r+");
   if(fp ==NULL)
   {
      perror("Failed to open input file");
      return;
   }
   char c = getc(fp);
   while(c!=EOF)
   {
      c = toupper(c);
      fseek(fp,-1,SEEK_CUR);
      fputc(c,fp);c = getc(fp);   
   }
   fclose(fp);
}
/*sends a file to a filedescriptor
   for this application that will be a socket   
*/
void sendFileToClient( int clientDesc, char * fName)
{
   FILE *fp = fopen(fName, "r");
   FILE* fpw = fdopen(dup(clientDesc),"w");
   if(fp ==NULL)
   {
      error("Failed to open input file");
      return;
   }
   if(fpw ==NULL)
   {
      error("Failed to open output socket as FILE");
      return;
   }
   char c=getc(fp);
   while(c != EOF)
   {
      c = toupper(c);
      putc(c,fpw);
      c=getc(fp);
   }
   putc(EOF,fpw);
   printf("Sent file");
   fclose (fp);
   fclose (fpw);
}




/*fatal error notifies user of error and closes app*/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
