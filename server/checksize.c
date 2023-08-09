#include<stdio.h>
#include<stdint.h>
struct packet{
        uint32_t interface;
        uint32_t packet_number;
        uint32_t bit_rate;
        uint64_t time_stamp;
        char data[1004];
}__attribute__((packed));
int main(){
struct packet a;
printf("%d ",sizeof(a.interface));
printf("%d ",sizeof(a.packet_number));
printf("%d ",sizeof(a.bit_rate));
printf("%d ",sizeof(a.time_stamp));
printf("%d \n",sizeof(a.data));
printf("%d \n",sizeof(struct packet));
}
