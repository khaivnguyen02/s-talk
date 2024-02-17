#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "write.h"
#include "list.h"

pthread_t threadWrite;


static pthread_mutex_t s_syncOkToWriteMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_syncOkToWriteCondVar = PTHREAD_COND_INITIALIZER;
static List* list;

void *WriteThread(void *arg)
{
    List* list = (List*)arg;
    char* MESSAGE;
     while (1) {
        // Lock the mutex before accessing the list
        pthread_mutex_lock(&s_syncOkToWriteMutex);
        
        // Check if the list is empty
        while (list->count == 0 ) {
            // Wait until there are elements in the list
            pthread_cond_wait(&s_syncOkToWriteCondVar, &s_syncOkToWriteMutex);
        }
        
        // Get the message from the list
        MESSAGE = List_trim(list);

        // Unlock the mutex
        pthread_mutex_unlock(&s_syncOkToWriteMutex);

        // Print the message character by character
        printf("Message receive: ");
        for (const char* msg = MESSAGE; *msg != '\0'; msg++) {
            printf("%c", *msg);
            fflush(stdout);
        }

    }
    // Free the message memory
    free((void*)MESSAGE);

    return NULL;
}

void  Write_init(List* listS)
{
    list = listS;
    pthread_create(&threadWrite, NULL, WriteThread, (void*)listS);
}

void Write_signalNextChar()
{
    // Signal other thread
    pthread_mutex_lock(&s_syncOkToWriteMutex);
    {
        pthread_cond_signal(&s_syncOkToWriteCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToWriteMutex);
}

void Write_waitForShutdown()
{
    pthread_join(threadWrite, NULL);
}