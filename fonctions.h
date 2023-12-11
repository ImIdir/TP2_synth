#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ERROR "Usage : %s host:port file \n error... \n"
#define PORT 1069

void printServerAddress(char *serverName, char *portStr);
int reserveSocket(char *serverName, char *portStr);