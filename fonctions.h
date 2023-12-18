#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define ERROR "Usage : %s host:port file \n Please try again... \n"
#define PORT "1069"
#define MAX_RRQ_SIZE 128
#define MAX_BUF_RECEIVE 516
#define MAX_BUF_SEND 2048

void printServerAddress(char *serverName, char *portStr);
void reserveSocket(char *serverName, char *portStr, int *sfd, struct addrinfo **result);
void sendRRQ(int sfd, char *fileName, int mode, struct addrinfo *result);
