#include <stdio.h>
#include <stdlib.h>
#include "receiver.h"
#include "write.h"
#include "send.h"
#include "read.h"
#include "list.h"

//142.58.15.219 

int main (int argc, char** argv)
{
    List* listReceive = List_create();
    List* listSend = List_create();
    char* myPort = argv[1];
    char* connectorHostname = argv[2];
    char* connectorPort = argv[3];

    // Startup my modules
       
    Read_init(listSend);
    Send_init(listSend, connectorHostname, connectorPort);
    Receiver_init(listReceive, myPort);
    Write_init(listReceive);

    // Wait for user input
    char x;
    scanf("%c", &x);

    // Shutdown my modules
     
    Read_shutdown();
    Send_waitForShutdown();
    Receiver_shutdown();
    Write_waitForShutdown();
   
    printf("Done\n");
    return 0;
}