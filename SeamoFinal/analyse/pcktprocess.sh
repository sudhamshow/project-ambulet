#!/bin/bash
loss=0
numOfLines=`wc -l $1 | awk '{print $1}'`
LineCount=1
PrevPkt=0
while [ $LineCount -lt $numOfLines ]
do
	CurrentPkt=`head -$LineCount $1 | tail -1 | awk '{print $1}'`
	a=`expr $CurrentPkt - $PrevPkt - 1`
	if (( $a > 0 ))
	then {
		echo "current $CurrentPkt prev $PrevPkt loss $a"
		loss=`expr $loss + $a`
	}
	fi
#	echo $CurrentPkt 
#	echo $PrevPkt
	LineCount=`expr $LineCount + 1`
	PrevPkt=$CurrentPkt	
	echo "last_packet	pdr	 recv_delay	consumed	if0	if1	if2"
done 
	echo "total loss = $loss"
