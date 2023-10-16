#include "thread_functions.h"
#include "socket_communications.h"
#include "LinkedList/list.h"

//read from user input and insert into list
void* keyboard_input_func(void* threadarg){

    //wrap threadarg with struct to pass in List
    pThreadD* args = (pThreadD*) threadarg;
    List* sendList = args->sharedList;
    
    char input[1024];//message buffer

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
//output thread
void* send_thread_func(void* threadarg) {
    //wrap
    pThreadD* args = (pThreadD*) threadarg;
    List* sendList = args->sharedList;
    const char* ip_address = args->ip_address;
    int port = args->port;
    char* message = args->message;
    
    while(1) { 
        //pick message from list
        List_last(sendList);//go to last item to retrieve latest message due to list_prepend
        message = List_remove(sendList);//remove last item in LL(latest message) and store latest in message

        // If the list is empty (message is NULL)
        if(message == NULL) {
            sleep(1);//wait before checking again (avoid busy-wait)
            continue;
        }

        //shoot message with socket_send
        socket_send(ip_address, port, message);
    }
    return NULL;
}

//receive message with receive_socket and insert the message into the list
void* receive_thread_func(void* threadarg) {
    pThreadD* args = (pThreadD*) threadarg;
    List* receiveList = args->sharedList;
    int port = args->port;
    char* message;
    while(1){
        //retreive message with receive_socket
        message = socket_receive(port);

        if(message){//if message exists
            char * storedMessage = strdup(message);//help protect against unexpected overwrites

            //insert the message into the list
            List_prepend(receiveList, storedMessage);

            //free original message
            free(message);
        }else{
            sleep(1);//avoid busy-wait
        }
    }
    return NULL;
}

//pick message from receive list and write onto the console
void* screen_output_func(void* threadarg){
    pThreadD* args = (pThreadD*) threadarg;
    List* receiveList = args->sharedList;
    char* message = args->message;

    while(1){
        //pick messages from list
        List_last(receiveList);
        message = List_remove(receiveList);

        if(message == NULL) {
            sleep(1);//wait before checking again (avoid busy-wait)
            continue;
        }
        //write on console
        printf("%s\n", message);
        free(message);
    }
    return NULL;
}