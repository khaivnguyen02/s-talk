#ifndef _RECEIVER_H_
#define _RECEIVER_H_
#include "list.h"

// Start background receive thread
void Receiver_init(List* myList, char* myPort);

// Stop background receive thread and cleanup
void Receiver_shutdown(void);

#endif