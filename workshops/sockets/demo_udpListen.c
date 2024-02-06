/*
 * UDP Listening program on port 22110
 *
 * Usage:
 *  On the target, run this program (netListenTest).
 *  On the host:
 *      > netcat -u 192.168.0.171 22110
 *      (Change the IP address to your board)
 *
 *  On the host, type in a number and press enter:
 *      4<ENTER>
 *  
 *  On the target, you'will see a debug message:
 *      Message received (2 bytes):
 *      '4
 *      '
 *
 *  On the host, you'll see the message:
 *      Math: 4 + 1 = 5
 *
 */

#include <stdio.h>
#include <string.h>
#include <netdb.h>

#define MSG_MAX_LEN 1024
#define PORT        22110

int main(int argCount, char** args)
{
    // printf("Arguments (%d) are: \n", argCount);
    // for (int i = 0; i < argCount; i++) {
    //     printf("Arg %d: %s\n", args[i]);
    // }

    printf("Brian's Net Listen Test on UDP port %d:\n", PORT);
    printf("Connect using: \n");
    printf("    netcat -u 127.0.0.1 %d\n", PORT);

    // Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                   // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
    sin.sin_port = htons(PORT);                 // Host to Network short

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // CHECK ERRORS

    // Bind the socket to the port (PORT) that we specify
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    // CHECK RETURN VALUE

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
        // printf("Message received (%d bytes): \n\n'%s'", bytesRx, messageRx);
        long remotePort = ntohs(sinRemote.sin_port);
        printf("(Port %ld) %s", remotePort, messageRx);
        // printf("%s", messageRx);

        // // Extract the value from the message:
        // // (process the message any way your app requires).
        // int incMe = atoi(messageRx);

        // // Compose the reply message:
        // // (NOTE: watch for buffer overflows!).
        // char messageTx[MSG_MAX_LEN];
        // sprintf(messageTx, "Math: %d + 1 = %d\n", incMe, incMe + 1);

        // // Transmit a reply:
        // sin_len = sizeof(sinRemote);
        // sendto( socketDescriptor,
        //     message, strlen(messageTx),
        //     0,
        //     (struct sockaddr *) &sinRemote, sin_len);
    }
}