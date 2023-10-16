#include "socket_communications.h"

//converts hostname to an ip
//result is stored in ip
int hostname_to_ip(const char *hostname, char *ip)
{
    struct hostent *he;
    struct in_addr **addr_list;

    if ((he = gethostbyname(hostname)) == NULL) 
    {
        // gethostbyname failed
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }
    
    return 1;
}


//socket sends messages to specified ip address and port.
int socket_send(const char* dest_ip, int dest_port, const char* message) {
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
    server_addr.sin_port = htonl(dest_port);//make sure endian is same
    server_addr.sin_addr.s_addr = inet_addr(dest_ip);

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

//opens receive socket at host_port
char* socket_receive(int host_port){
    printf("Net listen test on UDP port: %d\n\n", host_port);

    struct sockaddr_in sin; 
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(host_port); 

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
