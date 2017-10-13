
for i in `seq 10 20`; 
do 
    echo ifconfig ens33:$i 192.168.142.$i up
    sudo ifconfig ens33:$i 192.168.142.$i up
done
