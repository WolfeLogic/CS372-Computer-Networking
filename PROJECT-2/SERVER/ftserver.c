/******************************************************************************
* COURSE:   CS372-400 W2018
* PROJECT:  Programming Assignment #2, client-server network application
* AUTHOR:   DREW WOLFE
* RUN:      make all
* RUN:      ./ftserver <PORT>
* EXAMPLE:  ./ftserver 52364 
* NOTES:
* USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
* The ftclient.py must connect to the same host as that of ftserver.c
* Initiate ftserver.c prior to starting ftclient.py
* Terminate ftserver.c with "CONTROL + C"
*
* REFERENCES: ftserver.c
* struct addrinfo{};
* http://beej.us/guide/bgnet/html/single/bgnet.html#structs
*
* getaddrinfo()
* https://www.wikiwand.com/en/Getaddrinfo (seriously)
* http://beej.us/guide/bgnet/html/single/bgnet.html#getaddrinfo
*
* struct dirent
* https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
* 
* open()
* http://codewiki.wikidot.com/c:system-calls:open
*
* killSpaces() | It's C++, but the principle of the new temp string, helped
* https://stackoverflow.com/questions/35301432/remove-extra-white-spaces-in-c
*
* Clear screen
* https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <dirent.h>

#define RED     "\x1b[5;31;40m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[1;33;40m"
#define BLUE    "\x1b[1;34;40m"
#define MAGENTA "\x1b[1;35;40m"
#define CYAN    "\x1b[1;36;40m"
#define WHITE   "\x1b[1;37;40m"
#define RESET   "\x1b[0m"
#define FYELLOW "\x1b[5;33;40m" 


int spinUp(char *);
int clientDataPortWizard(char *, char *); 
int dirList(char **); 
void fileTX(int, int, char *);
void directoryTX(int, int);
void manageRequest(int);
void connectionSessionMngr(int);
void killSpaces(char *); 
// void clearScreen();

int main(int argc, char** argv) 
{
    printf(RESET"\n \nCONTROL + C to exit program \n \n");

    if (argc < 2) //Input validation
    {
        printf("Looks like you may have forgotten a port number..."RED" (╯°□°）╯︵ ┻━┻ \n"RESET);
        printf("USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports) \n");
        printf("$ ./ftserver <PORT> \n");
        printf("Let's try that again "CYAN" ᕦ(ò_óˇ)ᕤ \n"RESET);
        exit(1);
    }
    if (argc > 2) 
    {
        printf("Sooooo ahhhh... Did you type more than one port number? Or something? "MAGENTA"(⊙.☉)7 \n"RESET);
        printf("USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports) \n");
        printf("$ ./ftserver <PORT> \n");
        printf("We're just going to use the first valid number "BLUE" (∩｀-´)⊃━☆ﾟ.*･｡ﾟ \n"RESET);
    }
  
    //Setup socket
    int fd = spinUp(argv[1]); //A file descriptor (fd) is a non-negative integer
    printf("Server listening on port"WHITE" %s\n"RESET, argv[1]);
    connectionSessionMngr(fd);

    return 0;
}

/******************************************************************************
                             FUNCTION DEFINITIONS
******************************************************************************/

/* Initates server socket to control communication and returns the control 
   file descriptor (fd) IFF socket creation, binding and listening succeed */
int spinUp(char *port) 
{
    //Borrowed heavily from "Beej's Guide to Network Programming"
    struct addrinfo hints, *res;
    int serverInfo; 

    memset(&hints, 0, sizeof hints);
    //"ai" for addrinfo struct (from beej)
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port, &hints, &res);

    //Socket setup
    int fd = socket(hints.ai_family, hints.ai_socktype, 0);
    if(fd == -1) 
    {
        printf("Socket failed! The port may not be available "RED"┻━┻ ︵ヽ(`Д´)ﾉ︵﻿ ┻━┻\n"RESET);
        exit(1);
    }
    //Bind to the socket using info from getaddrinfo, exit on failure
    serverInfo = bind(fd, res->ai_addr, res->ai_addrlen);
    if(serverInfo == -1) 
    {
        printf("Socket binding failed "BLUE"ಥ﹏ಥ\n"RESET);
        exit(1);
    }
    //Backlog of 5 http://beej.us/guide/bgnet/html/single/bgnet.html#listen
    serverInfo = listen(fd, 5); 
    if(serverInfo == -1) 
    {
        printf("Listen() call failed "RED"¿ⓧ_ⓧﮌ\n"RESET);
        exit(1);
    }
    return fd;
}

/* Handles Client data port connection and returns the file descriptor (fd) */
int clientDataPortWizard(char* ip, char* port) 
{
    //Borrowed heavily from "Beej's Guide to Network Programming"
    struct addrinfo hints, *res;
    int serverInfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    //DNS and service name lookups, and fills out the structs
    serverInfo = getaddrinfo(ip, port, &hints, &res); 

    //Socket setup
    int clientDataFD = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(clientDataFD == -1) 
    {
        printf(RED"Womp womp... "RESET"Client data port connection failed\n");
        exit(1);
    }
    //Client data port connection, returns file descriptor (fd)
    serverInfo = connect(clientDataFD, res->ai_addr, res->ai_addrlen); 
    return clientDataFD;
}

/* Lists the contents of the ftserver.c directory as per client request */
int dirList(char **fileList) 
{
    DIR *d;
    struct dirent *dir; //struct used to return information about directory entries
    d = opendir(".");
    int i = 0;
    if(d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
                fileList[i] = dir->d_name;
                i++;
        }
    }
    return i;
}

