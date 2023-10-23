#include "thread_functions.h"

pthread_mutex_t sendListMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receriveListMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_t screen_thread, receive_thread, keyboard_thread, send_thread;

void threads_init(int port, int remotePort, char* remoteIp, List* inputList, List* outputList){
    int screenret, receiveret;
    int keyboardret, sendret;

    //initializing input struct
    //Input shared between screen and receive
    pThreadD inputArgs;
    inputArgs.sharedList = inputList;
    inputArgs.port = port;

    //initializing output struct
    //Output shared between keyboard and send
    pThreadD outputArgs;
    outputArgs.sharedList = outputList;
    outputArgs.ip_address = remoteIp;
    outputArgs.port = remotePort;

    //"input" threads
    screenret = pthread_create(&screen_thread, NULL, screen_output_func, &inputArgs);
    receiveret = pthread_create(&receive_thread, NULL, receive_thread_func, &inputArgs);

    //"output" threads
    keyboardret = pthread_create(&keyboard_thread, NULL, keyboard_input_func, &outputArgs);
    sendret = pthread_create(&send_thread, NULL, send_thread_func, &outputArgs);
    
    // //Let threads finish their last task
    pthread_join(screen_thread, NULL);
    pthread_join(receive_thread, NULL);
    pthread_join(keyboard_thread, NULL);
    pthread_join(send_thread, NULL);
}

void threads_shutdown(){
    //Make threads stop working
    pthread_cancel(screen_thread);
    pthread_cancel(receive_thread);
    pthread_cancel(keyboard_thread);
    pthread_cancel(send_thread);
    printf("\nThreads have been shutdown.\n");
}

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
                printf("Nothing typed\n");
                break;
            }
            // if(strcmp(input, "!\n") == 0){
            //     printf("Exiting program\n");
            //     pthread_mutex_lock(&sendListMutex);
            //     void* empty;
            //     pthread_mutex_unlock(&sendListMutex);
            //     threads_shutdown();
            //     break;
            // }
            pthread_mutex_lock(&sendListMutex);
            List_prepend(sendList, input);//insert entered item into input
            pthread_mutex_unlock(&sendListMutex);

        } 
        else {
            perror("Error reading input");
        }
    }
    printf("Exit keyboard thread");
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
    char* message;
    
    while(1) { 
        pthread_mutex_lock(&sendListMutex);
        //pick message from list
        // List_first(sendList);//go to last item to retrieve latest message due to list_prepend
        // message = List_remove(sendList);//remove last item in LL(latest message) and store latest in message
        message = List_trim(sendList);
        pthread_mutex_unlock(&sendListMutex);
        // If the list is empty (message is NULL)
        if(message == NULL) {
            sleep(1);//wait before checking again (avoid busy-wait)
            continue;
        }

        if(strcmp(message, "!\n") == 0){
            socket_send(ip_address, port, message);
            threads_shutdown();
            break;
        }
        // printf("Message: %s\n", message);
        // printf("Sending message \"%s \"to %s:%d\n",message, ip_address, port);
        //shoot message with socket_send
        socket_send(ip_address, port, message);
    }
    printf("Exit send thread\n");
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
            pthread_mutex_lock(&receriveListMutex);
            char * storedMessage = strdup(message);//help protect against unexpected overwrites

            // printf("Received message\n");
            if(strcmp(storedMessage, "!\n") == 0){
                
                threads_shutdown();
                break;
            }
            //insert the message into the list
            List_prepend(receiveList, storedMessage);
            //free original message
            free(message);
            pthread_mutex_unlock(&receriveListMutex);
        }else{
            sleep(1);//avoid busy-wait
        }
    }
    printf("exit recv thread\n");
    return NULL;
}

//pick message from receive list and write onto the console
void* screen_output_func(void* threadarg){
    pThreadD* args = (pThreadD*) threadarg;
    List* receiveList = args->sharedList;
    char* message;

    while(1){
        pthread_mutex_lock(&receriveListMutex);
        //pick messages from list
        // List_last(receiveList);
        // message = List_remove(receiveList);
        message = List_trim(receiveList);
        pthread_mutex_unlock(&receriveListMutex);

        if(message == NULL) {
            sleep(1);//wait before checking again (avoid busy-wait)
            continue;
        }
        
        //write on console
        printf("%s\n", message);
        free(message);
    }
    printf("Exit screen output\n");
    return NULL;
}