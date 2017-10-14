
for i in `seq 10 20`;
do
	echo ./bin/mqtt_connection localhost 9999 50000 192.168.142.$i
	./bin/mqtt_connection localhost 9999 50000 192.168.142.$i >/dev/null &
done


