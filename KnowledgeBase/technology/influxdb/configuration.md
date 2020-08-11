## 配置



1. 生成配置

    influxd config > default.conf

2. 默认配置文件

    /etc/influxdb/influxdb.conf

## Config

### 全局配置

```bash
reporting-disabled = false  # 该选项用于上报influxdb的使用信息给InfluxData公司，默认值为false
bind-address = "0.0.0.0:8088"  # 备份恢复时使用，默认值为8088
```

### meta配置

```bash
[meta]
dir = "/var/lib/influxdb/meta"  # meta数据存放目录
retention-autocreate = true  # 用于控制默认存储策略，数据库创建时，会自动生成autogen的存储策略，默认值：true
logging-enabled = true  # 是否开启meta日志，默认值：true
```

### data配置

```bash
[data]
dir = "/var/lib/influxdb/data"  # 最终数据（TSM文件）存储目录
wal-dir = "/var/lib/influxdb/wal"  # 预写日志存储目录
query-log-enabled = true  # 是否开启tsm引擎查询日志，默认值： true
cache-max-memory-size = 1048576000  # 用于限定shard最大值，大于该值时会拒绝写入，默认值：1000MB，单位：byte
cache-snapshot-memory-size = 26214400  # 用于设置快照大小，大于该值时数据会刷新到tsm文件，默认值：25MB，单位：byte
cache-snapshot-write-cold-duration = "10m"  # tsm引擎 snapshot写盘延迟，默认值：10Minute
compact-full-write-cold-duration = "4h"  # tsm文件在压缩前可以存储的最大时间，默认值：4Hour
max-series-per-database = 1000000  # 限制数据库的级数，该值为0时取消限制，默认值：1000000
max-values-per-tag = 100000  # 一个tag最大的value数，0取消限制，默认值：100000
```


### coordinator

```bash
[coordinator]
write-timeout = "10s"  # 写操作超时时间，默认值： 10s
max-concurrent-queries = 0  # 最大并发查询数，0无限制，默认值： 0
query-timeout = "0s"  # 查询操作超时时间，0无限制，默认值：0s
log-queries-after = "0s"  # 慢查询超时时间，0无限制，默认值：0s
max-select-point = 0  # SELECT语句可以处理的最大点数（points），0无限制，默认值：0
max-select-series = 0  # SELECT语句可以处理的最大级数（series），0无限制，默认值：0
max-select-buckets = 0  # SELECT语句可以处理的最大"GROUP BY time()"的时间周期，0无限制，默认值：0
```

### retention配置

```bash
[retention]
enabled = true  # 是否启用该模块，默认值 ： true
check-interval = "30m"  # 检查时间间隔，默认值 ："30m"
```



### shard-precreation配置

```bash
[shard-precreation]
enabled = true  # 是否启用该模块，默认值 ： true
check-interval = "10m"  # 检查时间间隔，默认值 ："10m"
advance-period = "30m"  # 预创建分区的最大提前时间，默认值 ："30m"
```


### monitor配置

>monitor 控制InfluxDB自有的监控系统。 默认情况下，InfluxDB把这些数据写入_internal 数据库，如果这个库不存在则自动创建。 _internal 库默认的retention策略是7天，如果你想使用一个自己的retention策略，需要自己创建。

```bash
[monitor]
store-enabled = true  # 是否启用该模块，默认值 ：true
store-database = "_internal"  # 默认数据库："_internal"
store-interval = "10s  # 统计间隔，默认值："10s"
```

### admin web配置

```bash
[admin]
enabled = true  # 是否启用该模块，默认值 ： false
bind-address = ":8083"  # 绑定地址，默认值 ：":8083"
https-enabled = false  # 是否开启https ，默认值 ：false
https-certificate = "/etc/ssl/influxdb.pem"  # https证书路径，默认值："/etc/ssl/influxdb.pem"
```

### http Api配置

