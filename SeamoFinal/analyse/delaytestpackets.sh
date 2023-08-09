#!/bin/bash
numOfLines=`wc -l $1 | awk '{print $1}'`
LineCount=1
nextline=2
PrevPkt=0
while [ $LineCount -lt $numOfLines ]
do
	SentTime1=`head -$LineCount $1 | tail -1 | awk '{print $5}' | tr -d \sent_time=`
	SentTime2=`head -$nextline $1 | tail -1 | awk '{print $5}' | tr -d \sent_time=`
	RecvTime1=`head -1 $1 | tail -1 | awk '{print $6}' | tr -d \recvTime=`
	RecvTime2=`head -$LineCount $1 | tail -1 | awk '{print $6}' | tr -d \recvTime=`
	pckt=`head -$LineCount $1 | tail -1 | awk '{print $4}'`
	if=`head -$LineCount $1 | tail -1 | awk '{print $2}'`
	timediff1=`expr $RecvTime2 - $RecvTime1`
	timediff2=`expr $SentTime2 - $SentTime1`
	LineCount=`expr $LineCount + 1`
	nextline=`expr $nextline + 1`
	echo "packet= $pckt sendDelay= $timediff2 recvdelay= $timediff1 iface=$if" 
done 
