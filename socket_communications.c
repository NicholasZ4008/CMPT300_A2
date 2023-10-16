#include "socket_communications.h"

int socket_send(const char* ip_address, int port, const char* message) {
    int sendfd;
    struct sockaddr_in server_addr;
    
    // Create socket
    sendfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sendfd < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    // Send data
    int send_status = sendto(sendfd, message, strlen(message), 0, 
        (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (send_status < 0) {
        perror("Error sending message");
        close(sendfd);
        return -1;
    }

    // Close socket
    int close_status = close(sendfd);
    if (close_status < 0) {
        perror("Error closing socket");
        return -1;
    }

    return 0; // Indicating success
}

char* socket_receive(int port){
    printf("Net listen test on UDP port: %d\n\n", port);

    struct sockaddr_in sin; 
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port); 

    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0) {
        perror("Error creating socket");
        return NULL;
    }

    if (bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("Error binding socket");
        close(socketDescriptor);
        return NULL;
    }

    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);
    char messageRx[MSG_MAX_LEN]; 

    int bytesRx = recvfrom(
        socketDescriptor, 
        messageRx,
        MSG_MAX_LEN, 
        0, 
        (struct sockaddr*) &sinRemote, 
        &sin_len
    );

    close(socketDescriptor); // Close the socket once the message is received

    if (bytesRx < 0) {
        perror("Error receiving message");
        return NULL;
    }

    int terminateIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
    messageRx[terminateIdx] = 0;

    //must dynamically allocate for return msg so it doesn't get replaced 
    char* returnMsg = malloc(terminateIdx + 1);
    if (returnMsg == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    strncpy(returnMsg, messageRx, terminateIdx + 1); // copy received message to dynamically allocated memory
    return returnMsg;
}
