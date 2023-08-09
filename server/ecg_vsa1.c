#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MYPORT 4950 
#define MAXBUFLEN 100
#define SERVERPORT 4955
int main(void)
{
int sockfd;
struct sockaddr_in my_addr; // my address information
struct sockaddr_in their_addr,local_addr; // connector’s address information
socklen_t addr_len;
int numbytes,writebytes;
char buf[MAXBUFLEN];
if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
perror("socket");
exit(1);
}


their_addr.sin_family = AF_INET; // host byte order
their_addr.sin_port = htons(SERVERPORT); // short, network byte order
their_addr.sin_addr.s_addr = inet_addr("10.32.13.88");
memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct

my_addr.sin_family = AF_INET; // host byte order
my_addr.sin_port = htons(MYPORT); // short, network byte order
my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
memset(&(my_addr.sin_zero), "\0", 8); // zero the rest of the struct

if (bind(sockfd, (struct sockaddr *)&my_addr,
sizeof(struct sockaddr)) == -1) {
perror("bind");
exit(1);
}

addr_len = sizeof(struct sockaddr);
while(1) {
if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
(struct sockaddr *)&local_addr, &addr_len)) == -1) {
perror("recvfrom");
exit(1);
}
buf[numbytes] = "\0";
//printf("%s\n",buf);

if ((writebytes=sendto(sockfd, buf, numbytes , 0,
(struct sockaddr *)&their_addr, addr_len)) == -1) {
perror("recvfrom");
exit(1);
}

}

return 0;
}
