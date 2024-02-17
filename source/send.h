#ifndef _Send_H
#define _Send_H

#include <pthread.h>
#include "list.h"

void Send_init(List* listS, char* remoteIPR, char* remotePortR);
void Send_signalNextChar();
void Send_waitForShutdown();

#endif