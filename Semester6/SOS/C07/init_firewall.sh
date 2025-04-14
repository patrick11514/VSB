#!/bin/bash
#RULES
iptables -P INPUT ACCEPT

iptables -A INPUT -p tcp --dport 80  -j ACCEPT
iptables -A INPUT -p tcp --dport 443 -j ACCEPT

iptables -A INPUT -p tcp -s 192.168.56.1 --dport 22 -j ACCEPT
iptables -A INPUT -p tcp --dport 22 -j DROP 

iptables -A INPUT -j DROP

#NAT
iptables -t nat -A POSTROUTING -o enp0s3 -j MASQUERADE
