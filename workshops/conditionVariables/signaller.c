#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "general.h"
#include "signaller.h"
#include "printer.h"

static const char* MESSAGE = "Hello world in Sync!";

pthread_t thread;

void* signallerThread(void* unused)
{
    for (const char* msg = MESSAGE; *msg != '\0'; msg++) {
        sleep_msec(500);

        // Signal other thread
        Printer_signalNextChar();
    }

    return NULL;
}

void Signaller_init()
{
    pthread_create(&thread, NULL, signallerThread, NULL);
}

void Signaller_waitForShutdown()
{
    pthread_join(thread, NULL);
}


