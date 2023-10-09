#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> //For strncmp()
#include <unistd.h> // For close()

#define MSG_MAX_LEN 1024
#define PORT 22110
#define LOCAL_HOST "127.0.0.1"
int main(){
    printf("Net listen test on UDP port: %d\n\n", PORT);
    
    printf("Connect using: ");
    printf("netcat -u %s %d\n", LOCAL_HOST, PORT);

    struct sockaddr_in sin; //MUST spell sockaddr correctly
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT); 

    //Create UDP socket
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    //Bind socket to port specified
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    while(1){
        //Get data (blocking)
        //Note: Sin passes information in and out of call
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN]; // Initialize space for max message to receive

        int bytesRx = recvfrom(
            socketDescriptor, 
            messageRx,
            MSG_MAX_LEN, 
            0, 
            (struct  sockaddr*) &sinRemote, 
            &sin_len
        );

        //Make null terminated
        int terminateIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
        messageRx[terminateIdx] = 0;

        //Will print message received
        printf("%s\n", messageRx);

        /*Code below will take in an integer, then increment it
        Once the value is incremented it will send it back and print the message*/
        // printf("Message received (%d bytes): \n\n'%s'\n",bytesRx, messageRx);


        //Extract value from message:
        // int incMe = atoi(messageRx);

        // char messageTx[MSG_MAX_LEN];
        // sprintf(messageTx, "Math: %d + 1 = %d\n", incMe, incMe + 1);


        //Transmit reply
        // sin_len = sizeof(sinRemote);
        // sendto(
        //     socketDescriptor,
        //     messageTx,
        //     strlen(messageTx),
        //     0,
        //     (struct sockaddr*) &sinRemote,
        //     sin_len
        // );


    }



    return 0;
}