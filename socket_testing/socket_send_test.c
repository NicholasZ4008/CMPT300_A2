#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "../constants.h"
#include "../socket_communications.h"

int main(){
    char message[MSG_MAX_LEN] = "Hi Server";
    printf("Sending message to %s:%d...\n", "142.58.15.166", 22110);
    
    int status = socket_send("142.58.15.166", 22110, message);
    
    if (status == 0) {
        printf("Message Sent successfully!\n");
    } else {
        printf("Failed to send message.\n");
    }
    return 0;
}
