#include "thread_functions.h"

void* send_thread(void* arg) {
    // Potential Loop to keep the thread alive to send multiple messages
    while (1) {
        // Code to fetch and/or format data to send
        // ...

        // Call your send socket function
        socket_send( );

        // Optional: Implement delay or conditional wait here
        // ...
    }
    return NULL;
}

void* receive_thread(void* arg) {
    // Potential Loop to keep the thread alive for continuous listening
    while (1) {
        // Call your receive socket function
        socket_receive( /* Parameters as per your function */ );

        // Optional: Implement a mechanism to process or display the received data
        // ...
    }
    return NULL;
}
