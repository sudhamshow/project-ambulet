#!/bin/bash
numOfLines=`wc -l $1 | awk '{print $1}'`
LineCount=1
nextline=2
PrevPkt=0
while [ $LineCount -lt $numOfLines ]
do
	RecvTime1=`head -$LineCount $1 | tail -1 | awk '{print $8}'`
	RecvTime2=`head -$nextline $1 | tail -1 | awk '{print $8}'`
	pckt=`head -$LineCount $1 | tail -1 | awk '{print $5}'`
	iface=`head -$LineCount $1 | tail -1 | awk '{print $6}' | tr -d \interface:`
	timediff1=`expr $RecvTime2 - $RecvTime1`
	LineCount=`expr $LineCount + 1`
	nextline=`expr $nextline + 1`
	echo "$pckt  $timediff1 $iface" 
done 
