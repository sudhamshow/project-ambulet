#include "multi_if1.h"

int main(int argc, char **argv)
{
	int count = 0;
	int sockfd1, iResult, interface;
	struct sockaddr_in serv_addr;
	struct packet send_buf;

	

	strcpy(send_buf.data, argv[1]);
	strcat(send_buf.data," ");
	strcat(send_buf.data,argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(61000);

	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}


		send_buf.interface = 2563;
		send_buf.packet_number = atoi(argv[3]);

		iResult = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-1 failed: %s \n", strerror(errno));
		}
		printf("packet no %d sent\n", send_buf.packet_number);	
	return (0);
}

