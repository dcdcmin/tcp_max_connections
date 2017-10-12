## tcpflood.cpp

测试服务器的最大连接数。可以用于支持长连接的服务器(如MQTT服务器)的最大连接数。

Linux下的用法：

1.设置tcp连接的Buffer大小：

```
sysctl -w net.ipv4.tcp_mem='4096 4096 4096'
sysctl -w net.ipv4.tcp_rmem='4096 4096 4096'
sysctl -w net.ipv4.tcp_wmem='4096 4096 4096'
```

2.设置客户端端口范围和超时时间

```
sysctl -w net.ipv4.ip_local_port_range="10000 61000"
sysctl -w net.ipv4.tcp_fin_timeout=30
```

3.绑定多个IP

```

for i in `seq 10 20`; 
do 
    echo ifconfig eth0:$i 10.0.0.$i up
    sudo ifconfig eth0:$i 10.0.0.$i up
done
```

> 网卡名和IP可以根据实际情况修改。

4.设置打开文件个数的限制。

修改：/etc/security/limits.conf

```
* - nofile 1048576
```

终端：

```
sysctl -w fs.file-max=2097152
sysctl -w fs.nr_open=2097152
sysctl -w net.core.somaxconn=65536

ulimit -Sn 1048576
ulimit -Hn 1048576
```

5.运行测试

```
Usage: ./tcpflood host port clients localip
```



