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
		strcpy(portStr,"PORT");
		port = PORT;
	} else {
		port = atoi(portStr);
	}	
printf("Try on server : %s on port %d\n", serverName, port);

//Call of printServerAddress to display the server's address
printServerAddress(argv[1], argv[2]);
}