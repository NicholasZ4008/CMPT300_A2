#include "thread_functions.h"
#include "socket_communications.h"
#include "constants.h"


int main(){
    pthread_t keyboard_thread, screen_thread, send_thread, receive_thread;
    int keyboardret, screenret, sendret, receiveret;

    pThreadD threadArgs;

    keyboardret = pthread_create(&keyboard_thread, NULL, keyboard_input_func, );

    // screenret = pthread_create(&keyboard_thread, NULL, screen_output_func, );
    // sendret = pthread_create(&keyboard_thread, NULL, send_thread_func, );
    // receiveret = pthread_create(&keyboard_thread, NULL, receive_thread_func, );


    //Independent thread creation

}
