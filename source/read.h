#ifndef _read_H_
#define _read_H_
#include "list.h"

// Start background receive thread
void Read_init(List* myList);

// Stop background receive thread and cleanup
void Read_shutdown();

#endif