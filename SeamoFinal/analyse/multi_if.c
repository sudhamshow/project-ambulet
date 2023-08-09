#include "multi_if1.h"

int main(int argc, char **argv)
{
	int sockfd1, iResult, interface;
	struct sockaddr_in serv_addr,src_addr;
	struct packet send_buf;
        unsigned long present_time;
	struct timeval tv;
	FILE* fp1,*fp2,*fp3;	
	char sleep[100], pkt[10],iface[10]; 
	fp1 = fopen("./time.txt", "r+");
	fp2 = fopen("./pckts.txt", "r+");
	fp3 = fopen("./iface.txt", "r+");
	strcpy(send_buf.data, "testPackets");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(61000);
	
	src_addr.sin_family = AF_INET;
    	src_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}
	for(int x=1;x<=1928;x++){
		fgets(sleep, 255, (FILE*)fp1);
		fgets(pkt, 255, (FILE*)fp2);
		fgets(iface, 255, (FILE*)fp3);
                send_buf.packet_number = atoi(pkt);
		send_buf.interface = atoi(iface);
		gettimeofday(&tv, NULL);
                present_time = 1000000 * tv.tv_sec + tv.tv_usec;
                send_buf.time_stamp = present_time;
		send_buf.bit_rate=256;
		printf("%s\n",sleep);
		iResult = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-1 failed: %s \n", strerror(errno));
		}

		usleep((atoi(sleep)-120>0) ? atoi(sleep)-120 : atoi(sleep));
		printf("packet no %d sent at %lu\n ", send_buf.packet_number,present_time);	
	}
	return (0);
}
