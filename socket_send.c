#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 22110
#define MSG_MAX_LEN 1024
#define LOCAL_HOST "127.0.0.1"

int main() {
    int sendfd;
    struct sockaddr_in server_addr;
    char buffer[MSG_MAX_LEN] = "Hello, Server!";//this will be sent

    // Create socket
    sendfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Define server address (in this case, localhost)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send data
    sendto(sendfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Message sent to server.\n");

    close(sendfd);
    return 0;
}
