#include "thread_functions.h"

pthread_mutex_t sendListMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receiveListMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inputListNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t outputListNotEmpty = PTHREAD_COND_INITIALIZER;

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

    printf("Listening on UDP port: %d\n\n", port);


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
}

//read from user input and insert into list
void* keyboard_input_func(void* threadarg){
    //wrap threadarg with struct to pass in List
    pThreadD* args = (pThreadD*) threadarg;
    List* sendList = args->sharedList;
    
    char input[1024];//message buffer

    while(1) {
        printf("Type your message then press enter: ");

        if(fgets(input, sizeof(input), stdin) != NULL) {            
            // if(strcmp(input, "\n") == 0) {//if nothing is typed and enter is pressed, break loop
            //     printf("Nothing typed\n");
            //     break;
            // }

            pthread_mutex_lock(&sendListMutex);
            List_prepend(sendList, input);//insert entered item into input
            pthread_cond_signal(&outputListNotEmpty);
            pthread_mutex_unlock(&sendListMutex);

        } 
        else {
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
    char* message;
    
    while(1) { 
        pthread_mutex_lock(&sendListMutex);
        //pick message from list

        while(List_count(sendList) == 0){
            pthread_cond_wait(&outputListNotEmpty, &sendListMutex);
        }

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

            if(strcmp(storedMessage, "!\n") == 0){
                free(message);
                free(storedMessage);
                threads_shutdown();
                break;
            }
            pthread_mutex_lock(&receiveListMutex);
            //insert the message into the list
            List_prepend(receiveList, storedMessage);
            pthread_cond_signal(&inputListNotEmpty);
            //free original message
            free(message);
            pthread_mutex_unlock(&receiveListMutex);
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
    char* message;

    while(1){
        pthread_mutex_lock(&receiveListMutex);

        while(List_count(receiveList) == 0){
            pthread_cond_wait(&inputListNotEmpty, &receiveListMutex);
        }
        
        //pick messages from list
        message = List_trim(receiveList);
        pthread_mutex_unlock(&receiveListMutex);

        if(message == NULL) {
            sleep(1);//wait before checking again (avoid busy-wait)
            continue;
        }
        
        //write on console
        printf("\nMessage received: %s", message);
        free(message);
    }
    return NULL;
}