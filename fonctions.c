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

void receiveAndAcknowledge(int sfd, char *fileName, struct addrinfo *result) {
    int ffd;  // File descriptor for the received file
    int blockNumber = 1;  // Initial block number
    int lastBlockSize = 0;  // Size of the last block received

    // Open the file for writing
    ffd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
    if (ffd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // Receive one data block
    int count = receiveOneBlock(sfd, ffd);

    // Send acknowledgment for the received block
    sendAck(sfd, blockNumber, result);

    // Close the file
    close(ffd);

    printf("File '%s' received successfully.\n", fileName);
    printf("Last block size: %d bytes\n", count - 4);  // Adjust for the block header
}


void sendAck(int sfd, int blockNumber,struct addrinfo *result) {
    // Construct the ACK packet
    char ackPacket[4];
    memset(ackPacket, 0, sizeof(ackPacket));
    ackPacket[0] = 0;  // Opcode for ACK
    ackPacket[1] = 4;
    ackPacket[2] = (blockNumber >> 8) & 0xFF;  // High-order byte
    ackPacket[3] = blockNumber & 0xFF;         // Low-order byte

    // Get the first address from the server's address information
    struct addrinfo *ptr = result;
    if (ptr == NULL) {
        fprintf(stderr, "No address information available\n");
        exit(EXIT_FAILURE);
    }

    // Send the ACK packet to the server
    ssize_t sentBytes = sendto(sfd, ackPacket, sizeof(ackPacket), 0, ptr->ai_addr, ptr->ai_addrlen);
    if (sentBytes == -1) {
        perror("Error sending ACK");
        exit(EXIT_FAILURE);
    }
}

int receiveOneBlock(int sfd, int ffd) {
    int count;
    struct sockaddr_storage addr;
    socklen_t add_size = sizeof(addr);
    char *buf;
    static int blockNumber = 1;

    buf = calloc(MAX_BUF_RECEIVE, sizeof(char));

    count = recvfrom(sfd, buf, MAX_BUF_RECEIVE, 0, (struct sockaddr *)&addr, &add_size);

    if (count == -1) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    if (buf[0] != 0 || buf[1] != 3) {
        fprintf(stderr, "Error from server: %s\n", buf + 4);
        exit(EXIT_FAILURE);
    }

    free(buf);

    return count;
}




