#include "multi_if.h"

int main(int argc, char **argv)
{
	int fd, fd2, count = 0;
	struct ifreq ifr, ifr2;
	int sockfd1, sockfd2, iResult;
	struct sockaddr_in wifi_addr, dongle_addr, serv_addr;
	struct packet send_buf;
	int delay, bit_rate, drop_pkt;

	bit_rate = atoi(argv[1]);
	drop_pkt = atoi(argv[2]);

	delay = BitRateToDelay(bit_rate);
	printf("delay=%d\n", delay);

	strcpy(send_buf.data, "These are test packets!");

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	fd2 = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;
	ifr2.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, "ppp0", IFNAMSIZ - 1);
	strncpy(ifr2.ifr_name, "wlan1", IFNAMSIZ - 1);

	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	ioctl(fd2, SIOCGIFADDR, &ifr2);
	close(fd2);

	printf("%s\n",
	       inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	printf("%s\n",
	       inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//	serv_addr.sin_addr.s_addr = inet_addr("202.41.124.55";
	serv_addr.sin_port = htons(55689);

	wifi_addr.sin_family = AF_INET;
	wifi_addr.sin_addr.s_addr = inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr));	//"202.41.124.62");
//   wifi_addr.sin_port         = htons(32000);

	dongle_addr.sin_family = AF_INET;
	dongle_addr.sin_addr.s_addr = inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));	//"10.32.14.74");
//   dongle_addr.sin_port       = htons(42000);

	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}

	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd2 == -1) {
		printf("socket error!: %s \n", strerror(errno));
	}

	iResult =
	    bind(sockfd1, (struct sockaddr *)&wifi_addr, sizeof(wifi_addr));
	if (iResult == -1) {
		printf("bind error1 : %s \n", strerror(errno));
	}

	iResult =
	    bind(sockfd2, (struct sockaddr *)&dongle_addr, sizeof(dongle_addr));
	if (iResult == -1) {
		printf("bind error2 : %s \n", strerror(errno));
	}

	for (count = 0; count < 10000; count++) {

		if( count%BUFFER_SIZE == drop_pkt){
			printf("packet no %d dropped\n",count);	
			count++;
			}
		send_buf.packet_number = count;
		send_buf.interface = 1;

		iResult = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-1 failed: %s \n", strerror(errno));
		}
/*     else{
	printf("packet sent through : WiFi \n");
	}*/

		printf("sendbuf.if:%d\t", send_buf.interface);

		send_buf.interface = 2;
		iResult = sendto(sockfd2, &send_buf, sizeof(send_buf), 0,
				 (struct sockaddr *)&serv_addr,
				 sizeof(serv_addr));
		if (iResult == -1) {
			printf("sendto-2 failed: %s \n", strerror(errno));
		}
		usleep(delay);
		printf("sendbuf.if:%d\n", send_buf.interface);
/*     else{
	printf("packet sent through: dongle \n");
	}*/
//   }
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
