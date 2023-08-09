#!/bin/bash
w=`wc -l $1 | awk '{print $1}'`
for ((a=1;a<$w;a++))
do
	echo "`head -$a $1 | tail -1 | awk '{print $1}' | tr -d \packet=` `head -$a $1 | tail -1 | awk '{print $3}' | tr -d \recvdelay=`"
done
