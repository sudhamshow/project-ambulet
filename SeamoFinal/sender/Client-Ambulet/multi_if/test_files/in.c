#include <stdio.h>
main (int argc,char** argv[])
{
int a = (int) argv[1];
int c;

c= getopt(argc,argv);

printf("argc %d c %d ",argc,c);
}
