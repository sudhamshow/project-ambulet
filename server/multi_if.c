#include "multi_if1.h"

int main(int argc, char **argv)
{
	int sockfd1, iResult, interface;
	struct sockaddr_in serv_addr,src_addr;
	struct packet send_buf;
        unsigned long present_time;
	struct timeval tv;
	

	strcpy(send_buf.data, "testPackets");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(61000);
	
	src_addr.sin_family = AF_INET;
    	src_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    	src_addr.sin_port = htons(34867);
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}
	if (bind(sockfd1, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        exit(1);
    }
		send_buf.interface = atoi(argv[2]);
                send_buf.packet_number = atoi(argv[1]);


		gettimeofday(&tv, NULL);
                present_time = 1000000 * tv.tv_sec + tv.tv_usec;
                send_buf.time_stamp = present_time;
		send_buf.bit_rate=256;
		iResult = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-1 failed: %s \n", strerror(errno));
		}

		printf("packet no %d sent at %lu\n ", send_buf.packet_number,present_time);	
	return (0);
}

