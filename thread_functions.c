#include "thread_functions.h"

void* send_thread_func(void* arg) {
    const char* ip_address = "127.0.0.1"; // Example IP
    int port = 22110; // Example port
    const char* message = "Hello, Server!";
    
    while(1) { 
        socket_send(ip_address, port, message);
        sleep(1); // Prevents spamming messages non-stop
    }
    
    return NULL;
}

void* receive_thread_func(void* arg) {
    int port = 22110; // Example port
    
    socket_receive(port);
    
    return NULL;
}
