/******************************************************************************
* COURSE:   CS372-400 W2018
* PROJECT:  Programming Assignment #1, client-server network application
* AUTHOR:   DREW WOLFE
* RUN:      make all
* RUN:      ./chatclient <server hostname> <port number>
* EXAMPLE:  ./chatclient flip3.engr.oregonstate.edu 52364 
* NOTES:
* USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
* The chatclient.c must connect to the same host as that of chatserver.py
* Initiate chatserver.py prior to starting chatclient.c
* Terminate chatclient.c with "\quit"
*
* REFERENCES:
* Sockets Tutorial
* http://www.linuxhowtos.org/C_C++/socket.htm
*
* Socket Programming in C/C++
* https://www.geeksforgeeks.org/socket-programming-cc/
*
* SOCKETS - SERVER & CLIENT - 2018 (less useful, but used)
* http://www.bogotobogo.com/cplusplus/sockets_server_client.php
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

int portNum; //port number
int sockfd; //file descriptor (straight from linuxhowtos.org)
int n; //read() and write() val
char message[501]; //to be sent
char buffer[489]; //mem for user's message
char handle[11]; //callsign
struct sockaddr_in serv_addr; //server address
struct hostent *server; //pointer to hostent struct

int d = 1; //loop exit criteria

void error(const char *); 
void InitialConnection(char *,int); 
void SendMessage();
void ReceiveMessage();

int main(int argc, char *argv[]) 
{

    // arg validation
    if (argc < 3) 
    {
       fprintf(stderr,"Should look like: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    printf("\e[1;1H\e[2J"); // tidy up

    bzero(handle,10);
    printf("type \"\\quit\" to quit chatclient\n\n");
    printf("Client handle: "); 
    fgets(handle,10,stdin); // gets client handle
    int v;
    for (v = 0; v < 11; v++) 
    {
        if (handle[v] == '\n') // remove newline
        {
            handle[v] = '\0';
        }
    }

    InitialConnection(argv[1],atoi(argv[2])); //connect w/ server

    printf("\e[1;1H\e[2J"); // tidy up
    printf("-Messages-\n");

    while (d == 1) 
    {
        if (d == 1) // if user has not typed \quit
        {
            SendMessage();
        }
        if (d == 1) 
        {
            ReceiveMessage();
        }
    }
    return 0;
}

/******************************************************************************
                             FUNCTION DEFINITIONS
******************************************************************************/

// Function establishes connection with server
void InitialConnection(char *hn,int pn) 
{
    portNum = pn;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket creation
    
    if (sockfd < 0) // socket validation
    {
        error("ERROR opening socket\n");
    }

    server = gethostbyname(hn);

    if (server == NULL) // host validation
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); // zeros out server address
    serv_addr.sin_family = AF_INET; // sets server address
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portNum); // port number to byte order

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    {
        error("ERROR connecting"); // server connection validation
    }
}

/*****************************************************************************/
// Function sends client message to server
void SendMessage() 
{
    printf("%s: ",handle); // displays handle
    bzero(buffer,489);
    fgets(buffer,488,stdin); // accepts client message

    int v;
    for (v = 0; v < 489; v++) 
    {
        if (buffer[v] == '\n') // remove newline
        {
            buffer[v] = '\0';
        }
    }

    if (buffer[0] == '\\' &&
        buffer[1] == 'q' &&
        buffer[2] == 'u' &&
        buffer[3] == 'i' &&
        buffer[4] == 't') 
    {
            d = 0;
            n = write(sockfd,buffer,strlen(buffer)); // also sends \quit to server

            if (n < 0) // write to socket validation
            {
                error("ERROR writing \"quit\" to socket"); 
            }
            close(sockfd);
    }
    else 
    {
        // setup for next message
        bzero(message,501); // zero out array for next message
        strcat(message,handle);
        strcat(message,": ");
        strcat(message,buffer);

        n = write(sockfd,message,strlen(message)); // send to server

        if (n < 0) // write to socket validation
        {
            error("ERROR writing message to socket");
        }
    }
}

/*****************************************************************************/
// Function receives/displays server's message
void ReceiveMessage() 
{
    bzero(message,501);

    n = read(sockfd,message,500); // read in server message

    if (n < 0) // read validation
    {
        error("ERROR reading from socket");
    }

    if (message[6] == '\\' &&
        message[7] == 'q' &&
        message[8] == 'u' &&
        message[9] == 'i' &&
        message[10] == 't') 
    {
            d = 0;
            close(sockfd);
    }
    else 
    {
        printf("%s\n",message); // if not \quit, write out message
    }
}

/*****************************************************************************/
// Function prints out errors and exits program
void error(const char *msg) 
{
    perror(msg);
    exit(0);
}