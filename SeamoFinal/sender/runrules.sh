#!/bin/bash
ip rule flush
ip rule add from all lookup main priority 32766
ip rule add from all lookup default priority 32767
eth1ip=`ifconfig eth1 | grep inet | awk '{print $2}' | tr -d \addr:`
ppp0ip=`ifconfig ppp0 | grep inet | awk '{print $2}' | tr -d \addr:`
ppp1ip=`ifconfig wwan1 | grep inet | awk '{print $2}' | tr -d \addr:`
echo $eth1
echo $ppp0ip
echo $ppp1ip

ip route add default dev ppp0 table ppp0
ip route add default dev wwan1 table ppp1
ip route add default via 192.168.8.1 dev eth1 table eth1

ip rule add from $eth1ip lookup eth1 priority 99
ip rule add from $ppp0ip lookup ppp0 priority 100
ip rule add from $ppp1ip lookup ppp1 priority 101
