/*
 simple client to work with server.c program.
 * Host name and port used by server is to be
 * passed as arguments.
 *
 * To test: Open a terminal window.
 * At prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $./client 127.0.0.1 54554
 * Please enter the message: Operating Systems is fun!  
 * I got your message
 * $ 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
const char * UpOut  = "fileUpper.txt";
const char * ChrOut = "fileChar.txt" ;

void error(const char *msg);
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)     &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
   // fgets(buffer,255,stdin);
    int readlen =0;
    char * inBuff ;
    size_t bufflen =0;
    readlen=getline(&inBuff,&bufflen,stdin);
    while( readlen>0)
    {
        n = write(sockfd,inBuff,strlen(inBuff));
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }
        if ( strncmp(inBuff, "toUpper",7)==0)
        {
           /* char * cntFile = (inBuff+8);*/
            FILE * sockRead = fdopen(dup(sockfd),"r+");
            if( sockRead==NULL)
            {
                error("Failed to read from socket");
            }
            FILE * outfile;
            outfile = fopen( UpOut,"w");
            if(outfile==NULL)
            {
                error("Failed to open outputfile");
            }
            char c = getc(sockRead);
            while(c!=EOF)
            {
                fputc(c,outfile);
                c = getc(sockRead);
            }
            printf("File Created" );
            fflush(stdout);
            fclose(outfile);
            fclose( sockRead);
        }

        else if ( strncmp(inBuff, "count",5)==0)
        {
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            if (n < 0) 
                error("ERROR reading from socket");
            printf("%s\n",buffer);
            //write buffer to a file
            /*char * cntFile=(inBuff+8);
            int blen = strlen(cntFile);
            if( cntFile[blen-1] == '\n' )
            {   
                cntFile[blen-1] = 0;
            }*/
            FILE *cntOut;
            cntOut = fopen( ChrOut,"w");
            if(cntOut==NULL)
            {
                error("Failed to open outputfile");
            }
            fprintf( cntOut, "%s",buffer);
            fclose(cntOut);
        }else
        {
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            if (n < 0) 
                error("ERROR reading from socket");
            printf("%s\n",buffer);
        }
        printf("Please enter the message: ");
        fflush(stdout);
        readlen=getline(&inBuff,&bufflen,stdin);
    }
    close(sockfd);
    return 0;
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}