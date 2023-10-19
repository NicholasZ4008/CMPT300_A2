#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "../constants.h"
#include "../socket_communications.h"

int main(){
    printf("Listening on port 22111...\n");
    char* receivedMessage = socket_receive(22111);
    
    if (receivedMessage != NULL) {
        printf("\nReceived: %s\n", receivedMessage);
        free(receivedMessage);
    } else {
        printf("No message received or error occurred.\n");
    }
    return 0;
}
