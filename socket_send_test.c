#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> //For strncmp()
#include <unistd.h> // For close()
#include "constants.h"
#include "socket_communications.h"

int main(){
    char message[MSG_MAX_LEN] = "Hi Server";

    socket_send(LOCAL_HOST,PORT,message);

    printf("\nMessage Sent\n");
}