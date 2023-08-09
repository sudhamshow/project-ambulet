#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
       {
           struct in_addr addr;
           char dst[INET_ADDRSTRLEN];

           if (argc != 2) {
               fprintf(stderr, "%s <dotted-address>\n", argv[0]);
               exit(0);
           }
/*
           if (inet_aton(argv[1], &addr) == 0) {
               fprintf(stderr, "Invalid address\n");
               exit(0);
           }
*/

          addr.s_addr = atoi(argv[1]);

           /* printf("%s\n", inet_ntoa(addr)); */
           inet_ntop (AF_INET, &addr, dst, INET_ADDRSTRLEN);
           printf("%s\n", dst);

           exit(0);
       }

