#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include "read.h"
#include "send.h"
#include "list.h"

#define DYNAMIC_LEN 128
#define MSG_MAX_LEN 1024

static pthread_t threadPID;
static List* list = NULL;


void *readThread(void *arg) {
    List* list = (List*)arg;
 
    while(1) {
        // Get the data (blocking)

        char messageTx[MSG_MAX_LEN];
        printf("Type something: ");
        scanf("%s", messageTx);
        
        List_append(list, messageTx);
        Send_signalNextChar();
        
        

    
    }
    return NULL;
}

void Read_init(List* listR)
{

    list = listR;
    pthread_create(&threadPID, NULL,readThread,(void*)listR);              
}


void Read_shutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);

    // Waits for thread to finish
    pthread_join(threadPID, NULL);


}