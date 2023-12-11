#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define ERROR "Usage : %s host:port file \n error... \n"
#define PORT 1069