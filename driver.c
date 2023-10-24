#include "thread_functions.h"
#include "socket_communications.h"
#include "LinkedList/list.h"
#include <pthread.h>

void freeItem(void* pItem){
    if(pItem != NULL){
        free(pItem);
    }
}

int main(int argc, char* argv[]){

    //make sure there are at least 4 arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [my port number] [remote machine name] [remote port number]\n", argv[0]);
        exit(1);
    }

    //input values into myPort and remotePort through the command-line
    int myPort = atoi(argv[1]);
    char* remoteMachineName = argv[2];
    int remotePort = atoi(argv[3]);

    //translate cpu_name (from command-line) to ip and store in remote_ip 
    char remote_ip[100];
    hostname_to_ip(remoteMachineName, remote_ip); //new ip address stored in remote_ip

    List* inputList = List_create();
    List* outputList = List_create();
    FREE_FN funcPointer = &freeItem;

    threads_init(myPort, remotePort, remote_ip, inputList, outputList);

    List_free(outputList, funcPointer);
    List_free(inputList, funcPointer);

    exit(0);
}
