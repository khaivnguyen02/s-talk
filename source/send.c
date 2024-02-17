#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<netdb.h>
#include "send.h"
#include "list.h"
#include <arpa/inet.h>


#define DYNAMIC_LEN 128
#define MSG_MAX_LEN 1024
pthread_t threadPID;


static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static List* list;
static int remotePort;
static char* remoteIP;


void *SendThread(void *arg)
{
    List* list = (List*)arg;
  
    //bind port and send to the remote port
    struct sockaddr_in sinRemote;
    memset(&sinRemote, 0, sizeof(sinRemote));
    sinRemote.sin_family = AF_INET;                   // Connection may be from network
    sinRemote.sin_port = htons(remotePort);                 // Host to Network short

    //convert char string to IP address
    if (inet_pton(AF_INET, remoteIP, &sinRemote.sin_addr) <= 0) {
        perror("Error converting IP address");
    }  

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    // CHECK ERRORS
    if (socketDescriptor == -1) {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }
   
    char *messageTx = NULL;
    while (1) {
        
        // Lock the mutex before access the list
        pthread_mutex_lock(&s_syncOkToSendMutex);
        // Check if the list is empty
        while (list->count == 0 ) {
            // Wait until there are elements in the list
            pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex);
        }
        
        // Get the message from the list
        
        messageTx = List_trim(list);
        
        // Unlock the mutex
        pthread_mutex_unlock(&s_syncOkToSendMutex);
        unsigned int sin_len = sizeof(sinRemote);
        int bytesSent = sendto(socketDescriptor,
                 messageTx, 
                 (strlen(messageTx)+1), 
                 0, 
                 (struct sockaddr *)&sinRemote, 
                 sin_len);
        if (bytesSent == -1) {
            perror("Error sending message");
        }
     
    }
    // Free the message memory
    free(messageTx);

    return NULL;
}

void  Send_init(List* listS, char* remoteIPR, char* remotePortR)
{
    if(remoteIPR == NULL || remotePortR ==NULL)
    {
        fprintf(stderr,"Error: Port number is not provided\n");
    }

    list = listS;
    remoteIP = remoteIPR;
    remotePort = atoi(remotePortR);
    
    
    pthread_create(&threadPID, NULL, SendThread, (void*)listS);
}

void Send_signalNextChar()
{
    // Signal other thread
    pthread_mutex_lock(&s_syncOkToSendMutex);
    {
        pthread_cond_signal(&s_syncOkToSendCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToSendMutex);
}

void Send_waitForShutdown()
{
    pthread_join(threadPID, NULL);
}