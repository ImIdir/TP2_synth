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

void reserveSocket(char *serverName, char *portStr, int *sfd, struct addrinfo **result) {
    int status;
    struct addrinfo *ptr;
    struct addrinfo hints;

    // Initialize hints to zero
    memset(&hints, 0, sizeof(struct addrinfo));

    // Set the desired socket characteristics
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; // Use Datagram sockets
    hints.ai_protocol = IPPROTO_UDP; // Use UDP protocol

    // Obtain address information for the TFTP server
    status = getaddrinfo(serverName, portStr, &hints, result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    ptr = *result;

    // Iterate over the address information to create a socket
    while (ptr != NULL) {
        *sfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (*sfd != -1) {
            break;  // Break out of the loop once a valid socket is created
        }
        ptr = ptr->ai_next;
    }
}

void sendRRQ(int sfd, char *fileName, int mode, struct addrinfo *result) {
    ssize_t sentBytes;

    // Construct the RRQ packet
    char rrq[MAX_RRQ_SIZE];
    memset(rrq, 0, MAX_RRQ_SIZE);
    rrq[0] = 0;  // Opcode for RRQ
    rrq[1] = 1;
    strcpy(rrq + 2, fileName);
    rrq[2 + strlen(fileName)] = 0;  // Add the first null terminator
    strcpy(rrq + 3 + strlen(fileName), "octet");
    rrq[3 + strlen(fileName) + strlen("octet")] = 0;  // Add the second null terminator

    // Get the first address from the server's address information
    struct addrinfo *ptr = result;
    if (ptr == NULL) {
        fprintf(stderr, "No address information available\n");
        exit(EXIT_FAILURE);
    }

    // Send the RRQ packet to the server
    sentBytes = sendto(sfd, rrq, 4+strlen(fileName)+strlen("octet"), 0, ptr->ai_addr, ptr->ai_addrlen);
    if (sentBytes == -1) {
        perror("Error sending RRQ");
        exit(EXIT_FAILURE);
    }
}
