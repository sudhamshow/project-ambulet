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
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#define MAX_SIZE 	1004
#define BUFFERSIZE 	1500

struct packet{
	int interface;
        int packet_number;
	int bit_rate;
	uint64_t time_stamp;
        char data [MAX_SIZE];
}__attribute__((packed));

