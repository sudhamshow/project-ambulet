#define _BSD_SOURCE
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
/********* CONSTANTS DECLARATION *******************/
#define DEBUG
#define MAX_SIZE 	1004	// SIZE OF THE PACKET
#define WINDOW_SIZE 	10	// SIZE FOR CALCULATING THE JITTER
#define BUFFER_SIZE	300	// SIZE FOR STORING THE PACKETS
#define BUFFER_SAFE	40	
#define PREVIOUS_WEIGHT 0.7	// WEIGHT FOR THE AVG OF LAST 10 PACKET DELAYS
#define PRESENT_WEIGHT 	0.3	// WEIGHT FOR THE PRESENT PACKET DELAY
#define PORT_NO		61000   // 55689// PORT ON WHICH SERVER RECIEVES THE DATA
#define MAX_INTERFACE   3 	// MAX NO OF CLIENTS SEND DATA TO THIS SERVER	
#define MAX_JITTER	200	// MAX WAIT TIME BEFORE DISCARDING THE PACKET
#define ECG_PORT	12345	// THE PORT ECG-PLOT APP IS RUNNING ON
/********* STRUCTURES DECLARATION ******************/

/** THIS IS THE PACKET STRUCTURE WHICH THE CLIENTS ARE SENDING DATA IN**/
struct packet {
	int interface;
	int packet_number;
	int bit_rate;
	uint64_t time_stamp;
	char data[MAX_SIZE];
}__attribute__((packed));

/**THIS STRUCTURE IS THE GLOBAL BUFFER WHICH CONTAINS THE PACKTES INSEQUENCE **/
struct global_buffer{
	struct packet in_seq_buffer[BUFFER_SIZE];
	int seq_flags[BUFFER_SIZE];
	int nxt_exp_pkt[BUFFER_SIZE];
	int most_recent_seq_no;
	unsigned long jitter;	
	unsigned long prev_time;
};

/**THIS STRUCTURE IS DEFINED AS AN ARRAY WHICH HOLDS THE INTERFACE PARAMETERS**/
struct packet_params {
	unsigned long jitter ;
	unsigned long curr_time;
	unsigned long prev_time;
	unsigned long curr_diff;
	unsigned long prev_diff[WINDOW_SIZE];
	int curr_pkt_no;
	int prev_pkt_no;
	int pkt_index;
};



/********* FUNCTIONS DECLARATION *******************/


void deliver_packet(int);

void *receive_packets_thread_function();

unsigned long get_current_time();

unsigned long jitter_computation(int);

struct packet_params reset_packet_parama(struct packet_params);

void *process_enque_thread_function();

void flush_all_flags();

void process_packet(struct packet *, unsigned long);

int drop_packet(struct packet*);

int discard_packet();
