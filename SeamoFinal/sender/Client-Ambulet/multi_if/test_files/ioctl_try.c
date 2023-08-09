#include <stdio.h>

#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int
main()
{
 int fd,fd2;
 struct ifreq ifr,ifr2;
 char * ip1;
 char * ip2;
 fd = socket(AF_INET, SOCK_DGRAM, 0);
 fd2 = socket(AF_INET, SOCK_DGRAM, 0);

 /* I want to get an IPv4 IP address */
 ifr.ifr_addr.sa_family = AF_INET;
 ifr2.ifr_addr.sa_family = AF_INET;

 /* I want IP address attached to "eth0" */
 strncpy(ifr.ifr_name, "eth1", IFNAMSIZ-1);
 strncpy(ifr2.ifr_name,"wlan1", IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);
 close(fd);
 ioctl(fd2, SIOCGIFADDR, &ifr2);
 close(fd2);
 
 /* display result */
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr));
 ip1 = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
 ip2 = inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr);

 printf("%s\n",ip1);
 printf("%s\n",ip2);

 return 0;
}
