#ifndef _WRITE_H
#define _WRITE_H

#include <pthread.h>
#include "list.h"

void Write_init(List* listS);
void Write_signalNextChar();
void Write_waitForShutdown();

#endif