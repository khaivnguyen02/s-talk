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
    char messageTx[MSG_MAX_LEN];
    while(1) {
        // Get the message
        
        printf("\nType something: ");
        scanf("%s", messageTx);

        // check if message is "!" then terminate the session
        // if (strcmp(messageTx, "!\n")==0)
        // {
        //     printf("ehe");
        //     break;
        // }

        //add message to the list
        List_append(list, messageTx);

        //signal send to send message
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