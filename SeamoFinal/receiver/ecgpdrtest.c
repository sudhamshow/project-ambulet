#include "./multi_if.h"
#define MYPORT 12345 
void* print_pdr_data(void *dat);
struct timeval tv;
unsigned long get_current_time();
	int sockfd;
	static int a;
	static int loss,pkt_then=0;
	struct sockaddr_in my_addr; 
	struct sockaddr_in their_addr; 
	socklen_t addr_len;
	unsigned long last_time=0,present_time=0;
	static int ifa[3]={0},key;
	int numbytes;
	float pdr=0;
	int last_pkt=0,pkt_cnt=0,pktin50=0,count=0;
	char* pkt;
	struct packet* buf;
	FILE *fptr[3];
int main(void){
	pthread_t print_pdr;
	pkt=malloc(BUFFERSIZE);
	fptr[0] = fopen("pdr.txt","a");
	if (fptr[0] == NULL){
	printf("Error");
	exit(1);
	}	

	fptr[1] = fopen("recv_delay.txt","a");
        if (fptr[1] == NULL){
        printf("Error");
        exit(1);
        }

	fptr[2] = fopen("ecg.txt","a");
        if (fptr[2] == NULL){
        printf("Error");
        exit(1);
        }

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}	

	my_addr.sin_family = AF_INET; 
	my_addr.sin_port = htons(MYPORT); 
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8); 
	if (bind(sockfd, (struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	addr_len = sizeof(struct sockaddr);
	if(!pthread_create(&print_pdr,NULL,print_pdr_data,(void*) &pdr)){
       	perror("pthread create");
	}
	while(1) { bzero(pkt,BUFFERSIZE); 
		   if ((numbytes=recvfrom(sockfd, pkt, BUFFERSIZE , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
                        perror("recvfrom");
                        exit(1);
              	   }  else {
				
				key=1;
				buf=(struct packet*) pkt;
                                last_pkt=buf->packet_number + 1;
//				printf("%d ",buf->packet_number);
                                pktin50 = ((last_pkt%300)==0 ? 300 : (last_pkt%300));
//                              pkt_cnt==50 ? (pkt_cnt=1) : 
				pkt_cnt++; 
				if((last_pkt/300)>count){if(last_pkt%300>0){pkt_cnt=1;count++;}}//{if(pkt_cnt==50){}else{pkt_cnt=1;count++;}}
				present_time=get_current_time();
        	        	fprintf(fptr[1],"%lu\n",(present_time-last_time) > 86400000000 ? 32000 : (present_time-last_time));
               			fflush(fptr[1]);
                                fprintf(fptr[2],"%s",buf->data);
                                fflush(fptr[2]);
				last_time = present_time;
//                              printf("count= %d stampmod50= %d ",pkt_cnt,pktin50);
 			/*	if (last_pkt - pkt_then > 31){a=0; pkt_then = last_pkt -1;} 
	                       	loss = last_pkt - pkt_then - 1;
				if (loss > 0){a = a + loss;}
			        pkt_then = last_pkt;
				//pdr=(float) pkt_cnt/pktin50;
                        */	ifa[buf->interface]++;
			        
				//printf("interface=%d ecgdata=%s packetstamp %d pdelratio= %f \n",buf->interface,buf->data,buf->packet_number,pdr);
      	          
		   }	
		 }

	return 0;
}

void* print_pdr_data(void *dat){
	while(1){
	if(key){	int total = ifa[0]+ifa[1]+ifa[2];
			31 - total > 1 ? loss=(31-total) : (loss=0); 
			float z=(float) total/(total + loss);
			if(total == 0) {pdr=0;loss=0;}
			else pdr=z;
			fprintf(fptr[0],"%f\n",*(float*)dat);
			fflush(fptr[0]);
                	printf("pckt_recvd:%d\t loss:%d\t pdr:%.3f\t  if0:%d \t if1:%d \t if2:%d\n ",last_pkt-1,loss,pdr,ifa[0],ifa[1],ifa[2]);
			ifa[0]=0;ifa[1]=0;ifa[2]=0,a=0;
			sleep(1);   //display pdr every second
	}
		}
}

unsigned long get_current_time(){
        unsigned long present_time;
        gettimeofday(&tv, NULL);
        present_time = 1000000 * tv.tv_sec + tv.tv_usec;
        return present_time;
        }


