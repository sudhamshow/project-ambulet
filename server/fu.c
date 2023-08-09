#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include"multi_if.h"
#define MYPORT 4950
#define SERVERPORT 4955 // the port users will be connecting to
#define MAXBUFLEN 100
int main()
{
int sockfd,fd2,fd ,iResult,sockfd1;
struct ifreq ifr, ifr2;
struct sockaddr_in their_addr,wifi_addr,eth_addr; // connector’s address information
struct hostent *he;
int numbytes;

fd = socket(AF_INET, SOCK_DGRAM, 0);
fd2 = socket(AF_INET, SOCK_DGRAM, 0);

ifr.ifr_addr.sa_family = AF_INET;

strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

ioctl(fd, SIOCGIFADDR, &ifr);
close(fd);

eth_addr.sin_family = AF_INET;
eth_addr.sin_port = htons(MYPORT);
eth_addr.sin_addr.s_addr = inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));


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
//their_addr.sin_addr = *((struct in_addr *)he->h_addr);
their_addr.sin_addr.s_addr = inet_addr("10.32.13.88");
memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct

//struct sockaddr_in my_addr; // my address information
struct sockaddr_in their1_addr; // connector’s address information
socklen_t addr_len;
int numbytes1;
char buf[MAXBUFLEN];
if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
perror("socket");
exit(1);
}

/*my_addr.sin_family = AF_INET; // host byte order
my_addr.sin_port = htons(MYPORT); // short, network byte order
my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
memset(&(my_addr.sin_zero), "\0", 8); // zero the rest of the struct
if (bind(sockfd, (struct sockaddr *)&my_addr,
sizeof(struct sockaddr)) == -1) {
perror("bind");
exit(1);
}*/

addr_len = sizeof(struct sockaddr);
while(1) {
if ((numbytes1=recvfrom(sockfd1, buf, MAXBUFLEN-1 , 0,
(struct sockaddr *)&their1_addr, &addr_len)) ) { 
printf("check");
buf[numbytes1] = '\0';
printf("%s\n",buf);
if ((numbytes = sendto(sockfd1, buf, (numbytes1), 0,
(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
perror("sendto");
exit(1);
}
}

buf[numbytes1] = '\0';
printf("%s\n",buf);}

return 0;
}
