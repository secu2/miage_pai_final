#define BUFFER 256

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *error){
    perror(error);
}

int main(int argc, char *argv[])
{
    int socketFileDescriptor, portNumber, RWReturn;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    char buffer[BUFFER];

    //We need 2 arguments, hostname and port
    if (argc < 3) {
        fprintf(stderr,"[Usage] %s h port\n", argv[0]);
        exit(0);
    }

    //Retrieve server port from 2nd argument
    portNumber = atoi(argv[2]);
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0)
        error("[Error] Can't open socket");

    //Retrieve host name from 1st argument
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"[Error] Can't resolve host\n");
        exit(0);
    }

    //Fill with 0's
    bzero((char *) &serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);

    if (connect(socketFileDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0){
        error("[Error] Can't connect to the server");
    }

    //Get the message from the client
    printf("Type message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    //Write the message on the soccket
    RWReturn = write(socketFileDescriptor, buffer, strlen(buffer));
    if (RWReturn < 0){
        error("[Error] Can't write on the socket");
    }

    //Clear buffer
    bzero(buffer,256);
    RWReturn = read(socketFileDescriptor, buffer, 255);
    if (RWReturn < 0){
        error("[Error] Can't read from socket");
    }
    printf("%s\n", buffer);
    close(socketFileDescriptor);
    return 0;
}
