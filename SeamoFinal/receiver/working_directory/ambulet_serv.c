#include "serv.h"


/*********** GLOBALS DECLARATION ************/
/** The interface_params array contains the previous and current    **
 ** environments of the packet recieved from each interface         **/

struct packet_params interface_params[MAX_INTERFACE];
struct global_buffer seq_params;
struct timeval tv;
int count[MAX_INTERFACE]={0};
static unsigned long sleep_time;
static int start_deliver=0;
static int send_sock;
int glob_index = 0;
FILE *fptr[3];
/**   The main function creates two threads one for receiving the   **
 **   packets and putting them in a circular buffer and other for   **
 **   sending the sequenced packets to the consumer.	   	    **/

int main()
{
	pthread_t recv_pkts, process_enq;
	int iResult;


	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	iResult =
	    pthread_create(&recv_pkts, NULL,
			   (void *)receive_packets_thread_function, NULL);
	if (iResult != 0) {
		printf("recv pkts thread not created : %s \n", strerror(errno));
	}

	iResult =
	    pthread_create(&process_enq, NULL,
			   (void *)process_enque_thread_function, NULL);
	if (iResult != 0) {
		printf("process enque thread not created : %s \n",
		       strerror(errno));
	}

	fptr[0] = fopen("../timediff0.txt","a");
        if (fptr[0] == NULL){
        printf("Error");
        exit(1);
        }

	fptr[1] = fopen("../timediff1.txt","a");
        if (fptr[1] == NULL){
        printf("Error");
        exit(1);
        }

	fptr[2] = fopen("../timediff2.txt","a");
        if (fptr[2] == NULL){
        printf("Error");
        exit(1);
        }
	seq_params.prev_time=0;	
	seq_params.jitter=50000;//
	pthread_join(process_enq, NULL);
	exit(0);
}




/**	receive_packets_thread_function continiously recieves the       **
 **	packets, notes the time when packets have arrived and		**
 **	the global variables with current time and packet number and   	**
 **	calls the jitter_computation which updates the jitter values.   **/

void *receive_packets_thread_function()
{
	int recv_sock, iResult,bitrate;
	char recvd_pkt[MAX_SIZE];
	socklen_t client_addr_len;
	struct packet *recvd;
	struct sockaddr_in client_addr, src_addr;
	unsigned long present_time;
	
	bzero(&seq_params, sizeof(struct global_buffer));
	bzero(&interface_params, MAX_INTERFACE * sizeof(struct packet_params));

	for (int a=0;a<BUFFER_SIZE;a++){seq_params.nxt_exp_pkt[a]=a;}
	for(int i=0; i<MAX_INTERFACE; i++){interface_params[i].jitter=50000;}
	recv_sock = socket(AF_INET, SOCK_DGRAM, 0);

	printf("socket created %d\n", recv_sock);

	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	client_addr.sin_port = htons(PORT_NO);

	client_addr_len = sizeof(struct sockaddr_in);

	iResult =
	    bind(recv_sock, (struct sockaddr *)&client_addr,
		 sizeof(client_addr));
	if (iResult == -1) {
		printf("bind error1 : %s \n", strerror(errno));
	} else
		printf("bind successful to INADDR_ANY\n");
	
	while (1) {

		iResult =
		    recvfrom(recv_sock, recvd_pkt, sizeof(struct packet), 0,
			     (struct sockaddr *)&src_addr, &client_addr_len);
		if (iResult == -1) {
			printf("recv from error: %s \n", strerror(errno));
		}

		present_time = get_current_time();

		recvd = (struct packet *)recvd_pkt;

                printf("packet recvd packet no: %d interface:%d at %lu\n",recvd -> packet_number,recvd->interface,present_time);
		if(bitrate != recvd->bit_rate){bitrate=recvd->bit_rate; sleep_time= (sizeof(struct packet)*8000)/bitrate -120;}
		process_packet(recvd, present_time);
	}
}

unsigned long get_current_time(){
	unsigned long present_time;
	gettimeofday(&tv, NULL); 
	present_time = 1000000 * tv.tv_sec + tv.tv_usec;
	return present_time;
	}	

unsigned long jitter_computation(int if_index){
	static int x;
	unsigned long average = 0, jitter;
	count[if_index]++;
	count[if_index] = (count[if_index] % WINDOW_SIZE)==0 ? WINDOW_SIZE : count[if_index] % WINDOW_SIZE ;
	if (x < WINDOW_SIZE ){x = count[if_index];}	
	for (int i = 0; i < x; i++) {
		average = average + interface_params[if_index].prev_diff[i];
	}

	average = average / x;
	jitter = (unsigned long) (0.3 * average) + (0.7 * interface_params[if_index].curr_diff);
	
	return (jitter);

}

	/* This is where packet is processed and stored. Jitter value is recorded w.r.t last packet received and 
   	 * the average Jitter is computed for the interface the packet is recvd on. The packet is then delivered to the
	 * respective slot in the global buffer or discarded if duplicate or late
	 */  

