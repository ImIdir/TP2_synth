#include "fonctions.h"

void printServerAddress(char *serverName, char *portStr) {
    int status;
    struct addrinfo *ptr;

    struct addrinfo hints;
    struct addrinfo *result;

    // Initialize hints to zero
    memset(&hints, 0, sizeof(struct addrinfo));

    // Set the desired socket characteristics
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; // Use Datagram sockets
    hints.ai_protocol = IPPROTO_UDP; // Use UDP protocol

    // Obtain address information for the TFTP server
    status = getaddrinfo(serverName, portStr, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    ptr = result;

    // Iterate over the address information to print the server's address
    while (ptr != NULL) {
        // Print server's address
        char address[INET6_ADDRSTRLEN];
        void *addr;
        if (ptr->ai_family == AF_INET) {
            addr = &((struct sockaddr_in *)ptr->ai_addr)->sin_addr;
        } else {
            addr = &((struct sockaddr_in6 *)ptr->ai_addr)->sin6_addr;
        }
        inet_ntop(ptr->ai_family, addr, address, sizeof(address));
        printf("Server address: %s\n", address);

        ptr = ptr->ai_next;
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(result);
}