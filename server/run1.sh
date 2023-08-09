#!/bin/bash
v=`wc -l pcktslog.txt | awk '{print $1}'`
#echo $v
a=1
b=1
c=0
while [ $a -lt $v ]
do
	w=`head -$b ecgsyn.dat | tail -1 | awk '{print $2}'`
	x=`head -$b pcktslog.txt | tail -1 | awk '{print $2}'`
	c=`head -$b pcktslog.txt | tail -1 | awk '{print $1}'`
	i=`head -$b iface.txt | tail -1 | awk '{print $2}'`
	a=`expr $a + 1`
	b=`expr $b + 1`
	x=`echo "scale=6;$x / 1000000" | bc`
	./send $c $i 
	sleep $x
done 
