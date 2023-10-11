#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> //For strncmp()
#include <unistd.h> // For close()

#define MSG_MAX_LEN 1024
#define PORT 22110
#define LOCAL_HOST "142.58.15.166"

int main(){
    printf("Net listen test on UDP port: %d\n\n", PORT);
    printf("Connect using: ");
    printf("netcat -u %s %d\n", LOCAL_HOST, PORT);

    printf("input something to send:");
    int send=0;
    scanf("%d", &send);

    struct sockaddr_in sin; //MUST spell sockaddr correctly
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT); 

    //Create UDP socket
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    //Bind socket to port specified
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

}