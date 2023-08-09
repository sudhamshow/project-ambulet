#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include"multi_if.h"
#define SERVERPORT 61000 // the port users will be connecting to
#define APP_PORT 18000

int main(int argc, char *argv[])
{
int fd,iResult,sockfd1;
struct ifreq ifr;
struct sockaddr_in their_addr,eth_addr; // connector’s address information
int numbytes;
unsigned long present_time;
struct timeval tv;
struct packet send_buf;
char* c = argv[2];
int x = strlen(c);

fd = socket(AF_INET, SOCK_DGRAM, 0);

ifr.ifr_addr.sa_family = AF_INET;

strncpy(ifr.ifr_name, "lo", IFNAMSIZ - 1);

ioctl(fd, SIOCGIFADDR, &ifr);
close(fd);

eth_addr.sin_family = AF_INET;
eth_addr.sin_port = htons(APP_PORT);
eth_addr.sin_addr.s_addr = inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
memset(&(eth_addr.sin_zero), '\0', 8);

if ((sockfd1 = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
perror("socket");
exit(1);
}

iResult =
	    bind(sockfd1, (struct sockaddr *)&eth_addr, sizeof(eth_addr));
	if (iResult == -1) {
		printf("bind error1 : %s \n", strerror(errno));
	}	

their_addr.sin_family = AF_INET; // host byte order
their_addr.sin_port = htons(SERVERPORT); // short, network byte order
their_addr.sin_addr.s_addr = inet_addr(argv[1]);
memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct
c[x+1]= '\0';


    		printf("%s %s %s %s\n",argv[1],argv[2],argv[3],argv[4]);
                send_buf.packet_number = atoi(argv[2]);
                printf("%d \n",send_buf.packet_number);
		strcpy(send_buf.data, c);
#if 0
                gettimeofday(&tv, NULL);

                present_time = 1000000 * tv.tv_sec + tv.tv_usec;
//      present_time = present_time / 1000;

                send_buf.time_stamp = present_time;

#endif
if ((numbytes = sendto(sockfd1, &send_buf, sizeof(send_buf), 0,
(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
perror("sendto");
exit(1);
}

return 0;

}
