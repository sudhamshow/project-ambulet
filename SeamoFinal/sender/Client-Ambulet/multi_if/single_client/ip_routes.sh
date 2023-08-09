#! /bin/bash

gateway_ppp0=`ifconfig ppp0 | grep P-t-P | awk -F : '{print $3}' | awk '{print $1}'`
echo $gateway_ppp0

ipaddress_ppp0=`ifconfig ppp0 | grep addr | awk -F : '{print $2}' | awk '{print $1}'`
echo $ipaddress_ppp0

ip route add default via $gateway_ppp0 table gw1

ip rule add from $ipaddress_ppp0 table gw1


ip route add default via 202.41.124.49  table gw2

ip rule add from 202.41.124.62 table gw2