/* Transmits the requested file to the client */ 
void fileTX(int connectionFD, int dataFD, char* fileName) 
{
    //First, check if file exists
    int i = 0;
    int exists = 0;
    char *fileList[100];
    int fileCount = dirList(fileList);
    
    //Search within the dir for the file
    for(i = 0; i < fileCount; i++) 
    {
        if(strcmp(fileList[i], fileName) == 0) 
        {
            exists = 1;
        }
    }

    if(exists == 1) 
    {
        char fileBuff[3000];
        memset(fileBuff, 0, sizeof(fileBuff));
        int fileFD = open(fileName, O_RDONLY); //Reason fcntl.h is included
        while (1) 
        {
            int charsRead = read(fileFD, fileBuff, sizeof(fileBuff)-1);
            if (charsRead == 0)
                break;

            if (charsRead < 0) 
            {
                printf("Uh oh... There was a tiny problem reading the file "BLUE"(´･_･`)\n"RESET);
                return;
            }
            send(dataFD, fileBuff, sizeof(fileBuff) - 1, 0);
        }
    }
    else 
    {
        printf(BLUE"(∩｀-´)⊃━☆ﾟ.*･｡ﾟ "RESET"This is not the file you're looking for... Or the file doesn't exist\n");
    }
}

/* Transmits a list of the files contained within ftserver.c's directory */
void directoryTX(int connectionFD, int dataFD) 
{
    int i = 0;
    char *fileList[100];

    int fileCount = dirList(fileList);
    //Loops through directory according to fileCount
    for (i = 0; i < fileCount; i++)
    {
        send(dataFD, fileList[i], strlen(fileList[i]), 0);
        send(dataFD, "\n", strlen("\n"), 0);
    }
}

/* Manages request from client */
void manageRequest(int clientFD) 
{
    char clientIP[50]; //maximum IPv6 string length is 45 characters (and we're using IPv4)
    memset(clientIP, 0, sizeof(clientIP));
    char clientCommand[10]; //Can't imagine why it'd need to be longer ¯\_(ツ)_/¯
    memset(clientCommand, 0, sizeof(clientCommand));
    char clientDataPort[10]; //(2^16)-1, or 0-65,535 (-1 because port 0 is reserved and unavailable)
    memset(clientDataPort, 0, sizeof(clientDataPort));
    char requestedFilename[100];
    memset(requestedFilename, 0, sizeof(requestedFilename));

    recv(clientFD, clientIP, sizeof(clientIP)-1, 0);
    printf("Client's IP:"WHITE" %s\n"RESET, clientIP);
    
    recv(clientFD, clientCommand, sizeof(clientCommand)-1, 0);
    printf("Command from Client:"WHITE" %s\n"RESET, clientCommand);
    
    //Manages request for a file
    if(strstr(clientCommand, "-g")) 
    {
        recv(clientFD, requestedFilename, sizeof(requestedFilename)-1, 0);
        printf("Requested filename received:"WHITE" %s"RESET, requestedFilename);
        killSpaces(requestedFilename);
        strtok(requestedFilename, "\n");
    }

    recv(clientFD, clientDataPort, sizeof(clientDataPort)-1, 0);
    printf("Client's data port:"WHITE" %s\n \n"RESET, clientDataPort);
    sleep(2); //Client & server sync time
  
    killSpaces(clientIP);
    killSpaces(clientDataPort);

    //Client's data port connection
    int dataPortFD = clientDataPortWizard(clientIP, clientDataPort);
    
    if(strstr(clientCommand, "-l")) 
    {
        printf("Probably transmitting the directory list "MAGENTA"（ ^_^）o"FYELLOW"自自"RESET MAGENTA"o（^_^ ）\n \n"RESET);
        directoryTX(clientFD, dataPortFD);
    }
    else if(strstr(clientCommand, "-g")) 
    {
        printf("Good chance "WHITE"%s "RESET"is being sent now! "CYAN"(ᵔᴥᵔ)و\n \n"RESET, requestedFilename);
        fileTX(clientFD, dataPortFD, requestedFilename);
    }
    else 
    {
        printf("We need a valid command"RED" ¯(_(⊙︿⊙)_)¯\n"RESET);
    }
    close(dataPortFD);
}

/* Manages the session between CNX open to CNX close, including idle time */
void connectionSessionMngr(int fd) 
{
    int clientFD;
    struct sockaddr_storage client_addr;
    socklen_t addr_len;

    //Loop until a client connects
    while(1) 
    {
        //Establish control connection 
        addr_len = sizeof(client_addr);
        clientFD = accept(fd, (struct sockaddr *)&client_addr, &addr_len);
        if(clientFD == -1) 
        {
            continue;
        }
        printf("Client connection initated"BLUE" ʕᵔᴥᵔʔ\n \n\n"RESET);
        manageRequest(clientFD);
        close(clientFD);
        sleep(2);
        // clearScreen();
        printf("\nConnection closed "YELLOW"ヽ(´▽｀)ノ"RESET"\n \n"RESET);
    }
}

/* Removes (kills) extra spaces within buffers. After thought */
void killSpaces(char *my_string) 
{
    char *i = my_string;
    char *j = my_string;
    while(*j != 0) 
    {
        *i = *j++;
        if(*i != ' ') 
        {
            i++;
        }
    }
    *i = 0;
}

// void clearScreen()
// {
//   const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
//   write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
// }
