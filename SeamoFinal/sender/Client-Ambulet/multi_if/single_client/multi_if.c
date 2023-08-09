#include "multi_if.h"

int main(int argc, char **argv)
{
	int count = 0;
	int sockfd1, iResult;
	struct sockaddr_in serv_addr;
	struct packet send_buf;
	int delay, bit_rate, drop_pkt;
	unsigned long present_time;
	struct timeval tv;
	FILE* fp1;
        fp1 = fopen("./ecgsyn.dat", "r+");

	if(argc <= 1){
		printf("Please enter bit rate!!!!!\n");
		exit(0);
	}
	bit_rate = atoi(argv[1]);   
		printf("argv=%s\n", argv[1]);

	if(argc == 3){
		drop_pkt = atoi(argv[2]);
	}else{
		drop_pkt = 0;
	}

	send_buf.bit_rate = bit_rate;
	
	delay = BitRateToDelay(bit_rate);
	printf("delay=%d\n", delay);



	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(60000);

	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}


	for (count = 0; count < 1000000; count++) {

		fgets(send_buf.data, 255, (FILE*)fp1);
		printf("%s\n",send_buf.data);
		send_buf.packet_number = count;
		send_buf.interface = 1;

		gettimeofday(&tv, NULL);
	
		present_time = 1000000 * tv.tv_sec + tv.tv_usec;
//	present_time = present_time / 1000;

		send_buf.time_stamp = present_time;

		iResult = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-1 failed: %s \n", strerror(errno));
		}

		printf("packet no %d with time_stamp %lu sent\n",count, send_buf.time_stamp);	
		usleep(delay);
	}
	return (0);
}

int BitRateToDelay(int bitrate)
{

	float size_of_packet;
	float number_of_packets, delay;

	size_of_packet = (sizeof(struct packet));   
	
//      printf("Bitrate in KBps : %f\n", ((bitrate * 1000) / 8));

	number_of_packets = ((bitrate * 1000) / 8) / size_of_packet;

	delay = 1000000 / number_of_packets;

	printf("num pakts : %f sizeof packet : %f Delay : %f\n",
	       number_of_packets, size_of_packet, delay);

	return (delay);
}
