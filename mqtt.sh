#!/bin/bash

PORT=9999
IP_END=20
IP_START=10
CLIENTS=50000
TARGET=localhost
IP_PREFIX="192.168.142"

for i in `seq ${IP_START} ${IP_END}`
do
	echo ./bin/mqtt_connection ${TARGET} ${PORT} ${CLIENTS} ${IP_PREFIX}.$i
	./bin/mqtt_connection ${TARGET} ${PORT} ${CLIENTS} ${IP_PREFIX}.$i &
done


