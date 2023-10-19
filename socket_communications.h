// Ensure header is not included more than once
#ifndef SOCKET_COMMUNICATIONS_H
#define SOCKET_COMMUNICATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "LinkedList/list.h"
#include "constants.h"

//converts machine name to an ip address
//returns 0 on success, 1 if there's an error 
int hostname_to_ip(const char *hostname , char *ip);

// Function to send a message to a remote IP and port.
// Returns 0 on success, and -1 on failure.
int socket_send(const char* ip_address, int port, const char* message);

// Function to receive messages on a specified port(should be for host port).
// Returns 0 on success, and -1 on failure.
char* socket_receive(int port);

#endif //SOCKET_COMMUNICATIONS_H
