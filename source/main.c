#include <stdio.h>
#include <stdlib.h>
#include "receiver.h"
#include "write.h"
#include "list.h"



int main (int argc, char** argv)
{
    List* listReceive = List_create();
    
    char* myPort = argv[1];
    // char* connectorHostname = argv[2];
    // char* connectorPort = argv[3];

    // Startup my modules
    Receiver_init(listReceive, myPort);
    Write_init(listReceive);

    // Wait for user input
    char x;
    scanf("%c", &x);

    // Shutdown my modules
    Receiver_shutdown();
    Write_waitForShutdown();

    printf("Done\n");
    return 0;
}