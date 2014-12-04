#define BUFFER 4096
#define DEFAULT_PORT 8081
#define WAITLIST 5
#define MAXSIZE 10
#define CHAINEINFIRMIERE "/INFIRMIERE"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

void handleConnection(int socket);
void error(const char *error);
int getMessageLength(char *buffer);
unsigned concatInts(unsigned a, unsigned b);


int main (int argc, char *argv[]){
    int socketFileDescriptor, newSocketFileDescriptor, portNumber;
    socklen_t clientAddressLength;
    int forkPID;
    int port = DEFAULT_PORT; //Default port

    //Create the structure containing the client and the server address
    /*
    struct sockaddr_in{
        short   sin_family; // AF_INET
        u_short sin_port; // Port
        struct  in_addr sin_addr;
        char    sin_zero[8]; // 00000000
    };
    */
    struct sockaddr_in serverAddress, clientAddress;
    if (argc < 2) {
        printf("[Info] No port provided, using default port (%d)\n", port);
    }else{
        port = atoi(argv[1]); // TODO Args
        printf("[Info] Using port %d \n", port);
    }
    portNumber = port;

    //Fill with 0's
    bzero((char *) &serverAddress, sizeof(serverAddress));

    //Fill struct
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0){
        error("[Error] Can't open socket");
    }

    //Bind socket to an address
    if (bind(socketFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
        error("[Error] Can't bind socket");
    }

    //Listen on the socket
    listen(socketFileDescriptor, WAITLIST);

    //Set client address length
    clientAddressLength = sizeof(clientAddress);

    //Accept connections on the created socket
    /*newSocketFileDescriptor = accept(socketFileDescriptor, (struct socketAddress *) &clientAddress, &clientAddressLength);
    if (newSocketFileDescriptor < 0){
        error("[Error] Can't accept connections on socket");
    }*/

    //Create the accept loop
    while (1) {
        newSocketFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *) &clientAddress, &clientAddressLength);
        //Client is connected
        if (newSocketFileDescriptor < 0){
            error("[Error] Can't accept connections on socket");
        }
        forkPID = fork();
        if (forkPID < 0){
            error("[Error] Can't create new process");
        }
        if (forkPID == 0)  {
            close(socketFileDescriptor);
            handleConnection(newSocketFileDescriptor);
            exit(0);
        }else{
            close(newSocketFileDescriptor);
        }
    }

    close(newSocketFileDescriptor);
    close(socketFileDescriptor);

    return 0;
}

void handleConnection(int socket){
    char buffer[BUFFER];
    int RWReturn;

    //Fill buffer with 0's
    bzero(buffer,BUFFER);

    //Read from the socket
    RWReturn = read(socket, buffer, BUFFER-1);
    int length = getMessageLength(buffer);
    if (RWReturn < 0){
        error("[Error] Can't read from socket (1)");
    }
    /*
    printf("Len: %d\n", length);
    if(length >= BUFFER){
        char finalString[length];
        int nbCut = length/BUFFER;
        strcpy(finalString, buffer);
        while(nbCut > 0){
            RWReturn = read(socket, buffer, BUFFER-1);
            if (RWReturn < 0){
                error("[Error] Can't read from socket (2)");
            }
            strcat(finalString, buffer);
        }
        printf("Received from client: %s\n", finalString);
    }else{
        //Print received message
        printf("Received from client: %s\n", buffer);
    }*/
    char chaineInfirmiere[] = CHAINEINFIRMIERE;
    if(strstr(buffer, chaineInfirmiere)!=NULL) {
        printf("Infirmiere\n");
    }
    else {
        printf("Communicate\n");
    }
    //Confirm message received
    RWReturn = write(socket, "ok", 2);
    if (RWReturn < 0){
        error("[Error] Can't write on socket");
    }
}

void error(const char *error){
    perror(error);
}

int getMessageLength(char *buffer){
    if(buffer[0] == '['){
        char *value = "";
        int i = 1;
        int temp;
        int closed = 0;
        int returnValue = 0;
        while(buffer[i] != ']' && i < MAXSIZE){
            temp = buffer[i];
            returnValue = ((returnValue*10)+temp)-48;
            i++;
        }
        if(buffer[i] != ']'){
            return -1;
        }
        if(returnValue == 0){
            return -1;
        }

        return returnValue+floor(log10(abs(returnValue))) + 3;
        //Get the message length + the [length string] length ([xxx] --> 5)
    }
    return -1;
}
