#include "fonctions.h"

int main (int argc, char * argv[]){
	//Prog's name in argv[0]
	//First argument in argv[1]
	//Total number of arguments in argc 
	
	char * serverName;
	char * portStr;
	int port;
	
	//Test of the number of input args
	if (argc != 3) {
		printf(ERROR, argv[0]);
		exit(EXIT_FAILURE);
	}
	
	//Retrieve data from command-line arguments
	serverName = strtok(argv[1], ":");
	portStr = strtok(NULL, ":");
	
	if (portStr == NULL){
		portStr = malloc(3);
		strcpy(portStr, PORT);
		port = atoi(PORT);
	} else {
		port = atoi(portStr);
	}

 printf("Try on server : %s on port %d\n", serverName, port);

//printServerAddress(argv[1], argv[2]);

// Call the function to reserve a socket
char *serverAddress = argv[1];
char *fileName = argv[2];
int sfd;
int ffd;
int blockNumber =0;
struct addrinfo *result;
reserveSocket(argv[1], PORT, &sfd, &result);

// Step A: Send RRQ
sendRRQ(sfd, argv[2], 0, result);
//Step B
receiveAndAcknowledge(sfd, fileName, result);
//Step C
while (1) {
        int count = receiveOneBlock(sfd, ffd);  // Receive one data block

        if (count < MAX_BUF_RECEIVE) {
            // Last block received
            break;
        }

        // Send acknowledgment for the received block
        sendAck(sfd, blockNumber, result);
        blockNumber++;
    }

//Close socket
close(sfd);
printf("ok \n");

return 0;
}