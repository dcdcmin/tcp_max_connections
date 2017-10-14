## 服务器最大连接数测试。

### Linux下的准备工作：

1.修改：/etc/security/limits.conf

增加下面这行：

```
* - nofile 2097152
```

> 重启系统。 

2.设置tcp连接参数及最大打开文件个数。

```
source tools/linux_env.sh
```

3.绑定多个IP。一个IP能链接的个数受限与端口的个数，所以需要多加几个IP。

```
./tools/add-ips.sh
```

> 网卡名和IP可以根据实际情况修改。

### 运行测试

*. http链接数测试

```
Usage: ./bin/http_connection host port clients [localip]
```

*. mqtt链接数测试

```
Usage: ./bin/mqtt_connection host port clients [localip]
```



