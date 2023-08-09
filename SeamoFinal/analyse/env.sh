#!/bin/bash
while [ TRUE ]
do
	tc qdisc add dev lo root netem delay 50ms 10ms 35% loss 35% 50% reorder 0%
	sleep 3
	tc qdisc change dev lo root netem delay 0ms loss 0% reorder 0%
	sleep 3
	tc qdisc change dev lo root netem delay 30ms 8ms 30% loss 20% 25% reorder 0%
	sleep 2
	tc qdisc del dev lo root
	sleep 2
done
