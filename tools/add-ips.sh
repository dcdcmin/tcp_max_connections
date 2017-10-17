#!/bin/bash

NIC="ens33"
IP_END=30
IP_START=10
IP_PREFIX="192.168.142"

for i in `seq $IP_START $IP_END`; 
do 
    echo ifconfig ${NIC}:$i ${IP_PREFIX}.$i up
    ifconfig ${NIC}:$i ${IP_PREFIX}.$i up
done
