#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include "receiver.h"
#include "write.h"
#include "list.h"

#define DYNAMIC_LEN 128
#define MSG_MAX_LEN 1024

static pthread_t threadPID;
//static int socketDescriptor;
static int myPort;
static List* list;


void* receiveThread(void* unused)
{

   // Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                   // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
    sin.sin_port = htons(myPort);                 // Host to Network short

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // CHECK ERRORS
    if (socketDescriptor == -1) {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }
    // Bind the socket to the port (PORT) that we specify
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    while(1) {
        // Get the data (blocking)
        // Will change sin (the address) to be the address of the client
        // Note: sin passes information in and out of call!
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];
        int bytesRx = recvfrom(socketDescriptor,
            messageRx, MSG_MAX_LEN, 0,
            (struct sockaddr *) &sinRemote, &sin_len);

        // Make it null terminated (so string functions work):
        int terminateIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
        messageRx[terminateIdx] = 0;
               
        List_append(list, messageRx);
        Write_signalNextChar();

    
    }
    return NULL;
}

void Receiver_init(List* listR, char* myPortR)
{
    if (myPortR == NULL) {
        fprintf(stderr, "Error: Port number is not provided.\n");
        return;
    }

    list = listR;
    myPort = atoi(myPortR);

    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        receiveThread,      // Function
        NULL);              // Arguments
}


void Receiver_shutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);

    // Waits for thread to finish
    pthread_join(threadPID, NULL);


}