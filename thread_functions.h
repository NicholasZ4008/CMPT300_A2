#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"

void* send_thread(void* arg);

void* receive_thread(void* arg);

#endif

