#include "thread_functions.h"
#include "socket_communications.h"
#include "LinkedList/list.h"

//read from user input and insert into list
void* keyboard_input_func(void* threadarg){

    //wrap threadarg with struct to pass in List
    pThreadD* args = (pThreadD*) threadarg;
    List* sendList = args->sharedList;
    
    char input[1024]; // Example buffer size

    while(1) {
        printf("Type your message then press Enter\n");
        if(fgets(input, sizeof(input), stdin) != NULL) {
            if(strcmp(input, "\n") == 0) {//if nothing is typed and enter is pressed, break loop
                break;
            }
            List_prepend(sendList, input);//insert entered item into input
        } else {
            perror("Error reading input");
        }
    }
    return NULL;
}

//pick message from list and send that message with send_socket
void* send_thread_func(void* threadarg) {
    //wrap
    pThreadD* args = (pThreadD*) threadarg;
    List* sendList = args->sharedList;
    const char* ip_address = args->ip_address;
    int port = args->port;
    const char* message = args->message;
    
    while(1) { 
        socket_send(ip_address, port, message);
        sleep(1); // Prevents spamming messages non-stop
    }
    
    return NULL;
}

//receive message with receive_socket and insert the message into the list
void* receive_thread_func(void* threadarg) {
    int port = 22110; // Example port
    
    socket_receive(port);
    
    return NULL;
}

//pick message from receive list and write onto the console
void* screen_output_func(void* threadarg){

    return NULL;
}