void process_packet(struct packet *recvd_pkt, unsigned long current_time){

	int if_idx = recvd_pkt->interface, buff_index = 0; 
 //     printf ("processing packet %d from interface %d\n",recvd_pkt->packet_number, if_idx);

	interface_params[if_idx].curr_time = current_time;
	interface_params[if_idx].curr_pkt_no = recvd_pkt->packet_number;
	interface_params[if_idx].curr_diff = interface_params[if_idx].curr_time - interface_params[if_idx].prev_time;
	if(interface_params[if_idx].curr_diff > recvd_pkt->time_stamp){interface_params[if_idx].curr_diff=50000;}


	interface_params[if_idx].jitter = jitter_computation(if_idx);

	fprintf(fptr[if_idx],"%lu\n",(interface_params[if_idx].jitter));
	fflush(fptr[if_idx]);

	interface_params[if_idx].prev_time = interface_params[if_idx].curr_time;
	interface_params[if_idx].pkt_index = (interface_params[if_idx].curr_pkt_no) % WINDOW_SIZE;
	interface_params[if_idx].prev_diff[count[if_idx] - 1] = interface_params[if_idx].curr_diff;
//      interface_params[if_idx].prev_diff[interface_params[if_idx].pkt_index]  =  interface_params[if_idx].curr_diff;

//	div_index = (interface_params[if_idx].curr_pkt_no / BUFFER_SIZE);
        buff_index = interface_params[if_idx].curr_pkt_no % BUFFER_SIZE;

        printf(" flag: %d next exp pkt %d\n",seq_params.seq_flags[buff_index],seq_params.nxt_exp_pkt[buff_index]);

	/**  Check if the arrived packet is already put in the buffer if    **
	 **  not then check for the packet number and then put the arrived  **
	 **  packet in the global buffer set the flag and  		    **
	 ** 'Next Expected Packet'                                          **/

	if (seq_params.seq_flags[buff_index] != 1 && (interface_params[if_idx].curr_pkt_no >= seq_params.nxt_exp_pkt[buff_index])
			&& (interface_params[if_idx].curr_pkt_no >= glob_index)) {
		if (seq_params.seq_flags[buff_index] == 2) { if(interface_params[if_idx].curr_pkt_no == seq_params.nxt_exp_pkt[buff_index]){
							  	printf ("dropping timed out packet\n");
								seq_params.nxt_exp_pkt[buff_index] += BUFFER_SIZE;	
							    	drop_packet(recvd_pkt);
							    	seq_params.seq_flags[buff_index]=0;	
								}
							     else {seq_params.nxt_exp_pkt[buff_index]=recvd_pkt->packet_number;goto SEND;}	
		} else { 
		    SEND:memcpy(&seq_params.in_seq_buffer[buff_index],recvd_pkt, sizeof(struct packet));
			if (seq_params.most_recent_seq_no < recvd_pkt->packet_number)
			 {seq_params.most_recent_seq_no = recvd_pkt->packet_number;}
			 seq_params.seq_flags[buff_index] = 1;
 	                 seq_params.nxt_exp_pkt[buff_index]=recvd_pkt->packet_number;
			 seq_params.nxt_exp_pkt[buff_index] += BUFFER_SIZE;	
			 printf  (" packet no %d from interface %d put in index no : %d flag:%d\n",
			     seq_params.in_seq_buffer[buff_index].packet_number,
			     seq_params.in_seq_buffer[buff_index].interface,
			     buff_index,seq_params.seq_flags[buff_index]);
			} 
	} else {
               printf("dropping duplicate packet\n");
              	drop_packet(recvd_pkt);
	}
	if(abs(seq_params.most_recent_seq_no - glob_index) >= BUFFER_SAFE) {start_deliver=1;}
	printf("start to deliver: %d \n",start_deliver);
	interface_params[if_idx].prev_pkt_no = interface_params[if_idx].curr_pkt_no;

}

void flush_all_flags(){
	for (int a=0;a < BUFFER_SIZE; a++)seq_params.seq_flags[a]=0;
}



/**	'process_enque_thread_function' looks at the global buffer once   **
 **	in a millisecond and checks for the packets which have arrived    **
 **	and gives it to the consumer in a ordered fashion. The ones  	  **
 **	which have not arrived in sequence are dropped. 	          **/

