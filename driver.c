#include "thread_functions.h"
#include "socket_communications.h"
#include "constants.h"


int main(){
    pthread_t keyboard_thread, screen_thread, send_thread, receive_thread;
    int keyboardret, screenret, sendret, receiveret;

    pThreadD threadArgs;
    threadArgs.sharedList = List_create();

    //thread creation 
    keyboardret = pthread_create(&keyboard_thread, NULL, keyboard_input_func, &threadArgs);
    // screenret = pthread_create(&keyboard_thread, NULL, screen_output_func, );
    // sendret = pthread_create(&keyboard_thread, NULL, send_thread_func, );
    // receiveret = pthread_create(&keyboard_thread, NULL, receive_thread_func, );


    //joins
    pthread_join(keyboard_thread, NULL);
    pthread_join(screen_thread, NULL);
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    exit(0);
}
