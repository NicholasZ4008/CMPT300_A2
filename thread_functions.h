#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
#include "LinkedList/list.h"
#include "socket_communications.h"
typedef struct ThreadInfo_S pThreadD;
struct ThreadInfo_S{
    List* sharedList;
    const char* ip_address;
    int port;
};

void threads_shutdown();

void threads_init(int port, int remotePort, char* remoteIp, List* inputList, List* outputList);

void* keyboard_input_func(void* threadarg);

void* send_thread_func(void* threadarg);

void* receive_thread_func(void* threadarg);

void* screen_output_func(void* threadarg);

#endif