void *process_enque_thread_function()
{
	int cur_index = 0; static int compensate, wait=1, drop_count;
	unsigned long time_now = 0, wait_time = 0;
	static unsigned long comp_time=0;
	printf("second thread started \n");
	while (1) {
                /* First, let us deliver packets */

		while(!start_deliver){}	

		while (seq_params.seq_flags[cur_index] == 1) {
//     	        	printf(" flag: %d next exp pkt %d\n",seq_params.seq_flags[cur_index],seq_params.nxt_exp_pkt[cur_index]);
//			printf("comp_time= %lu sleep_time %lu  get_current_time %lu prev_time %lu comp%d\n", (sleep_time - (get_current_time() - seq_params.prev_time)), sleep_time, get_current_time(), seq_params.prev_time, compensate);
			comp_time=abs(sleep_time - (get_current_time()-seq_params.prev_time));

			if (seq_params.most_recent_seq_no - glob_index < (15)){printf(" adjusted sleep:%lu wait=%d\n",sleep_time + wait*100,wait);usleep(sleep_time + wait*100);if(wait < 15)wait++;}
			else if(compensate) {printf(" comptime:%lu\n",comp_time);usleep(comp_time);wait=1;} 
			else {printf(" slptime:%lu\n",sleep_time);usleep(sleep_time);wait=1;}
			
			DELIVER:deliver_packet(cur_index);
                        	wait_time = 0;        
				cur_index = ++glob_index % BUFFER_SIZE;
				compensate = 0;
                        	drop_count=0;	
		}
                        

                   /* There is no packet at this index. If there are packets ahead, go into
                       wait phase */
			 
		if(seq_params.most_recent_seq_no > glob_index  ) {
			if (seq_params.most_recent_seq_no - glob_index >= 10 && (seq_params.in_seq_buffer[glob_index % BUFFER_SIZE].packet_number > seq_params.in_seq_buffer[(glob_index + 5) % BUFFER_SIZE].packet_number)){
				printf("here's the jump\n\n");
				flush_all_flags();
				glob_index = seq_params.most_recent_seq_no;
	                        cur_index = glob_index % BUFFER_SIZE;
				seq_params.seq_flags[cur_index] = 1;
				wait_time = 0;
				compensate=0;
                        	drop_count=0;	
				goto DELIVER;	
			}	
		}

			 if (!wait_time) {
					/*seq_params.jitter = 0;
    					for(int a=0; a < MAX_INTERFACE; a++){
							                printf(" %lu\n",interface_params[a].jitter);
							                interface_params[a].jitter >= seq_params.jitter   ?
							                seq_params.jitter = interface_params[a].jitter : (1) ;
					}*/
					time_now = get_current_time();
					seq_params.prev_time = time_now;
					compensate=1;
					wait_time = time_now + sleep_time;
					printf("wait_time = %lu, current time=%lu jitter= %lu for %d\n",wait_time,get_current_time(),seq_params.jitter,glob_index);
			} else if (get_current_time() > wait_time) {
				seq_params.seq_flags[cur_index] =  discard_packet(glob_index);
				drop_count++;
				printf("drop count %d\n",drop_count);
				if(drop_count==10){start_deliver=0;drop_count=0;printf("sleep until buffer replenishes \n");}
				printf("start deliver= %d\n ",start_deliver);
				printf(" flag: %d next exp pkt %d\n",seq_params.seq_flags[cur_index],seq_params.nxt_exp_pkt[cur_index]);
                                printf("waiting for/delivering packet no = %d\n",++glob_index);
                                cur_index = glob_index % BUFFER_SIZE;
				wait_time = 0;
				compensate=0;
				}

		}
}




void deliver_packet(int index)
{

	socklen_t _addr_len;
	struct sockaddr_in _addr;
/*	static unsigned long timethen,timenow;
        timenow=get_current_time();
	printf("recvdiff %lu\n",timenow-timethen);
	timethen=timenow;
  */      _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        _addr.sin_port = htons(12345);
        _addr_len = sizeof(struct sockaddr_in);
       
	if(sendto(send_sock, &seq_params.in_seq_buffer[index], sizeof(struct packet), 0,(struct sockaddr *)&_addr,_addr_len)<0)
		{perror("send to");printf("sendto-1 failed: %s \n", strerror(errno));}
        /* 
		segregate data and send to respective app ports
         */

	printf("packet %d from iface %d consumed from index %d at %lu\n",seq_params.in_seq_buffer[index].packet_number,seq_params.in_seq_buffer[index].interface,index, get_current_time());
	seq_params.seq_flags[index] = 0;
}

int drop_packet(struct packet *drop_pkt) 
{
	/**     bzero is a place holder for removing the header of the packet   **
         **     (packet_numer and interface in this case) and sending only the  **
         **     data part of the packet recieved!!                              **/
      printf("packet dropped %d from %d \n",drop_pkt->packet_number,drop_pkt->interface);

	return (0);
}

/*mark slot of timed out packets to drop them on arrival*/

int discard_packet(int pkt_no)
{
	printf("wait_ time expired for %d\n",pkt_no);
	return (2);
}
