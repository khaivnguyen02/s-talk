#include <stdio.h>
#include <stdlib.h>
#include "receiver.h"

int main (int argc, char** args)
{
    printf("Starting...\n");

    // Startup my modules
    pthread_mutex_t sharedMutex = PTHREAD_MUTEX_INITIALIZER;
    Receiver_init("Sire! A message! ", &sharedMutex);

    // Wait for user input
    printf("Enter something to kill the receive thread\n");
    char x;
    scanf("%c", &x);

    // Shutdown my modules
    Receiver_shutdown();

    printf("Done\n");
    return 0;
}