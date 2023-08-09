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
#include <pthread.h>
#define PKT_BUFSIZ      1500


//void *ecg_data_send();
//int ecg_server();
int ecg_server_sock,ecg_client_sock;
struct sockaddr_in saddr_ecg_client, saddr_ecg_server, caddr;
//pthread_t ecg_send;
  
/*int main() {
	if (pthread_create(&ecg_send, NULL, &ecg_data_send, NULL))
         {
                 printf(" Issue while creating `ecg_send` thread \n");
                 abort();
         }

}*/


//void *ecg_data_send()
int main()
{
   int recv_len,send_len, bind_err,bind_err1;
   char buf [PKT_BUFSIZ];
   socklen_t len1, len;

        ecg_client_sock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
        if(ecg_client_sock < 0)
         {
              perror(" Could not open ecg-sender sock \n");
         }
        bzero(&saddr_ecg_server, sizeof(saddr_ecg_server));

        saddr_ecg_server.sin_family = AF_INET;
	saddr_ecg_server.sin_addr.s_addr = INADDR_ANY;
//        saddr_ecg_server.sin_addr.s_addr = inet_addr("127.0.0.1");
        saddr_ecg_server.sin_port = htons(4950);
	


	 ecg_server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
         if(ecg_server_sock < 0)
         perror("Couldn't open ecg dest sock \n");

         bzero(&saddr_ecg_client, sizeof(saddr_ecg_client));
	 saddr_ecg_client.sin_family = AF_INET;
         saddr_ecg_client.sin_addr.s_addr = inet_addr("10.32.13.88");
         saddr_ecg_client.sin_port = htons(12000);
         


	bind_err1 = bind(ecg_server_sock, (struct sockaddr *)&saddr_ecg_client, sizeof(saddr_ecg_client));
         if (bind_err1 < 0)
           perror(" bind failed\n");
        bind_err = bind(ecg_client_sock, (struct sockaddr *)&saddr_ecg_server, sizeof(saddr_ecg_server));
        if (bind_err < 0)
           perror(" bind failed\n");

        //ecg_server_sock = ecg_server();

		 len1 = sizeof(saddr_ecg_client);
                 len  = sizeof(caddr);
	while(1) {
                   recv_len = recvfrom(ecg_client_sock, buf, PKT_BUFSIZ, 0, (struct sockaddr *)&caddr, &len);
                   if(recv_len < 0)
                     {
                           perror(" ecg recvfrom error \n");
                     }
		

                        send_len = sendto(ecg_server_sock, buf, recv_len + 1, 0, (struct sockaddr *)&saddr_ecg_client, len1);
                        if(send_len <= 0)
                         {
                             perror ("sendto error");
                          }

		}
}
             
/*int ecg_server()
{
    int ecg_recv,bind_err1;

         ecg_recv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
         if(ecg_recv < 0)
         perror("Couldn't open ecg dest sock \n");

         bzero(&saddr_ecg_client, sizeof(saddr_ecg_client));
         saddr_ecg_client.sin_addr.s_addr = inet_addr("10.32.13.88");
         saddr_ecg_client.sin_port = htons(4955);
	 bind_err1 = bind(ecg_recv, (struct sockaddr *)&saddr_ecg_server, sizeof(saddr_ecg_server));
         if (bind_err1 < 0)
           perror(" bind failed\n");

  return ecg_recv;
}
*/

