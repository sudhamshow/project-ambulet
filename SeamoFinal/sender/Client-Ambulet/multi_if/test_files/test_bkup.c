#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

int main()
{
   int fd,fd2;
   struct ifreq ifr,ifr2;
   char * ip1;
   char * ip2;
   int sockfd1,sockfd2,iResult;
   struct sockaddr_in wifi_addr,dongle_addr,serv_addr,serv_addr_10;
   char * sendline = "This is a test Packet";
   char * DestAddr;

   fd = socket(AF_INET, SOCK_DGRAM, 0);
   fd2 = socket(AF_INET, SOCK_DGRAM, 0);

   /* I want to get an IPv4 IP address */
   ifr.ifr_addr.sa_family = AF_INET;
   ifr2.ifr_addr.sa_family = AF_INET;

   /* I want IP address attached to "eth0" */
   strncpy(ifr.ifr_name, "ppp0", IFNAMSIZ-1);
   strncpy(ifr2.ifr_name,"wlan1", IFNAMSIZ-1);

   ioctl(fd, SIOCGIFADDR, &ifr);
   close(fd);
   ioctl(fd2, SIOCGIFADDR, &ifr2);
   close(fd2);

   printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
   printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr));
//   printf("Enter dongle IP :");
//   scanf("%s", &DestAddr);

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr=inet_addr("202.141.1.50");
   serv_addr.sin_port=htons(2000);


   serv_addr_10.sin_family = AF_INET;
   serv_addr_10.sin_addr.s_addr=inet_addr("202.141.1.50");//inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
   serv_addr_10.sin_port=htons(2210);

   wifi_addr.sin_family = AF_INET;
   wifi_addr.sin_addr.s_addr=inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr));//"202.41.124.62");
//   wifi_addr.sin_port=htons(32000);

   dongle_addr.sin_family = AF_INET;
   dongle_addr.sin_addr.s_addr=inet_addr(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));//"10.32.14.74");
//   dongle_addr.sin_port=htons(42000);
   
   sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(sockfd1 == -1){   
	printf("socket error!: %s \n", strerror(errno));
	}

   sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(sockfd2 == -1){   
	printf("socket error!: %s \n", strerror(errno));
	}

   iResult = bind(sockfd1,(struct sockaddr *)&wifi_addr,sizeof(wifi_addr));
   if(iResult == -1){
	printf("bind error1 : %s \n",strerror(errno));
	}
  
   iResult = bind(sockfd2,(struct sockaddr *)&dongle_addr,sizeof(dongle_addr));
   if(iResult == -1){
	printf("bind error2 : %s \n",strerror(errno));
	}

   while (1)
   {
     iResult =  sendto(sockfd1,sendline,strlen(sendline),0,
             (struct sockaddr *)&serv_addr,sizeof(serv_addr));
     if(iResult == -1){
	printf("sendto-1 failed: %s \n", strerror(errno));
	} 
/*     else{
	printf("packet sent through : WiFi \n");
	}*/

     iResult =  sendto(sockfd2,sendline,strlen(sendline),0,
             (struct sockaddr *)&serv_addr_10,sizeof(serv_addr_10));
     if(iResult == -1){
	printf("sendto-2 failed: %s \n", strerror( errno ));
	} 
/*     else{
	printf("packet sent through: dongle \n");
	}*/
   }
}
