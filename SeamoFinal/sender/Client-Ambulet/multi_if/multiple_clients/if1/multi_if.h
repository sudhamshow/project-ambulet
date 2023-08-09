#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_SIZE 	1016
#define INTERFACE_NAME 	4
#define BUFFER_SIZE 	20

int BitRateToDelay (int);

struct packet{
        int interface;
        int packet_number;
        char data [MAX_SIZE];
};

