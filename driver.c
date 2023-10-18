#include "thread_functions.h"
#include "socket_communications.h"
#include "constants.h"
#include "LinkedList/list.h"


int main(int argc, char* argv[]){

    //make sure there are at least 4 arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [my port number] [remote machine name] [remote port number]\n", argv[0]);
        exit(1);
    }

    pthread_t keyboard_thread, screen_thread, send_thread, receive_thread;
    int keyboardret, screenret, sendret, receiveret;

    //input values into myPort and remotePort through the command-line

    int myPort = atoi(argv[1]);
    char* remoteMachineName = argv[2];
    int remotePort = atoi(argv[3]);

    //translate cpu_name (from command-line) to ip and store in remote_ip 
    char remote_ip[100];
    hostname_to_ip(remoteMachineName, remote_ip);

    //initializing output struct
    //Output shared between keyboard and send
    pThreadD outputArgs;
    outputArgs.sharedList = List_create();
    outputArgs.ip_address = remote_ip;
    outputArgs.port = remotePort;


    //initializing input struct
    //Input shared between screen and receive
    pThreadD inputArgs;
    inputArgs.sharedList = List_create();
    inputArgs.port = myPort;


    //"output" threads
    keyboardret = pthread_create(&keyboard_thread, NULL, keyboard_input_func, &outputArgs);
    sendret = pthread_create(&send_thread, NULL, send_thread_func, &outputArgs);

    //"input" threads
    screenret = pthread_create(&screen_thread, NULL, screen_output_func, &inputArgs);
    receiveret = pthread_create(&receive_thread, NULL, receive_thread_func, &inputArgs);


    //joins
    pthread_join(keyboard_thread, NULL);
    pthread_join(screen_thread, NULL);
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    exit(0);
}

