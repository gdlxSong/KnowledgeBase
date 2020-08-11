## 简介


>https://blog.csdn.net/youngtong/article/details/84640382?utm_source=app


可执行文件在 usr/bin/telegraf，配置文件在/etc/telegraf目录下，也可直接配置生成

### 生成配置

```bash
telegraf --input-filter <pluginname>[:<pluginname>] --output-filter <outputname>[:<outputname>] config > telegraf.conf
```

```bash
# 生成配置文件，输入插件cpu,mem,disk,diskio,net; 输出插件：influxdb,opentsdb
telegraf --input-filter cpu:mem:disk:diskio:net --output-filter influxdb:opentsdb config > telegraf.conf

```