```bash
[http]
enabled = true  # 是否启用该模块，默认值 ：true
bind-address = ":8086"  # 绑定地址，默认值：":8086"
auth-enabled = false  # 是否开启认证，默认值：false
realm = "InfluxDB"  # 配置JWT realm，默认值: "InfluxDB"
log-enabled = true  # 是否开启日志，默认值：true
write-tracing = false  # 是否开启写操作日志，如果置成true，每一次写操作都会打日志，默认值：false
pprof-enabled = true  # 是否开启pprof，默认值：true
https-enabled = false  # 是否开启https，默认值：false
https-certificate = "/etc/ssl/influxdb.pem"  # 设置https证书路径，默认值："/etc/ssl/influxdb.pem"
https-private-key = ""  # 设置https私钥，无默认值
shared-secret = ""  # 用于JWT签名的共享密钥，无默认值
max-row-limit = 0  # 配置查询返回最大行数，0无限制，默认值：0
max-connection-limit = 0  # 配置最大连接数，0无限制，默认值：0
unix-socket-enabled = false  # 是否使用unix-socket，默认值：false
bind-socket = "/var/run/influxdb.sock"  # unix-socket路径，默认值："/var/run/influxdb.sock"
```



### subscriber配置

>subscriber控制kapacitor（时序数据处理引擎）接受数据的配置。

```bash
[subscriber]
enabled = true  # 是否启用该模块，默认值 ：true
http-timeout = "30s"  # http超时时间，默认值："30s"
insecure-skip-verify = false  # 是否允许不安全的证书
ca-certs = ""  # 设置CA证书
write-concurrency = 40  # 设置并发数目，默认值：40
write-buffer-size = 1000  # 设置buffer大小，默认值：1000
```

### graphite


```bash
[[graphite]]
enabled = false  # 是否启用该模块，默认值 ：false
database = "graphite"  # 数据库名称，默认值："graphite"
retention-policy = ""  # 存储策略，无默认值
bind-address = ":2003"  # 绑定地址，默认值：":2003"
protocol = "tcp"  # 协议，默认值："tcp"
consistency-level = "one"  # 一致性级别，默认值："one
batch-size = 5000  # 批量size，默认值：5000
batch-pending = 10  # 配置在内存中等待的batch数，默认值：10
batch-timeout = "1s"  # 超时时间，默认值："1s"
udp-read-buffer = 0  # udp读取buffer的大小，0表示使用操作系统提供的值，如果超过操作系统的默认配置则会出错。 该配置的默认值：0
separator = "."  # 多个measurement间的连接符，默认值： "."
```




### collectd

```bash
[[collectd]]
enabled = false  # 是否启用该模块，默认值 ：false
bind-address = ":25826"  # 绑定地址，默认值： ":25826"
database = "collectd"  # 数据库名称，默认值："collectd"
retention-policy = ""  # 存储策略，无默认值
typesdb = "/usr/local/share/collectd"  # 路径，默认值："/usr/share/collectd/types.db"
auth-file = "/etc/collectd/auth_file"
batch-size = 5000
batch-pending = 10
batch-timeout = "10s"
read-buffer = 0  # udp读取buffer的大小，0表示使用操作系统提供的值，如果超过操作系统的默认配置则会出错。默认值：0
```



### opentsdb

```bash
[[opentsdb]]
enabled = false  # 是否启用该模块，默认值：false
bind-address = ":4242"  # 绑定地址，默认值：":4242"
database = "opentsdb"  # 默认数据库："opentsdb"
retention-policy = ""  # 存储策略，无默认值
consistency-level = "one"  # 一致性级别，默认值："one"
tls-enabled = false  # 是否开启tls，默认值：false
certificate= "/etc/ssl/influxdb.pem"  # 证书路径，默认值："/etc/ssl/influxdb.pem"
log-point-errors = true  # 出错时是否记录日志，默认值：true
batch-size = 1000
batch-pending = 5
batch-timeout = "1s"
```

### udp

```bash
[[udp]]
enabled = false  # 是否启用该模块，默认值：false
bind-address = ":8089"  # 绑定地址，默认值：":8089"
database = "udp"  # 数据库名称，默认值："udp"
retention-policy = ""  # 存储策略，无默认值
batch-size = 5000
batch-pending = 10
batch-timeout = "1s"
read-buffer = 0  # udp读取buffer的大小，0表示使用操作系统提供的值，如果超过操作系统的默认配置则会出错。 该配置的默认值：0　
```


### continuous_queries

```bash
[continuous_queries]
enabled = true  # enabled 是否开启CQs，默认值：true
log-enabled = true  # 是否开启日志，默认值：true
run-interval = "1s"  # 时间间隔，默认值："1s"
```